#ifndef GENESIS_MESH_H
#define GENESIS_MESH_H
#include <resources/Resource.h>

namespace genesis
{
	struct MeshDesc 
	{

	};

	class Mesh final: public Resource
	{
	public:
		Mesh(const MeshDesc& mdesc, const ResourceDesc& rdesc);
		~Mesh() override;


		void load() override;
		void unload() override;

	private:

	};
}

#endif