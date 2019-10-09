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
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

ListView::ListView() :
	TextColor("item_text_clr"),
	HoverColor("item_hover_clr"),
	SelectedColor("item_selected_clr"),
	SelectedTextColor("item_selected_text_clr"),
	TextFormat("item_text_fmt"),
	m_hoverColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	m_selectedColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	m_selectedTextColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	m_textColor(D2D1::ColorF(D2D1::ColorF::Cyan))
{
	m_vsb = new ScrollBar(ltk::Vertical);
	m_vsb->ValueChangedDelegate += MakeDelegate(
		this, &ListView::HandleVScrollBar);
    this->AddChild(m_vsb);
	m_hsb = new ScrollBar(ltk::Horizontal);
	m_hsb->ValueChangedDelegate += MakeDelegate(
		this, &ListView::HandleHScrollBar);
    this->AddChild(m_hsb);

	m_header = new HeaderCtrl;
	this->AddChild(m_header);
	m_header->ResizingDelegate += MakeDelegate(this, &ListView::UpdateColumnWidth);
	m_header->ResizeEndDelegate += MakeDelegate(this, &ListView::HandleResizeEnd);
}

ListView::~ListView()
{
}

void ListView::OnThemeChanged()
{
	auto sm = StyleManager::Instance();
	m_textColor = sm->GetColor(TextColor);
	m_hoverColor = sm->GetColor(HoverColor);
	m_selectedColor = sm->GetColor(SelectedColor);
	m_selectedTextColor = sm->GetColor(SelectedTextColor);

	m_textFormat = sm->GetTextFormat(TextFormat);
}

bool ListView::OnEvent(Event *ev)
{
	if (ev->id > eMouseFirst && ev->id < eMouseLast) {
		MouseEvent *mev = static_cast<MouseEvent *>(ev);
		mev->y -= m_headerHeight;
	}
	return Sprite::OnEvent(ev);
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
	TranslateTransform(target, -m_hscroll, m_headerHeight);
    auto guard = LtkScopeGuard([&]() {
		TranslateTransform(target, m_hscroll, -m_headerHeight);
        target->PopAxisAlignedClip();
    });

	auto brush = GetWindow()->GetStockBrush();
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
			brush->SetColor(m_selectedColor);
            target->FillRectangle(rcItem, brush);
        }
        else if (m_hoverItem == i) {
			brush->SetColor(m_hoverColor);
            target->FillRectangle(rcItem, brush);
        }
        auto &line = m_vecData.at(i);
        auto text = line.cells.at(0).data();
        auto len = line.cells.at(0).length();
        rcItem.left = 0.0f;
        if (m_selectedItem == i) {
			brush->SetColor(m_selectedTextColor);
        } else {
			brush->SetColor(m_textColor);
        }
        if (m_vecColumns.size() > 0) {
            rcItem.right = m_vecColumns[0];
        }
        target->DrawText(text, len, m_textFormat, rcItem, brush);

        if (m_vecColumns.size() > 0) {
            float x = 0.0f;
            for (size_t j = 1; j < m_vecColumns.size(); j++) {
                x += m_vecColumns.at(j - 1);
                if (j < line.cells.size() && j < m_vecColumns.size()) {
                    text = line.cells.at(j).data();
                    len = line.cells.at(j).length();
                    rcItem.left = x;
                    rcItem.right = x + m_vecColumns.at(j);
                    target->DrawText(text, len, m_textFormat, rcItem, brush);
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

int ListView::GetSelectedItem()
{
	return m_selectedItem;
}

bool ListView::OnMouseWheel(MouseEvent *ev)
{
    m_scroll.BeginScroll(ev->delta);
    this->BeginAnimation();
    return true;
}

void ListView::RecreateResouce(ID2D1RenderTarget *target)
{
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
	this->HandleResizeEnd();

    RectF rc = this->GetClientRect();
    const float sb_size = 6.0f; // TODO load from StyleManager
    m_vsb->SetRect(RectF(
		rc.Width - sb_size, m_headerHeight,
		sb_size, rc.Height - m_headerHeight));
    m_hsb->SetRect(RectF(0, rc.Height - sb_size, rc.Width, sb_size + 2));

	m_header->SetRect(RectF(0, 0, rc.Width, m_headerHeight));
    return true;
}

void ListView::RemoveItem(UINT row)
{
	if (row >= m_vecData.size()) {
		__debugbreak();
	}
    m_vecData.erase(m_vecData.begin() + (size_t)row);
    this->Invalidate();
}

LPCWSTR ListView::GetItemText(UINT row)
{
	if (row >= m_vecData.size()) {
		__debugbreak();
	}
    return m_vecData[row].cells.at(0).c_str();
}

UINT ListView::GetItemCount()
{
	return m_vecData.size();
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
    m_hsb->SetContentSize(m_header->GetTotalWidth());
}

void ListView::UpdateColumnWidth()
{
    LTK_ASSERT(m_header);
    std::vector<float> cols;
    m_header->GetColumnWidth(cols);
    this->SetColumns(cols);
}

HeaderCtrl * ListView::GetHeaderCtrl()
{
	return m_header;
}

void ListView::SetHeaderCtrl(HeaderCtrl *head)
{
	LTK_ASSERT(false);
}

void ListView::HandleResizeEnd()
{
	if (!m_header) {
		return;
	}
	float max_hscroll = m_header->GetTotalWidth() - this->GetWidth();
	max_hscroll = max(0.f, max_hscroll);
	float pos = min(m_hscroll, max_hscroll);
	m_hsb->SetPosition(pos);
	// m_vsb->SetContentSize(m_header->GetTotalWidth()); TODO
	HandleHScrollBar(pos);
}

} // namespace ltk
