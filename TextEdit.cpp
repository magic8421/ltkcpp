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
    SAFE_RELEASE(m_brushSelectedText);
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

    if (m_selection > 0) {
        UINT32 begin = m_cursorPos;
        UINT32 end = m_selection;
        if (begin > end) {
            std::swap(begin, end);
        }
        UINT32 len = 0;
        std::vector<DWRITE_HIT_TEST_METRICS> vecMetrics;
        m_layout->HitTestTextRange(begin, end - begin, 0.0f, 0.0f,
            NULL, 0, &len);
        LTK_ASSERT(SUCCEEDED(hr));
        LTK_LOG("HitTestTextRange: %d metrics", len);
        vecMetrics.resize(len);
        m_layout->HitTestTextRange(begin, end - begin, m_padding, 0.0f,
            &vecMetrics[0], len, &len);
        LTK_ASSERT(SUCCEEDED(hr));
        m_brush->SetColor(StyleManager::ColorFromString("#0000ff"));
        for (auto &m : vecMetrics) {
            D2D1_RECT_F rc;
            rc.left = m.left;
            rc.right = rc.left + m.width;
            rc.top = m.top - m_scrollAni.GetScroll();
            rc.bottom = rc.top + m.height;
            target->FillRectangle(rc, m_brush);
        }
    }

    D2D_POINT_2F pt;
    pt.x = m_padding;
    pt.y = -m_scrollAni.GetScroll();
    m_brush->SetColor(StyleManager::ColorFromString("#000000"));
    target->DrawTextLayout(pt, m_layout, m_brush);

    return false;
}

void TextEdit::DeleteSelected()
{
    if (m_selection < 0) {
        return;
    }
    UINT begin = m_cursorPos;
    UINT end = m_selection;
    if (begin > end) {
        std::swap(begin, end);
    }
    m_text.erase(m_text.begin() + begin, m_text.begin() + end);
    m_cursorPos = min(m_cursorPos, m_selection); // TODO FIXME
    m_cursorPos = max(0, m_cursorPos);
    m_cursorPos = min(m_cursorPos, m_text.size() - 1);
    m_selection = -1;
}

bool TextEdit::OnChar(KeyEvent *ev)
{
    wchar_t ch = (wchar_t)ev->keyCode;
    if (ch == VK_BACK) {
        if (m_selection >= 0) {
            this->DeleteSelected();
        } else if (m_cursorPos > 0) {
            m_text.erase(m_text.begin() + m_cursorPos - 1);
            m_cursorPos--;
        } else {
            return true;
        }
    } else {
        if (m_selection >= 0) {
            this->DeleteSelected();
        }
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
        if (m_selection >= 0) {
            this->DeleteSelected();
        } else if (m_cursorPos < (int)m_text.size()) {
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
    if (m_selection >= 0) {
        this->DeleteSelected();
    }
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
        rc.Width - m_padding * 2, 0.0f, &m_layout);
    LTK_ASSERT(SUCCEEDED(hr));

    if (m_selection > 0) {
        UINT32 begin = m_cursorPos;
        UINT32 end = m_selection;
        if (begin > end) {
            std::swap(begin, end);
        }
        m_layout->SetDrawingEffect(
            m_brushSelectedText, DWRITE_TEXT_RANGE{ begin, end - begin });
    }

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
    RectF rcCursor(x + m_padding, y, 1, dhtm.height);
    this->SetCaretPos(rcCursor);
}

bool TextEdit::OnMouseWheel(MouseEvent *ev)
{
    m_scrollAni.BeginScroll(ev->delta);
    this->HideCaret();
    this->BeginAnimation();
    return true;
}

int TextEdit::HitTest(float x, float y)
{
    int pos = -1;
    y += m_scrollAni.GetScroll();
    BOOL isTrailingHit = FALSE;
    BOOL isInside = FALSE;
    DWRITE_HIT_TEST_METRICS dhtm = { 0 };
    HRESULT hr = m_layout->HitTestPoint(
        x, y, &isTrailingHit, &isInside, &dhtm);
    LTK_ASSERT(SUCCEEDED(hr));
    //LTK_LOG("isTrailingHit: %d, isInside: %d", isTrailingHit, isInside);
    m_isInside = isInside ? true : false;

    if (!isTrailingHit) {
        pos = dhtm.textPosition;
    } else {
        pos = dhtm.textPosition + 1;
    }
    if (pos > (int)m_text.size()) {
        __debugbreak();
    }
    return pos;
}

bool TextEdit::OnLBtnDown(MouseEvent *ev)
{
    ev->x -= m_padding;
    m_scrollAni.Stop();
    this->EndAnimation();
    m_cursorPos = HitTest(ev->x, ev->y);

    if (m_selection >= 0) {
        m_selection = -1;
        m_prevSelection = -1;
        this->RecreateLayout();
    }
    this->UpdateCursor(false);
    this->SetCapture();
    m_bCapture = true;
    return false;
}

bool TextEdit::OnLBtnUp(MouseEvent *ev)
{
    m_bCapture = false;
    this->ReleaseCapture();
    return false;
}

bool TextEdit::OnMouseMove(MouseEvent *ev)
{
    if (!m_bCapture) {
        return false;
    }
    m_selection = HitTest(ev->x, ev->y);
    //LTK_LOG("m_selection:%d", m_selection);
    if (m_selection != m_prevSelection) {
        m_prevSelection = m_selection;
        this->RecreateLayout();
    }
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

    SAFE_RELEASE(m_brushSelectedText);
    textColor = StyleManager::ColorFromString("#ffffff");
    hr = target->CreateSolidColorBrush(textColor, &m_brushSelectedText);
    LTK_ASSERT(SUCCEEDED(hr));
}

} // namespace ltk