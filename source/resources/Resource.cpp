#include <resources/Resource.h>

using namespace genesis;
using namespace std;

Resource::Resource(const ResourceDesc& desc) : Base(desc.base), m_graphicsDevice{desc.graphicsDevice}, m_path{string{desc.path}}, m_loaded{false} {}

Resource::~Resource() {}

const char* Resource::getPath() const noexcept
{
	return m_path.c_str();
}

bool Resource::isLoaded() const noexcept
{
	return m_loaded;
}

void Resource::load()
{
	if (isLoaded()) {
		GENESIS_LOG_WARNING("Resoure is already loaded: {}", m_path);
		return;
	}

	onLoad();
	m_loaded = true;
}

void Resource::unload()
{
	if (!isLoaded()) {
		GENESIS_LOG_WARNING("Resoure is already unloaded: {}", m_path);
		return;
	}

	onUnload();
	m_loaded = false;
}