#ifndef GENESIS_SPRITE_BATCH_H
#define GENESIS_SPRITE_BATCH_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Point.h>
#include <math/Vec4.h>
#include <SpriteBatch.h>

namespace genesis
{
	struct SpriteBatchDesc 
	{
		DeviceContext& deviceContext;
	};

	class SpriteBatch: public GraphicsResource
	{
	public:
		SpriteBatch(const SpriteBatchDesc& sdesc, const GraphicsResourceDesc& gdesc);
		~SpriteBatch() override;

		void drawText(FontAtlas& font, const char* text, Point pos, Vec4 color);

	private:
		UniquePtr<DirectX::DX11::SpriteBatch> m_batch;

		friend class GraphicsEngine;
	};
}


#endif