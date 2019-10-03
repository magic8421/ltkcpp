//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextEdit.h"
#include "TextEdit_p.h"
#include "StyleManager.h"
#include "ScrollBar.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {


TextEdit::TextEdit() : Sprite(new TextEditPrivate(this))
{
	d_func()->Init();
}

TextEdit::TextEdit(TextEditPrivate *d)
{
	d->Init();
}

TextEdit::~TextEdit()
{
}

TextEditPrivate::TextEditPrivate(TextEdit *q) : SpritePrivate(q)
{
}

TextEditPrivate::~TextEditPrivate()
{
	auto d = this;
	SAFE_RELEASE(d->format);
	SAFE_RELEASE(d->layout);
	SAFE_RELEASE(d->brushSelectedText);
}

void TextEditPrivate::Init()
{
	auto d = this;
	auto q = q_func();

	d->cursorPos = d->text.size();

	HRESULT hr = GetDWriteFactory()->CreateTextFormat(L"Î¢ÈíÑÅºÚ", NULL, DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"zh-cn",
		&d->format);
	LTK_ASSERT(SUCCEEDED(hr));

	q->EnableClipChildren(true);

	d->vsb = new ScrollBar(ScrollBar::Vertical);
	q->AddChild(d->vsb);
}

bool TextEdit::OnPaint(PaintEvent *ev)
{
	LTK_D(TextEdit);
    auto target = ev->target;
    auto rc = this->GetClientRect();

    HRESULT hr = S_OK;
    DWRITE_TEXT_METRICS textMetrics;
	hr = d->layout->GetMetrics(&textMetrics);
    LTK_ASSERT(SUCCEEDED(hr));
	d->scrollAni.UpdateScroll(textMetrics.height - rc.Height);
	if (!d->scrollAni.IsRunning()) {
		this->EndAnimation();
	}
	d->vsb->SetPosition(d->scrollAni.GetScroll());
    //LTK_LOG("scroll: %f", d->scrollAni.GetScroll());

    rc.X = 0.5f;
    rc.Y = 0.5f;
    rc.Width -= 0.5f;
    rc.Height -= 0.5f;
	auto brush = GetWindow()->GetStockBrush();
	brush->SetColor(StyleManager::ColorFromString("#000000"));
    target->DrawRectangle(D2D1RectF(rc), brush);

	if (d->selection > 0) {
		UINT32 begin = d->cursorPos;
		UINT32 end = d->selection;
        if (begin > end) {
            std::swap(begin, end);
        }
        UINT32 len = 0;
        std::vector<DWRITE_HIT_TEST_METRICS> vecMetrics;
		d->layout->HitTestTextRange(begin, end - begin, 0.0f, 0.0f,
            NULL, 0, &len);
        LTK_ASSERT(SUCCEEDED(hr));
        //LTK_LOG("HitTestTextRange: %d metrics", len);
        vecMetrics.resize(len);
		d->layout->HitTestTextRange(begin, end - begin, d->padding, 0.0f,
            &vecMetrics[0], len, &len);
        LTK_ASSERT(SUCCEEDED(hr));
		brush->SetColor(StyleManager::ColorFromString("#0000ff"));
        for (auto &m : vecMetrics) {
            D2D1_RECT_F rc2;
            rc2.left = m.left;
            rc2.right = rc2.left + m.width;
			rc2.top = m.top - d->scrollAni.GetScroll();
            rc2.bottom = rc2.top + m.height;
            target->FillRectangle(rc2, brush);
        }
    }

    D2D_POINT_2F pt;
	pt.x = d->padding;
	pt.y = -d->scrollAni.GetScroll();
	brush->SetColor(StyleManager::ColorFromString("#000000"));
	target->DrawTextLayout(pt, d->layout, brush);

    return false;
}

void TextEditPrivate::DeleteSelected()
{
	auto d = this;

	if (d->selection < 0) {
        return;
    }
	UINT begin = d->cursorPos;
	UINT end = d->selection;
    if (begin > end) {
        std::swap(begin, end);
    }
	d->text.erase(d->text.begin() + begin, d->text.begin() + end);
#pragma warning(push)
#pragma warning(disable:4018)
	d->cursorPos = min(d->cursorPos, d->selection); // TODO FIXME
	d->cursorPos = max(0, d->cursorPos);
	d->cursorPos = min(d->cursorPos, d->text.size() - 1);
#pragma warning(pop)
	d->selection = -1;
}

bool TextEdit::OnChar(KeyEvent *ev)
{
	LTK_D(TextEdit);

    wchar_t ch = (wchar_t)ev->keyCode;
    if (ch == VK_BACK) {
		if (d->selection >= 0) {
            d->DeleteSelected();
		}
		else if (d->cursorPos > 0) {
			d->text.erase(d->text.begin() + d->cursorPos - 1);
			d->cursorPos--;
        } else {
            return true;
        }
    } else {
		if (d->selection >= 0) {
            d->DeleteSelected();
        }
		d->text.insert(d->text.begin() + d->cursorPos, ch);
		d->cursorPos++;
    }
	//LTK_LOG("key code: 0x%04x d->cursorPos: %d", ch, d->cursorPos);
    this->RecreateLayout();
    this->UpdateCursor(true);
    return true;
}

bool TextEdit::OnKeyDown(KeyEvent *ev)
{
	LTK_D(TextEdit);

    wchar_t ch = (wchar_t)ev->keyCode;
    HRESULT hr = S_OK;
    float x = 0.0f;
    float y = 0.0f;
    DWRITE_HIT_TEST_METRICS dhtm = { 0 };

    if (ch == VK_DELETE) { // TODO not here to handle.. maybe OnKeyDown
		if (d->selection >= 0) {
            d->DeleteSelected();
		}
		else if (d->cursorPos < (int)d->text.size()) {
			d->text.erase(d->text.begin() + d->cursorPos);
        } else {
            return true;
        }
    } else if (ch == VK_LEFT) {
		d->selection = -1;
		if (d->cursorPos > 0) {
			d->cursorPos--;
        }
    } else if (ch == VK_RIGHT) {
		d->selection = -1;
		if (d->cursorPos < (int)d->text.size()) {
			d->cursorPos++;
        }
    } else if (ch == VK_UP) {
		d->selection = -1;
		hr = d->layout->HitTestTextPosition(d->cursorPos, FALSE, &x, &y, &dhtm);
        LTK_ASSERT(SUCCEEDED(hr));
		d->cursorPos = d->HitTest(x, y - 1.0f - d->scrollAni.GetScroll());
    } else if (ch == VK_DOWN) {
		d->selection = -1;
		hr = d->layout->HitTestTextPosition(d->cursorPos, FALSE, &x, &y, &dhtm);
        LTK_ASSERT(SUCCEEDED(hr));
		d->cursorPos = d->HitTest(x, y + dhtm.height + 1.0f - d->scrollAni.GetScroll());
    }
    this->RecreateLayout(); // TODO move to VK_DELETE branch
    this->UpdateCursor(true);
    return true;
}

bool TextEdit::OnImeInput(ImeEvent *ev)
{
	LTK_D(TextEdit);

	if (d->selection >= 0) {
        d->DeleteSelected();
    }
	d->text.insert(d->cursorPos, ev->text);
	d->cursorPos += wcslen(ev->text);
    this->RecreateLayout();
    this->UpdateCursor(true);
    return true;
}

void TextEdit::RecreateLayout()
{
	LTK_D(TextEdit);

	SAFE_RELEASE(d->layout);
    auto rc = this->GetClientRect();
    HRESULT hr = GetDWriteFactory()->CreateTextLayout(
		d->text.c_str(), d->text.size(), d->format,
		rc.Width - d->padding * 2, 0.0f, &d->layout);
    LTK_ASSERT(SUCCEEDED(hr));

	if (d->selection > 0) {
		UINT32 begin = d->cursorPos;
		UINT32 end = d->selection;
        if (begin > end) {
            std::swap(begin, end);
        }
		d->layout->SetDrawingEffect(
			d->brushSelectedText, DWRITE_TEXT_RANGE{ begin, end - begin });
    }

    DWRITE_TEXT_METRICS textMetrics;
	hr = d->layout->GetMetrics(&textMetrics);
    LTK_ASSERT(SUCCEEDED(hr));
    float layout_h = textMetrics.height;
    //LTK_LOG("RecreateLayout layout_h:%f", layout_h);
    if (layout_h > rc.Height) {
		d->vsb->SetVisible(true);
		d->vsb->SetContentSize(layout_h);
    } else {
		d->vsb->SetVisible(false);
    }
    this->Invalidate();
}

void TextEdit::UpdateCursor(bool bEnsureVisible)
{
	LTK_D(TextEdit);

    HRESULT hr = S_OK;
    float x = 0.0f;
    float y = 0.0f;
    DWRITE_HIT_TEST_METRICS dhtm = { 0 };
	/*if (!d->isInside) {
        hr = d->layout->HitTestTextPosition(d->cursorPos - 1, FALSE, &x, &y, &dhtm);
        LTK_ASSERT(SUCCEEDED(hr));
        x += dhtm.width;
    } else*/ {
		hr = d->layout->HitTestTextPosition(d->cursorPos, FALSE, &x, &y, &dhtm);
        LTK_ASSERT(SUCCEEDED(hr));
    }
    //LTK_LOG("char at caret: 0x%04x", d->text[d->cursorPos]);
	y -= d->scrollAni.GetScroll();
    if (bEnsureVisible) {
        auto rc = this->GetClientRect();
        float delta = y + dhtm.height - rc.Height;
        if (delta > 0.01f) {
			d->scrollAni.SetScroll(d->scrollAni.GetScroll() + delta);
            y -= delta;
        }
        if (y < -0.01f) {
			d->scrollAni.SetScroll(d->scrollAni.GetScroll() + y);
            // y = 0.0f;
        }
        LTK_LOG("delta: %.1f y: %.1f", delta, y);
    }
	RectF rcCursor(x + d->padding, y, 1, dhtm.height);
    this->SetCaretPos(rcCursor);
}

bool TextEdit::OnMouseWheel(MouseEvent *ev)
{
	LTK_D(TextEdit);

	//LTK_LOG("delta %.2f", ev->delta);
	d->scrollAni.BeginScroll(ev->delta);
    this->HideCaret();
    this->BeginAnimation();
    return true;
}

int TextEditPrivate::HitTest(float x, float y)
{
	auto d = this;

    int pos = -1;
	y += d->scrollAni.GetScroll();
    BOOL isTrailingHit = FALSE;
    BOOL isInside = FALSE;
    DWRITE_HIT_TEST_METRICS dhtm = { 0 };
	HRESULT hr = d->layout->HitTestPoint(
        x, y, &isTrailingHit, &isInside, &dhtm);
    LTK_ASSERT(SUCCEEDED(hr));
    //LTK_LOG("isTrailingHit: %d, isInside: %d", isTrailingHit, isInside);
	d->isInside = isInside ? true : false;

    if (!isTrailingHit) {
        pos = dhtm.textPosition;
    } else {
        pos = dhtm.textPosition + 1;
    }
	if (pos > (int)d->text.size()) {
        __debugbreak();
    }
    return pos;
}

bool TextEdit::OnLBtnDown(MouseEvent *ev)
{
	LTK_D(TextEdit);

	ev->x -= d->padding;
	d->scrollAni.Stop();
    this->EndAnimation();
	d->cursorPos = d->HitTest(ev->x, ev->y);

	if (d->selection >= 0) {
		d->selection = -1;
		d->prevSelection = -1;
        this->RecreateLayout();
    }
    this->UpdateCursor(false);
    this->SetCapture();
	d->bCapture = true;
	this->SetFocus();
    return false;
}

bool TextEdit::OnLBtnUp(MouseEvent *ev)
{
	LTK_D(TextEdit);

	d->bCapture = false;
    this->ReleaseCapture();
    return false;
}

bool TextEdit::OnMouseMove(MouseEvent *ev)
{
	LTK_D(TextEdit);

	if (!d->bCapture) {
        return false;
    }
	d->selection = d->HitTest(ev->x, ev->y);
    //LTK_LOG("d->selection:%d", d->selection);
	if (d->selection != d->prevSelection) {
		d->prevSelection = d->selection;
        this->RecreateLayout();
    }
    return false;
}

bool TextEdit::OnSize(SizeEvent *ev)
{
	LTK_D(TextEdit);

    this->RecreateLayout();

	d->vsb->SetRect(RectF(ev->width - 8, 0, 6, ev->height));
    
    return false;
}

void TextEdit::OnRecreateResouce(ID2D1RenderTarget *target)
{
	LTK_D(TextEdit);

	SAFE_RELEASE(d->brushSelectedText);
    auto textColor = StyleManager::ColorFromString("#ffffff");
	auto hr = target->CreateSolidColorBrush(textColor, &d->brushSelectedText);
    LTK_ASSERT(SUCCEEDED(hr));
}

} // namespace ltk