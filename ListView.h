#pragma once

#include "Sprite.h"
#include "Animation.h"
#include "ScrollBar.h"
#include "HeaderCtrl.h"

namespace ltk {

class ListView : public Sprite
{
public:
    ListView();
    virtual ~ListView();

    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnMouseWheel(MouseEvent *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnMouseLeave(MouseEvent *ev) override;
    virtual bool OnSize(SizeEvent *ev) override;
    virtual void RecreateResouce(ID2D1RenderTarget *target) override;

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
};

} // namespace ltk
