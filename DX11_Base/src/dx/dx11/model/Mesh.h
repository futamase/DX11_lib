#pragma once
//
// メッシュ
//

#include <assimp/scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <string>
#include <map>
#include <vector>

struct Vertex
{
	aiVector3D position;
	aiVector2D texcoord;
	aiVector3D normal;

	Vertex() = default;

	Vertex(const aiVector3D& pos, const aiVector2D& tex, const aiVector3D& _normal) :
		position(pos), texcoord(tex), normal(_normal)
	{}
};


// 頂点バッファとかを持たせたいなら継承にしたいなあ
class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;

	bool LoadMesh(const std::string& fileName)
	{
		// 本当は複数回インスタンス化したくない
		// スレッドセーフじゃないから排他制御の考慮の必要あり
		Assimp::Importer importer;

		auto pScene = importer.ReadFile(fileName.c_str(),
			aiProcess_Triangulate);

		if (!pScene)
			return false;

		return InitFromScene(pScene, fileName);
	}


private:
	bool InitFromScene(const aiScene* pScene, const std::string& fileName)
	{
		m_entries.resize(pScene->mNumMeshes);

		for (size_t i = 0; i < m_entries.size(); i++)
		{
			auto pMesh = pScene->mMeshes[i];
			InitMesh(i, pMesh);
		}

		return InitMaterials(pScene, fileName);
	}

	void InitMesh(unsigned int index, const aiMesh* pMesh)
	{
		m_entries[index].materialIndex = pMesh->mMaterialIndex;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		const aiVector3D zero(0, 0, 0);

		for (size_t i = 0; i < pMesh->mNumVertices; i++)
		{
			const aiVector3D* pPos = &(pMesh->mVertices[i]);
			const aiVector3D* pNormal = &(pMesh->mNormals[i]);
			const aiVector3D* pTexCoord = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : &zero;

			Vertex v(aiVector3D(pPos->x, pPos->y, pPos->z),
				aiVector2D(pTexCoord->x, pTexCoord->y),
				aiVector3D(pNormal->x, pNormal->y, pNormal->z));

			vertices.emplace_back(v);
		}

		for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
			const aiFace& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.emplace_back(face.mIndices[0]);
			indices.emplace_back(face.mIndices[1]);
			indices.emplace_back(face.mIndices[2]);
		}

		m_entries.at(index).vertices = std::move(vertices);
		m_entries.at(index).indices = std::move(indices);
		m_entries.at(index).Init(vertices, indices);
	}

	bool InitMaterials(const aiScene* pScene, const std::string& fileName)
	{
		std::string::size_type slashIndex = fileName.find_last_of("/");
		std::string Dir;

		if (slashIndex == std::string::npos) 
			Dir = ".";
		else if (slashIndex == 0) 
			Dir = "/";
		else 
			Dir = fileName.substr(0, slashIndex);
		

		bool Ret = true;

		for (unsigned int i = 0; i < pScene->mNumMaterials; i++) 
		{
			const aiMaterial* pMaterial = pScene->mMaterials[i];

			if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) <= 0)
				continue;

			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_FAILURE)
				continue;

			std::string FullPath = Dir + "/" + Path.data;

			m_texNames.emplace_back(FullPath);
		}

		return Ret;
	}

public:
	static const unsigned int INVALID_MATERIAL = 0xFFFFFFFF;

	struct MeshEntry
	{
		MeshEntry()
		{
			numIndices = 0;
			materialIndex = INVALID_MATERIAL;
		}
		~MeshEntry() = default;
		void Init(const std::vector<Vertex>& vertices,
			const std::vector<unsigned>& indices)
		{
			numIndices = indices.size();
		}
		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;
		unsigned int numIndices;
		unsigned int materialIndex;
	};
	const std::vector<MeshEntry>& GetMesh() const { return m_entries; }
private:

	std::vector<MeshEntry> m_entries;
	std::vector<std::string> m_texNames;
};

#if 0
struct Weight {
	uint32_t vertex_id;
	float value;
};

struct Bone {
	std::string name;
	ci::Matrix44f offset;

	std::vector<Weight> weights;
};

struct Mesh {
	Mesh()
		: has_bone(false)
	{}

	ci::TriMesh body;
	ci::TriMesh orig;

	uint32_t material_index;

	bool has_bone;
	std::vector<Bone> bones;
};


// ボーンの情報を作成
Bone createBone(const aiBone* b) {
	Bone bone;

	bone.name = b->mName.C_Str();
	bone.offset.set(b->mOffsetMatrix[0], true);

	ci::app::console() << "bone:" << bone.name << " weights:" << b->mNumWeights << std::endl;

	const aiVertexWeight* w = b->mWeights;
	for (u_int i = 0; i < b->mNumWeights; ++i) {
		Weight weight{ w[i].mVertexId, w[i].mWeight };
		bone.weights.push_back(weight);
	}

	return bone;
}

// メッシュを生成
Mesh createMesh(const aiMesh* const m) {
	Mesh mesh;

	// 頂点データを取り出す
	u_int num_vtx = m->mNumVertices;
	ci::app::console() << "Vertices:" << num_vtx << std::endl;

	const aiVector3D* vtx = m->mVertices;
	for (u_int h = 0; h < num_vtx; ++h) {
		mesh.body.appendVertex(fromAssimp(vtx[h]));
	}

	// 法線
	if (m->HasNormals()) {
		ci::app::console() << "Has Normals." << std::endl;

		const aiVector3D* normal = m->mNormals;
		for (u_int h = 0; h < num_vtx; ++h) {
			mesh.body.appendNormal(fromAssimp(normal[h]));
		}
	}

	// テクスチャ座標(マルチテクスチャには非対応)
	if (m->HasTextureCoords(0)) {
		ci::app::console() << "Has TextureCoords." << std::endl;

		const aiVector3D* uv = m->mTextureCoords[0];
		for (u_int h = 0; h < num_vtx; ++h) {
			mesh.body.appendTexCoord(ci::Vec2f(uv[h].x, uv[h].y));
		}
	}

	// 頂点カラー(マルチカラーには非対応)
	if (m->HasVertexColors(0)) {
		ci::app::console() << "Has VertexColors." << std::endl;

		const aiColor4D* color = m->mColors[0];
		for (u_int h = 0; h < num_vtx; ++h) {
			mesh.body.appendColorRgba(fromAssimp(color[h]));
		}
	}

	// 面情報
	if (m->HasFaces()) {
		ci::app::console() << "Has Faces." << std::endl;

		const aiFace* face = m->mFaces;
		for (u_int h = 0; h < m->mNumFaces; ++h) {
			assert(face[h].mNumIndices == 3);

			const u_int* indices = face[h].mIndices;
			mesh.body.appendTriangle(indices[0], indices[1], indices[2]);
		}
	}

	// 骨情報
	mesh.has_bone = m->HasBones();
	if (mesh.has_bone) {
		ci::app::console() << "Has Bones." << std::endl;

		aiBone** b = m->mBones;
		for (u_int i = 0; i < m->mNumBones; ++i) {
			mesh.bones.push_back(createBone(b[i]));
		}

		// 骨アニメーションで書き換えるので、元を保存
		mesh.orig = mesh.body;
	}

	mesh.material_index = m->mMaterialIndex;

	return mesh;
}
#endif