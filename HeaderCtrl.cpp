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
    auto btn = new Button;
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

bool HeaderCtrl::OnEvent(Event *ev)
{
    switch (ev->id) {
    case eDelegateMouseEvent:

        break;
    }
    return __super::OnEvent(ev);
}

} // namespace ltk