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
    RefPtr<HeaderButton> button;
};

class LTK_CPP_API HeaderCtrl : public Widget
{
public:
    RTTI_DECLARATIONS(HeaderCtrl, Widget);
    HeaderCtrl();
    virtual ~HeaderCtrl();

    void GetColumnWidth(std::vector<float> &vecColumns);
    void GetColumnOrder(std::vector<int> &vecOrder);

    void AddColumn(LPCWSTR name, float size);
    void SetHScroll(float pos);
    void DoLayout();
	float GetTotalWidth(); // including the tail dummy button

    void OnColumnResizeBegin(HeaderButton *btn, const PointF& pt);
    void OnColumnReorderBegin(HeaderButton *btn, const PointF& pt);

	MulticastDelegate0 ResizingDelegate;
	MulticastDelegate0 ResizeEndDelegate;
    MulticastDelegate1<const std::vector<int> &> ColumnOrderChanged;

    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;
	
private:
    std::vector<ColumnData> m_vecColumns;
    PointF m_dragPoint;
    HeaderButton *m_draggingButton = nullptr; // TODO ¸Ä³ÉRefPtr
    int m_resizingCol = -1;
    int m_reorderCol = -1;
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