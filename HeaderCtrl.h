//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

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

class LTK_API HeaderCtrl : public Sprite
{
public:
    HeaderCtrl();
    virtual ~HeaderCtrl();

    void GetColumnWidth(std::vector<float> &vecColumns);
    void AddColumn(LPCWSTR name, float size);
    void SetHScroll(float pos);
    void DoLayout();
	float GetTotalWidth(); // including the tail dummy button

    void OnColumnResizeBegin(HeaderButton *btn, PointF pt);

    Delegate<void()> ResizingEvent;
	Delegate<void()> ResizeEndEvent;
	Delegate<void()> DeleteEvent;

    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;
	
private:
    std::vector<ColumnData> m_vecColumns;
    PointF m_dragPoint;
    HeaderButton *m_draggingButton = nullptr;
    int m_resizingCol = -1;
    float m_hscroll = 0.0f;
	static const float DummyButtonWidth;
};

class HeaderButton : public Button
{
public:
    explicit HeaderButton(HeaderCtrl *parent);
    virtual ~HeaderButton() {}

    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;

private:
    HeaderCtrl *m_parent;
};

}