#include "stdafx.h"
#include "HeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

HeaderCtrl::HeaderCtrl()
{
    this->SetClipChildren(true);
}

HeaderCtrl::~HeaderCtrl()
{
    for (auto& col : m_vecColumns) {
        col.button->SetDelegate(nullptr);
    }
}

void HeaderCtrl::GetColumnWidth(std::vector<float> &vecColumns)
{
    vecColumns.clear();
    for (UINT i = 0; i < m_vecColumns.size(); i ++) {
        vecColumns.push_back(m_vecColumns[i].width);
    }
}

void HeaderCtrl::AddColumn(LPCWSTR name, float size)
{
    auto btn = new HeaderButton(this);
    btn->SetBackgroundStyle("header_btn");
    btn->SetText(name);
    ComlunData data;
    data.name = name;
    data.width = size;
    data.button = btn;
    m_vecColumns.push_back(data);
    btn->SetDelegate(this);
    Sprite::AddChild(btn);
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
        for (int i = 0; i < m_resizingCol; i ++) {
            x -= m_vecColumns[i].width;
        }
        x -= m_dragPoint.X;
        m_vecColumns[m_resizingCol].width = x;
        this->DoLayout();
        this->ColumnResizeEvent.Invoke();
    }
    return false;
}

bool HeaderCtrl::OnLBtnUp(MouseEvent *ev)
{
    m_resizingCol = -1;
    this->ReleaseCapture();
    return false;
}

void HeaderCtrl::DoLayout()
{
    auto rc = this->GetClientRect();
    float x = 0.0f;
    for (UINT i = 0; i < m_vecColumns.size(); i++) {
        m_vecColumns[i].button->SetRect(
            RectF(x, 0.0f, m_vecColumns[i].width, rc.Height));
        x += m_vecColumns[i].width;
    }
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