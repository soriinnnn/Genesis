#ifndef GENESIS_SPRITE_BATCH_H
#define GENESIS_SPRITE_BATCH_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>
#include <math/Point.h>
#include <math/Vec4.h>
#include <math/Vec2.h>
#include <SpriteBatch.h>
#include <CommonStates.h>

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

		void begin(SamplerState* samplerState = nullptr);
		void drawText(FontAtlas& font, const char* text, Point pos, Vec2 scale, Vec4 color);
		void drawImage(ImageTexture& image, Rect size, Point pos, Vec2 scale, Vec4 color);
		void drawSolid(Rect size, Point pos, Vec2 scale, Vec4 color);
		void end();

	private:
		UniquePtr<DirectX::DX11::SpriteBatch> m_batch;
		UniquePtr<DirectX::CommonStates> m_states;
		SharedPtr<ImageTexture> m_whiteTexture;
	};
}


#endif