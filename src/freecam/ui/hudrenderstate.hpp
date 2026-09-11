#include <bx/font.h>

class HUDRenderState {
	t2Vector savedShadowOffset;
	tARGB savedFontColor;
	tARGB savedShadowColor;
	t2Vector savedScale;

	cFont* pFont;
public:

	HUDRenderState();

	/// Saves the old render state before we modify it.
	void save();

	/// Restores it.
	void restore();

	void setFontColor(const tARGB& color) {
		pFont->color = color;
	}

	void setShadowColor(const tARGB& color) {
		pFont->shadowColor = color;
	}

	void setShadowOffset(const t2Vector& offset) {
		pFont->shadowOffset = offset;
	}

	void setScale(const t2Vector& scale) {
		pFont->scale = scale;
	}

	cFont* getFont() const {  return pFont; }

};
