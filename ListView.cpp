//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListView.h"
#include "Animation.h"
#include "StyleManager.h"
#include "ScrollBar.h"
#include "HeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

ListView::ListView()
{
    m_vsb = new ScrollBar(ScrollBar::Vertical);
    m_vsb->ValueChangedEvent.Attach([this](float pos) {
        this->HandleVScrollBar(pos);
    });
    this->AddChild(m_vsb);
    m_hsb = new ScrollBar(ScrollBar::Horizontal);
    m_hsb->ValueChangedEvent.Attach([this](float pos) {
        this->HandleHScrollBar(pos);
    });
    this->AddChild(m_hsb);

    m_style = StyleManager::Instance()->GetListViewStyle("default");
}

ListView::~ListView()
{
    if (m_header) {
        m_header->ColumnResizeEvent.Remove(m_columnResizeTracker);
        m_header->DeleteEvent.Remove(m_headerDeletedTracker);
    }
    RELEASE_AND_INVALIDATE(ID2D1SolidColorBrush, m_brush);
    RELEASE_AND_INVALIDATE(IDWriteTextFormat, m_textFormat);
}

bool ListView::OnPaint(PaintEvent *ev)
{
    ID2D1RenderTarget *target = ev->target;

    RectF rcSprite = this->GetRect();
    if (m_scroll.UpdateScroll(this->GetTotalHeight() - rcSprite.Height)) {
        this->EndAnimation();
    }
    m_vsb->SetPosition(m_scroll.GetScroll());

    D2D1_RECT_F rcClip;
    rcClip.left = 0;
    rcClip.right = rcSprite.Width;
    rcClip.top = 0;
    rcClip.bottom = rcSprite.Height;
    target->PushAxisAlignedClip(rcClip, D2D1_ANTIALIAS_MODE_ALIASED);
    TranslateTransform(target, -m_hscroll, 0.0f);
    auto guard = LtkScopeGuard([&]() {
        TranslateTransform(target, m_hscroll, 0.0f);
        target->PopAxisAlignedClip();
    });

    rcClip.left = 0;
    D2D1_RECT_F rcItem;
    int i = (int)(m_scroll.GetScroll() / ItemHeight);
    i = max(i, 0);
    rcItem.top = -m_scroll.GetScroll() + i * ItemHeight;
    int display_max = (int)(rcSprite.Height / ItemHeight + i + 1.5f);
    //LTK_LOG("I: %d display_max: %d", i, display_max);
    for (; i < (int)m_vecData.size() && i < display_max; i++) {
        rcItem.left = m_hscroll;
        rcItem.right = rcSprite.Width + m_hscroll;
        rcItem.bottom = rcItem.top + ItemHeight;
        if (m_selectedItem == i) {
            m_brush->SetColor(m_style->SelectedColor);
            //m_brush->SetColor(StyleManager::Instance()->GetColor(
            //    StyleManager::clrListBoxSelected));
            target->FillRectangle(rcItem, m_brush);
        }
        else if (m_hoverItem == i) {
            m_brush->SetColor(m_style->HoverColor);
            //m_brush->SetColor(StyleManager::Instance()->GetColor(
            //    StyleManager::clrListBoxHover));
            target->FillRectangle(rcItem, m_brush);
        }
        auto &line = m_vecData.at(i);
        auto text = line.cells.at(0).data();
        auto len = line.cells.at(0).length();
        rcItem.left = 0.0f;
        if (m_selectedItem == i) {
            m_brush->SetColor(m_style->SelectedTextColor);
        } else {
            m_brush->SetColor(m_style->TextColor);
        }
        //m_brush->SetColor(StyleManager::Instance()->GetColor(
        //    StyleManager::clrTextNormal));
        //if (m_vecColumns.size() > 0) {
            rcItem.right = m_vecColumns[0];
        //}
        target->DrawText(text, len, m_textFormat, rcItem, m_brush);

        if (m_vecColumns.size() > 0) {
            float x = 0.0f;
            for (size_t j = 1; j < m_vecColumns.size(); j++) {
                x += m_vecColumns.at(j - 1);
                if (j < line.cells.size() && j < m_vecColumns.size()) {
                    text = line.cells.at(j).data();
                    len = line.cells.at(j).length();
                    rcItem.left = x;
                    rcItem.right = x + m_vecColumns.at(j);
                    target->DrawText(text, len, m_textFormat, rcItem, m_brush);
                }
            }
        }
        rcItem.top += ItemHeight;
    }

    return true;
}

void ListView::AddItem(LPCWSTR text)
{
    LineData data;
    data.cells.push_back(std::move(std::wstring(text)));
    m_vecData.push_back(std::move(data));
    m_vsb->SetContentSize(this->GetTotalHeight());
    this->Invalidate();
}

bool ListView::SetSubItemText(UINT row, UINT col, LPCWSTR text)
{
    if (row >= m_vecData.size()) {
        return false;
    }
    auto &row_data = m_vecData[row];
    while (row_data.cells.size() - 1 < col) {
        row_data.cells.push_back(std::wstring());
    }
    row_data.cells[col] = text;
    this->Invalidate();

    return true;
}

bool ListView::OnLBtnDown(MouseEvent *ev)
{
    m_scroll.Stop();
    this->EndAnimation();

    m_selectedItem = (int)((m_scroll.GetScroll() + ev->y) / ItemHeight);
    this->Invalidate();
    return true;
}

bool ListView::OnMouseMove(MouseEvent *ev)
{
    this->TrackMouseLeave();
    if (m_scroll.IsRunning()) {
        return true;
    }
    m_hoverItem = (int)((m_scroll.GetScroll() + ev->y) / ItemHeight);
    this->Invalidate();
    return true;
}

bool ListView::OnMouseLeave(MouseEvent *ev)
{
    m_hoverItem = -1;
    this->Invalidate();
    return true;
}

float ListView::GetTotalHeight()
{
    return m_vecData.size() * ItemHeight;
}

bool ListView::OnMouseWheel(MouseEvent *ev)
{
    m_scroll.BeginScroll(ev->delta);
    this->BeginAnimation();
    return true;
}

void ListView::RecreateResouce(ID2D1RenderTarget *target)
{
    SAFE_RELEASE(m_brush);
    HRESULT hr = target->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black), &m_brush);
    LTK_ASSERT(SUCCEEDED(hr));
    SAFE_RELEASE(m_textFormat);
    hr = GetDWriteFactory()->CreateTextFormat(L"Î¢ÈíÑÅºÚ", NULL, DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"zh-cn",
        &m_textFormat);
    m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    m_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
    LTK_ASSERT(SUCCEEDED(hr));
}

void ListView::HandleVScrollBar(float pos)
{
    //LTK_LOG("HandleScrollBar %f", pos);
    m_scroll.SetScroll(pos);
    this->Invalidate();
}

void ListView::HandleHScrollBar(float pos)
{
    m_hscroll = pos;
    if (m_header) {
        m_header->SetHScroll(m_hscroll);
    }
    this->Invalidate();
}

bool ListView::OnSize(SizeEvent *ev)
{
    RectF rc = this->GetClientRect();
    const float sb_size = 6.0f; // TODO load from StyleManager
    m_vsb->SetRect(RectF(rc.Width - sb_size, 0, sb_size, rc.Height));
    m_hsb->SetRect(RectF(0, rc.Height - sb_size, rc.Width, sb_size + 2));
    return true;
}

void ListView::RemoveItem(int row)
{
    if (row < 0 || row >= (int)m_vecData.size()) {
        return;
    }
    m_vecData.erase(m_vecData.begin() + (size_t)row);
    this->Invalidate();
}

LPCWSTR ListView::GetItemText(int row)
{
    if (row < 0 || row >= (int)m_vecData.size()) {
        return NULL;
    }
    return m_vecData[row].cells.at(0).c_str();
}

void ListView::ScrollToBottom()
{
    RectF rcSprite = this->GetRect();
    m_scroll.SetScroll(this->GetTotalHeight() - rcSprite.Height);
    this->Invalidate();
}

void ListView::SetColumns(std::vector<float> &columns)
{
    m_vecColumns.swap(columns);
    float sum = 0.0f;
    for (float w : m_vecColumns) {
        sum += w;
    }
    sum += 10.0f;
    m_hsb->SetContentSize(sum);
}

void ListView::UpdateColumnWidth()
{
    LTK_ASSERT(m_header);
    std::vector<float> cols;
    m_header->GetColumnWidth(cols);
    this->SetColumns(cols);
}

void ListView::SetHeaderCtrl(HeaderCtrl *head)
{
    if (m_header) {
        LTK_LOG("the ListView already has a HeadCtrl");
        m_header->ColumnResizeEvent.Remove(m_columnResizeTracker);
    }
    m_header = head;
    m_columnResizeTracker = m_header->ColumnResizeEvent.Attach([this]() {
        this->UpdateColumnWidth();
    });
    m_headerDeletedTracker = m_header->DeleteEvent.Attach([this]() {
        m_header = nullptr;
    });
    this->UpdateColumnWidth();
}


} // namespace ltk
