#include <graphics/resources/FontAtlas.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;
using namespace std;
using namespace DirectX;
using namespace DirectX::DX11;

FontAtlas::FontAtlas(const FontAtlasDesc& fdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	try {
		WString path{fdesc.path, fdesc.path + strlen(fdesc.path)};
		m_spriteFont = make_unique<SpriteFont>(&m_device, path.c_str());
	}
	catch (const std::exception&) {
		GENESIS_LOG_THROW_ERROR("FontAtlas creation failed for \"{}\". Check font path or the font itself.", fdesc.path);
	}
	m_spriteFont->SetDefaultCharacter(L'?');
	m_spriteFont->SetPixelAlignment(true);
}

FontAtlas::~FontAtlas() {}

bool FontAtlas::contains(wchar_t character) const
{
	return m_spriteFont->ContainsCharacter(character);
}

Rect FontAtlas::getSize(const wchar_t* text) const
{
	RECT bounds = m_spriteFont->MeasureDrawBounds(text, DirectX::XMFLOAT2(0.0f, 0.0f));
	return Rect{
		bounds.left + bounds.right,
		bounds.top + bounds.bottom
	};
}

FontAtlas::Glyph FontAtlas::getGlyph(wchar_t character) const
{
	const SpriteFont::Glyph* dxGlyph = m_spriteFont->FindGlyph(character);

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
	return m_spriteFont->GetLineSpacing();
}

wchar_t FontAtlas::getDefaultCharacter() const
{
	return m_spriteFont->GetDefaultCharacter();
}

void FontAtlas::setPixelAlignment(bool enable)
{
	m_spriteFont->SetPixelAlignment(enable);
}

void FontAtlas::setLineSpacing(float spacing)
{
	m_spriteFont->SetLineSpacing(spacing);
}

void FontAtlas::setDefaultCharacter(wchar_t character)
{
	m_spriteFont->SetDefaultCharacter(character);
}