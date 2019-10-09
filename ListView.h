//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Animation.h"
#include "HeaderCtrl.h"


namespace ltk {

class ScrollBar;

class LTK_CPP_API ListView : public Sprite
{
public:
    ListView();
    virtual ~ListView();

    void AddItem(LPCWSTR text);
    bool SetSubItemText(UINT row, UINT col, LPCWSTR text);
    float GetTotalHeight();
    int GetSelectedItem();
    void RemoveItem(UINT row);
	LPCWSTR GetItemText(UINT row);
	UINT GetItemCount();
    void ScrollToBottom();
    void UpdateColumnWidth();
	HeaderCtrl *GetHeaderCtrl();
    void SetHeaderCtrl(HeaderCtrl *head);
	void HandleResizeEnd();

	ImmutableString TextColor;
	ImmutableString HoverColor;
	ImmutableString SelectedColor;
	ImmutableString SelectedTextColor;
	ImmutableString TextFormat;

	virtual bool OnEvent(Event *ev) override;
	virtual bool OnPaint(PaintEvent *ev) override;
	virtual bool OnLBtnDown(MouseEvent *ev) override;
	virtual bool OnMouseWheel(MouseEvent *ev) override;
	virtual bool OnMouseMove(MouseEvent *ev) override;
	virtual bool OnMouseLeave(MouseEvent *ev) override;
	virtual bool OnSize(SizeEvent *ev) override;
	virtual void RecreateResouce(ID2D1RenderTarget *target) override;
	virtual void OnThemeChanged() override;

	void HandleVScrollBar(float pos);
	void HandleHScrollBar(float pos);

protected:
	void SetColumns(std::vector<float> &columns);

private:
    struct LineData
    {
        std::vector<std::wstring> cells;
    };
    std::vector<LineData> m_vecData;
    ID2D1SolidColorBrush *m_brush = nullptr;
    IDWriteTextFormat *m_textFormat = nullptr;
    ScrollAnimation m_scroll;
    ScrollBar *m_vsb = nullptr;
    ScrollBar *m_hsb = nullptr;
    HeaderCtrl *m_header = nullptr;
    int m_hoverItem = -1;
    int m_selectedItem = -1;
    std::vector<float> m_vecColumns;
    float m_hscroll = 0.0f;

    const float ItemHeight = 25.0f;
	float m_headerHeight = 30.f;

protected:
	D2D1_COLOR_F m_textColor;
	D2D1_COLOR_F m_hoverColor;
	D2D1_COLOR_F m_selectedColor;
	D2D1_COLOR_F m_selectedTextColor;
};

} // namespace ltk
