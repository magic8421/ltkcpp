//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Animation.h"

namespace ltk {

class HeaderCtrl;
class ListViewPrivate;

class ListView : public Sprite
{
public:
    ListView();
	virtual ~ListView() {}

    void AddItem(LPCWSTR text);
    bool SetSubItemText(UINT row, UINT col, LPCWSTR text);
    float GetTotalHeight();
    int GetSelectedItem();
    void RemoveItem(int row);
    LPCWSTR GetItemText(int row);
    void ScrollToBottom();
    void SetColumns(std::vector<float> &columns);
    void UpdateColumnWidth();
    //void ShowHeader(bool show);
    void SetHeaderCtrl(HeaderCtrl *head);
	void HandleResizeEnd();

	virtual bool OnPaint(PaintEvent *ev) override;
	virtual bool OnLBtnDown(MouseEvent *ev) override;
	virtual bool OnMouseWheel(MouseEvent *ev) override;
	virtual bool OnMouseMove(MouseEvent *ev) override;
	virtual bool OnMouseLeave(MouseEvent *ev) override;
	virtual bool OnSize(SizeEvent *ev) override;
	virtual void OnRecreateResouce(ID2D1RenderTarget *target) override;
	virtual void OnThemeChanged() override;

protected:
	ListView(ListViewPrivate *d);
private:
	LTK_DECLARE_PRIVATE(ListView);
};

} // namespace ltk
