#pragma once

#include <string>

namespace tama
{
	using FilePath = std::wstring;

	namespace FileSystem
	{
		bool Exists(const FilePath& path);

		size_t GetFileSize(const FilePath& path);
	}
}