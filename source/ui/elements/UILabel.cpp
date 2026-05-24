#include <ui/elements/UILabel.h>
#include <resources/Font.h>
#include <graphics/resources/FontAtlas.h>
#include <graphics/resources/SpriteBatch.h>
#include <cmath>

using namespace genesis;
using namespace std;

static float getCharAdvance(const FontAtlas::Glyph& glyph);
static float getCharWidth(const FontAtlas::Glyph& glyph);
static void flushWord(WString& result, WString& currentWord, float& lineWidth, float& wordWidth, bool& lineStart, const FontAtlas& atlas, float maxWidth);
static void handleLineBreak(WString& result, WString& currentWord, float& lineWidth, float& wordWidth, bool& lineStart, const FontAtlas& atlas, float maxWidth);
static void handleWhiteSpace(WString& result, WString& currentWord, float& lineWidth, float& wordWidth, bool& lineStart, const FontAtlas& atlas, float maxWidth);
static void handleWordOverflow(WString& result, WString& currentWord, float& lineWidth, float& wordWidth, bool& lineStart, const FontAtlas& atlas, float maxWidth);

UILabel::UILabel(const UIElementDesc& desc): UIElement(desc) {}

UILabel::~UILabel() {}

void UILabel::setContent(const char* content)
{
	m_content = WString{content, content + strlen(content)};
	adjustContent(m_size);
}

void UILabel::setFont(const SharedPtr<Font>& font)
{
	m_font = font;
	adjustContent(m_size);
}

void UILabel::adjustContent(const Rect& size)
{
	if (!m_font) {
		return;
	}

	Rect contentSize = m_font->getAtlas().getSize(m_content.c_str());
	if (size.width() >= contentSize.width()) {
		m_adjustedContent = m_content;
		return;
	}

	WString result;
	WString currentWord;
	float lineWidth = 0.0f;
	float wordWidth = 0.0f;
	bool lineStart = true;
	const FontAtlas& atlas = m_font->getAtlas();
	float maxWidth = static_cast<float>(size.width());

	for (int i = 0; i < m_content.size(); i++) {
		wchar_t wc = m_content[i];

		if (wc == L'\n') {
			handleLineBreak(result, currentWord, lineWidth, wordWidth, lineStart, atlas, maxWidth);
			continue;
		}
		
		if (wc == L' ') {
			handleWhiteSpace(result, currentWord, lineWidth, wordWidth, lineStart, atlas, maxWidth);
			continue;
		}

		FontAtlas::Glyph glyph = atlas.getGlyph(wc);
		currentWord += wc;
		wordWidth += getCharAdvance(glyph);
	}
	flushWord(result, currentWord, lineWidth, wordWidth, lineStart, atlas, maxWidth);

	m_adjustedContent = std::move(result);
}

void UILabel::onRender(SpriteBatch& batch)
{
	if (!m_font) {
		return;
	}
	batch.drawSolid(m_size, m_position, m_scale, {1.0f, 0.0f, 0.0f, 1.0f});
	batch.drawText(m_font->getAtlas(), m_adjustedContent.c_str(), getGlobalPosition(), m_scale, m_color);
}

void UILabel::onSize()
{
	adjustContent(m_size);
}

/* STATIC FUNCTION DEFINITIONS */

float getCharAdvance(const FontAtlas::Glyph& glyph)
{
	return getCharWidth(glyph) + glyph.xAdvance;
}

float getCharWidth(const FontAtlas::Glyph& glyph)
{
	return static_cast<float>(glyph.subRect.width()) + glyph.xOffset;
}

/*
 * Bolca la paraula actual al resultat, aplicant els salts de línia necessaris.
 * 
 * - Si la paraula sencera és més ampla que l’amplada màxima, es parteix caràcter a caràcter
 *   (handleWordOverflow) repartint‑la en tantes línies com calgui.
 * - Si la paraula cap en una línia però no entra a la línia actual, s’insereix un salt
 *   de línia abans d’escriure‑la.
 * - Si cap perfectament a la línia actual, s’afegeix sense salts addicionals.
 * - Permet el desbordament d’un sol caràcter quan aquest excedeix l’amplada màxima.
 * 
 */
void flushWord(WString& result, WString& currentWord, float& lineWidth, float& wordWidth, bool& lineStart, const FontAtlas& atlas, float maxWidth)
{
	if (currentWord.empty()) {
		return;
	}

	FontAtlas::Glyph lastGlyph = atlas.getGlyph(currentWord.back());
	float width = getCharWidth(lastGlyph);
	float advance = getCharAdvance(lastGlyph);

	float visualWordWidth;
	if (currentWord.size() == 1) {
		visualWordWidth = width;
	}
	else {
		visualWordWidth = wordWidth - advance + width;
	}

	if (visualWordWidth > maxWidth) {
		handleWordOverflow(result, currentWord, lineWidth, wordWidth, lineStart, atlas, maxWidth);
		return;
	}

	if (!lineStart && lineWidth + visualWordWidth > maxWidth) {
		result += L'\n';
		lineWidth = 0.0f;
		lineStart = true;
	}

	result += currentWord;
	lineWidth += wordWidth;
	lineStart = false;
	currentWord.clear();
	wordWidth = 0.0f;
}

void handleLineBreak(WString& result, WString& currentWord, float& lineWidth, float& wordWidth, bool& lineStart, const FontAtlas& atlas, float maxWidth)
{
	flushWord(result, currentWord, lineWidth, wordWidth, lineStart, atlas, maxWidth);
	result += L'\n';
	lineWidth = 0.0f;
	lineStart = true;
}

void handleWhiteSpace(WString& result, WString& currentWord, float& lineWidth, float& wordWidth, bool& lineStart, const FontAtlas& atlas, float maxWidth)
{
	flushWord(result, currentWord, lineWidth, wordWidth, lineStart, atlas, maxWidth);

	FontAtlas::Glyph glyph = atlas.getGlyph(L' ');
	float advance = getCharAdvance(glyph);

	if (!lineStart && lineWidth + advance > maxWidth) {
		result += L'\n';
		lineWidth = 0.0f;
		lineStart = true;
	}
	else {
		result += L' ';
		lineWidth += advance;
		lineStart = false;
	}
}

/* 
 * Permet desbordament de caràcters quan un sol caràcter és més gran que la mida de l'element.
 */
void handleWordOverflow(WString& result, WString& currentWord, float& lineWidth, float& wordWidth, bool& lineStart, const FontAtlas& atlas, float maxWidth)
{
	for (wchar_t wc : currentWord) {
		FontAtlas::Glyph glyph = atlas.getGlyph(wc);
		float width = getCharWidth(glyph);
		float advance = getCharAdvance(glyph);

		if (!lineStart && lineWidth + width > maxWidth) {
			result += L'\n';
			lineWidth = 0.0f;
			lineStart = true;
		}	

		result += wc;
		lineWidth += advance;
		lineStart = false;
	}

	currentWord.clear();
	wordWidth = 0.0f;
}