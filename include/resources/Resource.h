#ifndef GENESIS_RESOURCE_H
#define GENESIS_RESOURCE_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis 
{
	struct ResourceDesc
	{
		BaseDesc base;
		const char* path;
	};

	class Resource: public Base
	{
	public:
		~Resource() override;

		const char* getPath() const noexcept;
		bool isLoaded() const noexcept;

		virtual void load() = 0;
		virtual void unload() = 0;

	protected:
		explicit Resource(const ResourceDesc& desc);

	protected:
		std::string m_path;
		bool m_isLoaded;
	};
}

#endif