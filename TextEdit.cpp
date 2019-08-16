//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextEdit.h"
#include "StyleManager.h"
#include "ScrollBar.h"

namespace ltk {


TextEdit::TextEdit()
{
    //m_text = L"test... test ...";
    m_cursorPos = m_text.size();

    HRESULT hr = GetDWriteFactory()->CreateTextFormat(L"Î¢ÈíÑÅºÚ", NULL, DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"zh-cn",
        &m_format);
    LTK_ASSERT(SUCCEEDED(hr));

    this->EnableFocus(true);
    this->EnableClipChildren(true);

    m_vsb = new ScrollBar(ScrollBar::Vertical);
    this->AddChild(m_vsb);
}

TextEdit::~TextEdit()
{
    SAFE_RELEASE(m_format);
    SAFE_RELEASE(m_layout);
    SAFE_RELEASE(m_brush);
}

bool TextEdit::OnPaint(PaintEvent *ev)
{
    auto target = ev->target;
    auto rc = this->GetClientRect();

    HRESULT hr = S_OK;
    DWRITE_TEXT_METRICS textMetrics;
    hr = m_layout->GetMetrics(&textMetrics);
    LTK_ASSERT(SUCCEEDED(hr));
    if (m_scrollAni.UpdateScroll(textMetrics.height - rc.Height)) {
        this->EndAnimation();
        //this->UpdateCursor(false);
    }
    m_vsb->SetPosition(m_scrollAni.GetScroll());
    //LTK_LOG("scroll: %f", m_scrollAni.GetScroll());

    rc.X = 0.5f;
    rc.Y = 0.5f;
    rc.Width -= 0.5f;
    rc.Height -= 0.5f;
    target->DrawRectangle(D2D1RectF(rc), m_brush);

    D2D_POINT_2F pt;
    pt.x = 0.0f;
    pt.y = -m_scrollAni.GetScroll();
    target->DrawTextLayout(pt, m_layout, m_brush);

    return false;
}

bool TextEdit::OnChar(KeyEvent *ev)
{
    wchar_t ch = (wchar_t)ev->keyCode;
    if (ch == VK_BACK) {
        if (m_cursorPos > 0) {
            m_text.erase(m_text.begin() + m_cursorPos - 1);
            m_cursorPos--;
        } else {
            return true;
        }
    } else {
        m_text.insert(m_text.begin() + m_cursorPos, ch);
        m_cursorPos++;
    }
    //LTK_LOG("key code: 0x%04x m_cursorPos: %d", ch, m_cursorPos);
    this->RecreateLayout();
    this->UpdateCursor(true);
    return true;
}

bool TextEdit::OnKeyDown(KeyEvent *ev)
{
    wchar_t ch = (wchar_t)ev->keyCode;
    if (ch == VK_DELETE) { // TODO not here to handle.. maybe OnKeyDown
        if (m_cursorPos < (int)m_text.size()) {
            m_text.erase(m_text.begin() + m_cursorPos);
        } else {
            return true;
        }
    } else if (ch == VK_LEFT) {
        if (m_cursorPos > 0) {
            m_cursorPos--;
            this->UpdateCursor(true);
            return true;
        }
    } else if (ch == VK_RIGHT) {

    }
    this->RecreateLayout();
    this->UpdateCursor(true);
    return true;
}

bool TextEdit::OnImeInput(ImeEvent *ev)
{
    m_text.insert(m_cursorPos, ev->text);
    m_cursorPos += wcslen(ev->text);
    this->RecreateLayout();
    this->UpdateCursor(true);
    return true;
}

void TextEdit::RecreateLayout()
{
    SAFE_RELEASE(m_layout);
    auto rc = this->GetClientRect();
    HRESULT hr = GetDWriteFactory()->CreateTextLayout(
        m_text.c_str(), m_text.size(), m_format,
        rc.Width, 0.0f, &m_layout);
    LTK_ASSERT(SUCCEEDED(hr));

    DWRITE_TEXT_METRICS textMetrics;
    hr = m_layout->GetMetrics(&textMetrics);
    LTK_ASSERT(SUCCEEDED(hr));
    float layout_h = textMetrics.height;
    LTK_LOG("layout_h:%f", layout_h);
    if (layout_h > rc.Height) {
        m_vsb->SetVisible(true);
        m_vsb->SetContentSize(layout_h);
    } else {
        m_vsb->SetVisible(false);
    }
    this->Invalidate();
}

void TextEdit::UpdateCursor(bool bEnsureVisible)
{
    HRESULT hr = S_OK;
    float x = 0.0f;
    float y = 0.0f;
    DWRITE_HIT_TEST_METRICS dhtm = { 0 };
    /*if (!m_isInside) {
        hr = m_layout->HitTestTextPosition(m_cursorPos - 1, FALSE, &x, &y, &dhtm);
        LTK_ASSERT(SUCCEEDED(hr));
        x += dhtm.width;
    } else*/ {
        hr = m_layout->HitTestTextPosition(m_cursorPos, FALSE, &x, &y, &dhtm);
        LTK_ASSERT(SUCCEEDED(hr));
    }
    //LTK_LOG("char at caret: 0x%04x", m_text[m_cursorPos]);
    y -= m_scrollAni.GetScroll();
    if (bEnsureVisible) {
        auto rc = this->GetClientRect();
        float delta = y + dhtm.height - rc.Height;
        if (delta > 0.01f) {
            m_scrollAni.SetScroll(m_scrollAni.GetScroll() + delta);
        }
        if (y < -0.01f) {
            m_scrollAni.SetScroll(m_scrollAni.GetScroll() + y);
        }
        LTK_LOG("delta: %.1f y: %.1f", delta, y);
    }
    RectF rcCursor(x, y, 1, dhtm.height);
    this->SetCaretPos(rcCursor);
}

bool TextEdit::OnMouseWheel(MouseEvent *ev)
{
    m_scrollAni.BeginScroll(ev->delta);
    this->HideCaret();
    this->BeginAnimation();
    return true;
}

bool TextEdit::OnLBtnDown(MouseEvent *ev)
{
    m_scrollAni.Stop();
    this->EndAnimation();

    ev->y += m_scrollAni.GetScroll();
    BOOL isTrailingHit = FALSE;
    BOOL isInside = FALSE;
    DWRITE_HIT_TEST_METRICS dhtm = { 0 };
    HRESULT hr = m_layout->HitTestPoint(
        ev->x, ev->y, &isTrailingHit, &isInside, &dhtm);
    LTK_ASSERT(SUCCEEDED(hr));
    LTK_LOG("isTrailingHit: %d, isInside: %d", isTrailingHit, isInside);
    m_isInside = isInside ? true: false;

   /* if (!isInside) {
        m_cursorPos = dhtm.textPosition;
    }
    else */if (!isTrailingHit) {
        m_cursorPos = dhtm.textPosition;
    } else {
        m_cursorPos = dhtm.textPosition + 1;
    }
    if (m_cursorPos > (int)m_text.size()) {
        __debugbreak();
    }
    this->UpdateCursor(false);
    return false;
}

bool TextEdit::OnSize(SizeEvent *ev)
{
    this->RecreateLayout();

    m_vsb->SetRect(RectF(ev->width - 6, 0, 6, ev->height));
    
    return false;
}

void TextEdit::RecreateResouce(ID2D1RenderTarget *target)
{
    SAFE_RELEASE(m_brush);
    auto textColor = StyleManager::Instance()->GetColor(StyleManager::clrTextNormal);
    HRESULT hr = target->CreateSolidColorBrush(textColor, &m_brush);
    LTK_ASSERT(SUCCEEDED(hr));
}

} // namespace ltk