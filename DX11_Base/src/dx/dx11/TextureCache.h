#pragma once

#include "Texture.h"
#include <unordered_map>

namespace tama
{
	class TextureCache
	{
		std::unordered_map<std::wstring, Texture> m_texList;
		std::pair<std::wstring, Texture*> m_cache;

	public:
		void Add(const std::wstring& key, const std::wstring& filePath);

		Texture& At(const std::wstring& key);

		const Texture& At(const std::wstring& key) const;

		bool Exist(const std::wstring& key) const;

		void Remove(const std::wstring& key);

		void RemoveAll();

	public:
		static TextureCache& I()
		{
			static TextureCache instance;
			return instance;
		}
	private:
		TextureCache() = default;
		TextureCache(const TextureCache&) = delete;
		TextureCache& operator = (const TextureCache&) = delete;
	};
}