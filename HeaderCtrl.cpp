//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

const float HeaderCtrl::DummyButtonWidth = 10.f;

HeaderCtrl::HeaderCtrl()
{
    this->EnableClipChildren(true);

    auto btn = new HeaderButton(this);
    btn->Background = "header_btn_bg";
    ColumnData data;
    data.button = btn;
    m_vecColumns.push_back(data);
    Sprite::AddChild(btn);
}

HeaderCtrl::~HeaderCtrl()
{
    this->DeleteEvent.Invoke();
}

void HeaderCtrl::GetColumnWidth(std::vector<float> &vecColumns)
{
    vecColumns.clear();
    for (UINT i = 0; i < m_vecColumns.size() - 1; i ++) {
        vecColumns.push_back(m_vecColumns[i].width);
    }
}

void HeaderCtrl::AddColumn(LPCWSTR name, float size)
{
    auto btn = new HeaderButton(this);
    btn->Background = "header_btn_bg";
    btn->SetText(name);
    ColumnData data;
    data.name = name;
    data.width = size;
    data.button = btn;
    m_vecColumns.insert(m_vecColumns.end() - 1, data);
    Sprite::AddChild(btn);
}

void HeaderCtrl::SetHScroll(float pos)
{
    if (m_hscroll != pos) {
        m_hscroll = pos;
        this->DoLayout();
    }
}

bool HeaderCtrl::OnSize(SizeEvent *ev)
{
    this->DoLayout();
    return true;
}

void HeaderCtrl::OnColumnResizeBegin(HeaderButton *btn, PointF pt)
{
    m_dragPoint = pt;
    m_draggingButton = btn;
    int i = 0;
    for (; i < (int)m_vecColumns.size(); i ++) {
        if (m_vecColumns[i].button == btn) {
            break;
        }
    }
    i--;
    if (i >= 0) {
        m_resizingCol = i;
    }
    this->SetCapture();
}

bool HeaderCtrl::OnMouseMove(MouseEvent *ev)
{
    if (m_resizingCol >= 0) {
        auto x = ev->x;
        x += m_hscroll;
        for (int i = 0; i < m_resizingCol; i ++) {
            x -= m_vecColumns[i].width;
        }
        x -= m_dragPoint.X;
        x = max(30.0f, x);
        m_vecColumns[m_resizingCol].width = x;
        this->DoLayout();
        this->ResizingEvent.Invoke();
    }
    return false;
}

bool HeaderCtrl::OnLBtnUp(MouseEvent *ev)
{
    m_resizingCol = -1;
    this->ReleaseCapture();
	this->ResizeEndEvent.Invoke();
    return false;
}

void HeaderCtrl::DoLayout()
{
    auto rc = this->GetClientRect();
    float sum = 0.0f;
    for (UINT i = 0; i < m_vecColumns.size() - 1; i++) {
        sum += m_vecColumns[i].width;
    }
    float last_column_w = rc.Width - sum + DummyButtonWidth;
    last_column_w = max(DummyButtonWidth, last_column_w);
    m_vecColumns[m_vecColumns.size() - 1].width = last_column_w;
    //LTK_LOG("last_column_w %f", last_column_w);

    float x = -m_hscroll;
    for (UINT i = 0; i < m_vecColumns.size(); i++) {
        m_vecColumns[i].button->SetRect(
            RectF(x, 0.0f, m_vecColumns[i].width, rc.Height));
        x += m_vecColumns[i].width;
    }
}

float HeaderCtrl::GetTotalWidth()
{
	float sum = 0.f;
	for (UINT i = 0; i < m_vecColumns.size() - 1; i++) {
		sum += m_vecColumns[i].width;
	}
	sum += DummyButtonWidth;
	return sum;
}

//////////////////////////////////////////////////////////////////////////
// HeaderButton
//////////////////////////////////////////////////////////////////////////

HeaderButton::HeaderButton(HeaderCtrl *parent) : m_parent(parent)
{}

bool HeaderButton::OnMouseMove(MouseEvent *ev)
{
    if (ev->x < 5.0f) {
        ::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
    } else {
        Button::OnMouseMove(ev);
    }
    return false;
}

bool HeaderButton::OnLBtnDown(MouseEvent *ev)
{
    if (ev->x < 5.0f) {
        m_parent->OnColumnResizeBegin(this, PointF(ev->x, ev->y));
    } else {
        Button::OnLBtnDown(ev);
    }
    return true;
}

} // namespace ltk