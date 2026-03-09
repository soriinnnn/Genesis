#include <resources/Resource.h>

using namespace genesis;
using namespace std;

Resource::Resource(const ResourceDesc& desc): Base(desc.base) 
{
	m_path = string{desc.path};
	m_isLoaded = false;
}

Resource::~Resource() {}

const char* Resource::getPath() const noexcept
{
	return m_path.c_str();
}

bool Resource::isLoaded() const noexcept
{
	return m_isLoaded;
}
