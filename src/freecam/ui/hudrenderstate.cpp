
#include "hudrenderstate.hpp"
#include "../bxhelpers.hpp"

HUDRenderState::HUDRenderState() {
	pFont = getGame()->debugFont;
}

void HUDRenderState::save() {
	savedScale = pFont->scale;
	savedFontColor = pFont->color;
	savedShadowOffset = pFont->shadowOffset;
	savedShadowColor = pFont->shadowColor;
}

void HUDRenderState::restore() {
	pFont->scale = savedScale;
	pFont->color = savedFontColor;
	pFont->shadowColor = savedShadowColor;
	pFont->shadowOffset = savedShadowOffset;
}
