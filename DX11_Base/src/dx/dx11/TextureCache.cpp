#include "TextureCache.h"

namespace tama
{
	void TextureCache::Add(const std::wstring& key, const std::wstring& filePath)
	{
		if (m_texList.find(key) != m_texList.end())
			throw std::runtime_error("そのキーは使用されています");

		Texture tex;
		tex.Load(filePath);
		m_texList.emplace(key, tex);
	}

	Texture& TextureCache::At(const std::wstring& key)
	{
		if (m_cache.first == key)
			return *m_cache.second;
		else
		{
			m_cache.first = key;
			m_cache.second = &m_texList.at(key);
			return *m_cache.second;
		}
	}

	const Texture& TextureCache::At(const std::wstring& key) const
	{
		static std::pair<std::wstring, const Texture*> constCache;
		if (constCache.first == key)
			return *constCache.second;
		else
		{
			constCache.first = key;
			constCache.second = &m_texList.at(key);
			return *constCache.second;
		}
	}

	bool TextureCache::Exist(const std::wstring& key) const
	{
		return m_texList.find(key) != m_texList.end();
	}

	void TextureCache::Remove(const std::wstring& key)
	{
		m_texList.erase(key);
	}

	void TextureCache::RemoveAll()
	{
		m_texList.clear();
	}
}

