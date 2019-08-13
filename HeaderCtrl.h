#pragma once

#include "Button.h"

namespace ltk {

class HeaderButton;

struct ColumnData {
    std::wstring name;
    float width = 100.0f;
    int order = -1;
    HeaderButton *button = nullptr;
};

class HeaderCtrl : public Sprite
{
public:
    HeaderCtrl();
    virtual ~HeaderCtrl();

    void GetColumnWidth(std::vector<float> &vecColumns);
    void AddColumn(LPCWSTR name, float size);
    void SetHScroll(float pos);
    void DoLayout();

    void OnColumnResizeBegin(HeaderButton *btn, PointF pt);

    Delegate<void()> ColumnResizeEvent;

    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;

private:
    std::vector<ColumnData> m_vecColumns;
    PointF m_dragPoint;
    HeaderButton *m_draggingButton = nullptr;
    int m_resizingCol = -1;
    float m_hscroll = 0.0f;
};

class HeaderButton : public Button
{
public:
    HeaderButton(HeaderCtrl *parent);
    virtual ~HeaderButton() {}

    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;

private:
    HeaderCtrl *m_parent;
};

}