//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Animation.h"
#include "Delegate.h"
#include "ImmutableString.h"


namespace ltk {

class ScrollBar;
class HeaderCtrl;

class ListView : public Sprite
{
public:
    ListView();
    virtual ~ListView();

    void HandleVScrollBar(float pos);
    void HandleHScrollBar(float pos);

    void AddItem(LPCWSTR text);
    bool SetSubItemText(UINT row, UINT col, LPCWSTR text);
    float GetTotalHeight();
    int GetSelectedItem() { return m_selectedItem; }
    void RemoveItem(int row);
    LPCWSTR GetItemText(int row);
    void ScrollToBottom();
    void SetColumns(std::vector<float> &columns);
    void UpdateColumnWidth();
    //void ShowHeader(bool show);
    void SetHeaderCtrl(HeaderCtrl *head);
	void HandleResizeEnd();

	ImmutableString TextColor;
	ImmutableString HoverColor;
	ImmutableString SelectedColor;
	ImmutableString SelectedTextColor;
	ImmutableString TextFormat;

	virtual bool OnPaint(PaintEvent *ev) override;
	virtual bool OnLBtnDown(MouseEvent *ev) override;
	virtual bool OnMouseWheel(MouseEvent *ev) override;
	virtual bool OnMouseMove(MouseEvent *ev) override;
	virtual bool OnMouseLeave(MouseEvent *ev) override;
	virtual bool OnSize(SizeEvent *ev) override;
	virtual void OnRecreateResouce(ID2D1RenderTarget *target) override;
	virtual void OnThemeChanged() override;

private:
    struct LineData
    {
        std::vector<std::wstring> cells;
    };
    enum State
    {
        stStop, stScrollUp, stScrollDown
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

    Cookie m_columnResizeTracker = 0;
    Cookie m_headerDeletedTracker = 0;

protected:
	D2D1_COLOR_F m_textColor;
	D2D1_COLOR_F m_hoverColor;
	D2D1_COLOR_F m_selectedColor;
	D2D1_COLOR_F m_selectedTextColor;
};

} // namespace ltk
