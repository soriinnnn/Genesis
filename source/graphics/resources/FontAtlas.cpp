#include <graphics/resources/FontAtlas.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;
using namespace std;
using namespace DirectX;
using namespace DirectX::DX11;

FontAtlas::FontAtlas(const FontAtlasDesc& fdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	try {
		String temp{fdesc.path};
		WString path{temp.begin(), temp.end()};
		m_font = make_unique<SpriteFont>(&m_device, path.c_str());
	}
	catch (const std::exception&) {
		GENESIS_LOG_THROW_ERROR("FontAtlas creation failed for \"{}\". Check font path or the font itself.", fdesc.path);
	}
	m_font->SetDefaultCharacter(L'?');
	m_font->SetPixelAlignment(true);
}

FontAtlas::~FontAtlas() {}

bool FontAtlas::contains(wchar_t character) const
{
	return m_font->ContainsCharacter(character);
}

Rect FontAtlas::getSize(const wchar_t* text) const
{
	RECT bounds = m_font->MeasureDrawBounds(text, DirectX::XMFLOAT2(0.0f, 0.0f));
	return Rect{
		bounds.left + bounds.right,
		bounds.top + bounds.bottom
	};
}

FontAtlas::Glyph FontAtlas::getGlyph(wchar_t character) const
{
	const SpriteFont::Glyph* dxGlyph = m_font->FindGlyph(character);

	Glyph glyph{};
	glyph.character = dxGlyph->Character;
	glyph.subRect = Rect{dxGlyph->Subrect.left, dxGlyph->Subrect.top, dxGlyph->Subrect.right, dxGlyph->Subrect.bottom};
	glyph.xOffset = dxGlyph->XOffset;
	glyph.yOffset = dxGlyph->YOffset;
	glyph.xAdvance = dxGlyph->XAdvance;

	return glyph;
}

float FontAtlas::getLineSpacing() const
{
	return m_font->GetLineSpacing();
}

void FontAtlas::setPixelAlignment(bool enable)
{
	m_font->SetPixelAlignment(enable);
}

void FontAtlas::setLineSpacing(float spacing)
{
	m_font->SetLineSpacing(spacing);
}

void FontAtlas::setDefaultCharacter(wchar_t character)
{
	m_font->SetDefaultCharacter(character);
}
