#ifndef GENESIS_ASSET_MANAGER_H
#define GENESIS_ASSET_MANAGER_H
#include <string>

namespace genesis
{
	class AssetManager
	{
	public:
		static std::string readFile(const char* filepath);
	};
}

#endif