#pragma once
#include "Sprite_p.h"

namespace ltk {

class ScrollBar;

class TextEditPrivate : public SpritePrivate
{
public:
	TextEditPrivate(TextEdit *q);
	virtual ~TextEditPrivate();

	void Init();
	void DeleteSelected();
	int HitTest(float x, float y);

private:
	int cursorPos = 0;
	int selection = -1;
	int prevSelection = -1;

	wstring text;
	IDWriteTextFormat *format = nullptr;
	IDWriteTextLayout *layout = nullptr;
	ID2D1SolidColorBrush *brushSelectedText = nullptr;

	ScrollBar *vsb = nullptr;
	ScrollAnimation scrollAni;
	bool isInside = true;
	bool bCapture = false;

	float padding = 10.0f;

private:
	LTK_DECLARE_PUBLIC(TextEdit);
};

} // namespace ltk
