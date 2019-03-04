#include  "FileSystem.h"

#include <boost\filesystem.hpp>

#ifdef _DEBUG
#pragma comment(lib, "libboost_filesystem-vc140-mt-gd-1_62")
#pragma comment(lib, "libboost_system-vc140-mt-gd-1_62")
#else
#pragma comment(lib, "libboost_filesystem-vc140-mt-1_62")
#pragma comment(lib, "libboost_system-vc140-mt-1_62")
#endif

namespace tama
{
	namespace FileSystem
	{
		namespace fs = boost::filesystem;

		bool Exists(const FilePath& path)
		{
			const fs::wpath boostPath(path);

			boost::system::error_code error;
			const bool result = fs::exists(path, error);

			return (result && !error);
		}

		size_t GetFileSize(const FilePath& path)
		{
			const fs::wpath boostPath(path);

			return fs::file_size(path);
		}
	}
}