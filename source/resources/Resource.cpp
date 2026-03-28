#include <resources/Resource.h>

using namespace genesis;
using namespace std;

Resource::Resource(const ResourceDesc& desc): Base(desc.base), m_id{desc.id}, m_path{desc.path} {}

Resource::~Resource() {}

ResourceId Resource::getId() const noexcept
{
	return m_id;
}

const char* Resource::getPath() const noexcept
{
	return m_path.c_str();
}