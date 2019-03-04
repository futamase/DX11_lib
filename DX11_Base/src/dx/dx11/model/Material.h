#pragma once

#include <assimp/scene.h>
#include <assimp\material.h>
#include <string>

namespace tama
{
	struct Material {
		Material()
			: has_texture(false)
		{
		}

		bool has_texture;
		std::string texture_name;
	};


	// ƒ}ƒeƒŠƒAƒ‹‚ð¶¬
	Material createMaterial(const aiMaterial* const mat) 
	{
		Material material;

		{
			aiColor3D color;
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		}

		{
			aiColor3D color;
			mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		}

		{
			aiColor3D color;
			mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		}

		float shininess = 80.0f;
		mat->Get(AI_MATKEY_SHININESS, shininess);

		{
			aiColor3D color;
			mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		}

		aiString tex_name;
		if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), tex_name) == AI_SUCCESS) {
			material.has_texture = true;
		}

//		ci::app::console() << "Diffuse:" << material.body.getDiffuse() << std::endl;
//		ci::app::console() << "Ambient:" << material.body.getAmbient() << std::endl;
//		ci::app::console() << "Specular:" << material.body.getSpecular() << std::endl;
//		ci::app::console() << "Shininess:" << material.body.getShininess() << std::endl;
//		ci::app::console() << "Emission:" << material.body.getEmission() << std::endl;
//		if (material.has_texture) {
//			ci::app::console() << "Texture:" << material.texture_name << std::endl;
//		}

		return material;
	}
}