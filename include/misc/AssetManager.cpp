#include <misc/AssetManager.h>
#include <fstream>

using namespace genesis;
using namespace std;

string AssetManager::readFile(const char* filepath)
{
	ifstream stream(filepath);
	if (!stream) {
		return string{};
	}

	return string(istreambuf_iterator<char>(stream), istreambuf_iterator<char>());
}
