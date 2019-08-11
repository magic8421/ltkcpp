#pragma once

#include "Sprite.h"
#include "Button.h"

namespace ltk {

class HeaderButton;

struct ComlunData {
    std::wstring name;
    float width = 100.0f;
    int order = -1;
    HeaderButton *button = nullptr;
};

class HeaderCtrl : public Sprite
{
public:
    RTTI_DECLARATIONS(HeaderCtrl, Sprite);
    HeaderCtrl();
    virtual ~HeaderCtrl();

    void GetColumnWidth(std::vector<float> &vecColumns);
    void AddColumn(LPCWSTR name, float size);
    void DoLayout();

    void OnColumnResizeBegin(HeaderButton *btn, PointF pt);

    Delegate<void()> ColumnResizeEvent;

    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;

private:
    std::vector<ComlunData> m_vecColumns;
    PointF m_dragPoint;
    HeaderButton *m_draggingButton = nullptr;
    int m_resizingCol = -1;
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