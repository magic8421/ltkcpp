//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListView.h"
#include "ListView_p.h"
#include "Animation.h"
#include "StyleManager.h"
#include "ScrollBar.h"
#include "HeaderCtrl.h"
#include "Window.h"
#include "ScopeGuard.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

const float ItemHeight = 25.0f;

ListView::ListView() : Sprite(new ListViewPrivate(this))
{
	d_func()->Init();
}

ListView::ListView(ListViewPrivate *d)
{
	d->Init();
}

ListViewPrivate::ListViewPrivate(ListView *q) :
	SpritePrivate(q),
	TextColor("item_text_clr"),
	HoverColor("item_hover_clr"),
	SelectedColor("item_selected_clr"),
	SelectedTextColor("item_selected_text_clr"),
	TextFormat("item_text_fmt"),
	hoverColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	selectedColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	selectedTextColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	textColor(D2D1::ColorF(D2D1::ColorF::Cyan))
{
}

void ListViewPrivate::Init()
{
	auto d = this;
	auto q = q_func();
	d->vsb = new ScrollBar(ScrollBar::Vertical);
	d->vsb->AttachValueChangedDelegate(MakeDelegate(d, &ListViewPrivate::HandleVScrollBar));
	q->AddChild(vsb);
	d->hsb = new ScrollBar(ScrollBar::Horizontal);
	d->hsb->AttachValueChangedDelegate(MakeDelegate(d, &ListViewPrivate::HandleHScrollBar));
	q->AddChild(d->hsb);
}

ListViewPrivate::~ListViewPrivate()
{
	auto d = this;
	d->RemoveHeaderDelegates();
}

void ListView::OnThemeChanged()
{
	LTK_D(ListView);

	auto sm = StyleManager::Instance();
	d->textColor = sm->GetColor(d->TextColor);
	d->hoverColor = sm->GetColor(d->HoverColor);
	d->selectedColor = sm->GetColor(d->SelectedColor);
	d->selectedTextColor = sm->GetColor(d->SelectedTextColor);

	d->textFormat = sm->GetTextFormat(d->TextFormat);
}

bool ListView::OnPaint(PaintEvent *ev)
{
	LTK_D(ListView);
	ID2D1RenderTarget *target = ev->target;

    RectF rcSprite = this->GetRect();
	d->scroll.UpdateScroll(this->GetTotalHeight() - rcSprite.Height);
	if (!d->scroll.IsRunning()) {
        this->EndAnimation();
    }
	d->vsb->SetPosition(d->scroll.GetScroll());

    D2D1_RECT_F rcClip;
    rcClip.left = 0;
    rcClip.right = rcSprite.Width;
    rcClip.top = 0;
    rcClip.bottom = rcSprite.Height;
    target->PushAxisAlignedClip(rcClip, D2D1_ANTIALIAS_MODE_ALIASED);
	TranslateTransform(target, -d->hscroll, 0.0f);
    auto guard = LtkScopeGuard([&]() {
		TranslateTransform(target, d->hscroll, 0.0f);
        target->PopAxisAlignedClip();
    });

	auto brush = GetWindow()->GetStockBrush();
    rcClip.left = 0;
    D2D1_RECT_F rcItem;
	int i = (int)(d->scroll.GetScroll() / ItemHeight);
    i = max(i, 0);
	rcItem.top = -d->scroll.GetScroll() + i * ItemHeight;
    int display_max = (int)(rcSprite.Height / ItemHeight + i + 1.5f);
    //LTK_LOG("I: %d display_max: %d", i, display_max);
	for (; i < (int)d->vecData.size() && i < display_max; i++) {
		rcItem.left = d->hscroll;
		rcItem.right = rcSprite.Width + d->hscroll;
        rcItem.bottom = rcItem.top + ItemHeight;
		if (d->selectedItem == i) {
			brush->SetColor(d->selectedColor);
            target->FillRectangle(rcItem, brush);
        }
		else if (d->hoverItem == i) {
			brush->SetColor(d->hoverColor);
            target->FillRectangle(rcItem, brush);
        }
		auto &line = d->vecData.at(i);
        auto text = line.cells.at(0).data();
        auto len = line.cells.at(0).length();
        rcItem.left = 0.0f;
		if (d->selectedItem == i) {
			brush->SetColor(d->selectedTextColor);
        } else {
			brush->SetColor(d->textColor);
        }
        //if (d->vecColumns.size() > 0) {
		rcItem.right = d->vecColumns[0];
        //}
		target->DrawText(text, len, d->textFormat, rcItem, brush);

		if (d->vecColumns.size() > 0) {
            float x = 0.0f;
			for (size_t j = 1; j < d->vecColumns.size(); j++) {
				x += d->vecColumns.at(j - 1);
				if (j < line.cells.size() && j < d->vecColumns.size()) {
                    text = line.cells.at(j).data();
                    len = line.cells.at(j).length();
                    rcItem.left = x;
					rcItem.right = x + d->vecColumns.at(j);
					target->DrawText(text, len, d->textFormat, rcItem, brush);
                }
            }
        }
        rcItem.top += ItemHeight;
    }

    return true;
}

void ListView::AddItem(LPCWSTR text)
{
	LTK_D(ListView);
    LineData data;
    data.cells.push_back(std::move(std::wstring(text)));
	d->vecData.push_back(std::move(data));
	d->vsb->SetContentSize(this->GetTotalHeight());
    this->Invalidate();
}

bool ListView::SetSubItemText(UINT row, UINT col, LPCWSTR text)
{
	LTK_D(ListView);
	if (row >= d->vecData.size()) {
        return false;
    }
	auto &row_data = d->vecData[row];
    while (row_data.cells.size() - 1 < col) {
        row_data.cells.push_back(std::wstring());
    }
    row_data.cells[col] = text;
    this->Invalidate();

    return true;
}

bool ListView::OnLBtnDown(MouseEvent *ev)
{
	LTK_D(ListView);

	d->scroll.Stop();
    this->EndAnimation();

	d->selectedItem = (int)((d->scroll.GetScroll() + ev->y) / ItemHeight);
    this->Invalidate();
    return true;
}

bool ListView::OnMouseMove(MouseEvent *ev)
{
	LTK_D(ListView);

    this->TrackMouseLeave();
	if (d->scroll.IsRunning()) {
        return true;
    }
	d->hoverItem = (int)((d->scroll.GetScroll() + ev->y) / ItemHeight);
    this->Invalidate();
    return true;
}

bool ListView::OnMouseLeave(MouseEvent *ev)
{
	LTK_D(ListView);

	d->hoverItem = -1;
    this->Invalidate();
    return true;
}

float ListView::GetTotalHeight()
{
	LTK_D(ListView);

	return d->vecData.size() * ItemHeight;
}

int ListView::GetSelectedItem()
{
	LTK_D(ListView);

	return d->selectedItem;
}

bool ListView::OnMouseWheel(MouseEvent *ev)
{
	LTK_D(ListView);

	//LTK_LOG("wheel delta: %.2f", ev->delta);
	// TODO lag when using a high precision trackpad.
	if (fabs(ev->delta) < 1.f) {
		d->accumulateVScroll += ev->delta;
		//LTK_LOG("accumulateVScroll %0.3f", d->accumulateVScroll);
		if (fabs(d->accumulateVScroll) > 1.0f) {
			d->accumulateVScroll = 0.f;
			d->scroll.BeginScroll(ev->delta);
			this->BeginAnimation();
		}
	}
	else {
		d->scroll.BeginScroll(ev->delta);
		this->BeginAnimation();
	}
	return true;
}

void ListView::OnRecreateResouce(ID2D1RenderTarget *target)
{
}

void ListViewPrivate::HandleVScrollBar(float pos)
{
	auto d = this;
	auto q = q_func();
    d->scroll.SetScroll(pos);
    q->Invalidate();
}

void ListViewPrivate::HandleHScrollBar(float pos)
{
	auto d = this;
	auto q = q_func();
    d->hscroll = pos;
    if (d->header) {
        d->header->SetHScroll(d->hscroll);
    }
    q->Invalidate();
}

bool ListView::OnSize(SizeEvent *ev)
{
	LTK_D(ListView);

	this->HandleResizeEnd();

    RectF rc = this->GetClientRect();
    const float sb_size = 6.0f; // TODO load from StyleManager
	d->vsb->SetRect(RectF(rc.Width - sb_size, 0, sb_size, rc.Height));
	d->hsb->SetRect(RectF(0, rc.Height - sb_size, rc.Width, sb_size + 1));
    return true;
}

void ListView::RemoveItem(int row)
{
	LTK_D(ListView);

	if (row < 0 || row >= (int)d->vecData.size()) {
        return;
    }
	d->vecData.erase(d->vecData.begin() + (size_t)row);
    this->Invalidate();
}

LPCWSTR ListView::GetItemText(int row)
{
	LTK_D(ListView);

	if (row < 0 || row >= (int)d->vecData.size()) {
        return NULL;
    }
	return d->vecData[row].cells.at(0).c_str();
}

void ListView::ScrollToBottom()
{
	LTK_D(ListView);

    RectF rcSprite = this->GetRect();
	d->scroll.SetScroll(this->GetTotalHeight() - rcSprite.Height);
    this->Invalidate();
}

// TODO rename to SetColumnWidth
void ListView::SetColumns(std::vector<float> &columns)
{
	LTK_D(ListView);

	d->vecColumns.swap(columns);
	d->hsb->SetContentSize(d->header->GetTotalWidth());
}

void ListView::UpdateColumnWidth()
{
	LTK_D(ListView);

	LTK_ASSERT(d->header);
    std::vector<float> cols;
	d->header->GetColumnWidth(cols);
    this->SetColumns(cols);
}


void ListViewPrivate::OnHeaderDelete()
{
	this->header = nullptr;
}

void ListViewPrivate::RemoveHeaderDelegates()
{
	auto d = this;
	LTK_Q(ListView);

	if (d->header) {
		LTK_LOG("the ListView already has a HeadCtrl");
		d->header->RemoveResizingDelegate(MakeDelegate(
			q, &ListView::UpdateColumnWidth));
		d->header->RemoveResizeEndDelegate(MakeDelegate(
			q, &ListView::HandleResizeEnd));
		d->header->RemoveDeleteDelegate(MakeDelegate(
			d, &ListViewPrivate::OnHeaderDelete));
	}
}

void ListView::SetHeaderCtrl(HeaderCtrl *head)
{
	LTK_D(ListView);

	d->RemoveHeaderDelegates();
	d->header = head;

	d->header->AttachResizingDelegate(MakeDelegate(
		this, &ListView::UpdateColumnWidth));
	d->header->AttachResizeEndDelegate(MakeDelegate(
		this, &ListView::HandleResizeEnd));
	d->header->AttachDeleteDelegate(MakeDelegate(
		d, &ListViewPrivate::OnHeaderDelete));
	this->UpdateColumnWidth();
}

void ListView::HandleResizeEnd()
{
	LTK_D(ListView);

	float max_hscroll = d->header->GetTotalWidth() - this->GetWidth();
	max_hscroll = max(0.f, max_hscroll);
	float pos = min(d->hscroll, max_hscroll);
	d->hsb->SetPosition(pos);
	// d->vsb->SetContentSize(d->header->GetTotalWidth()); TODO
	d->HandleHScrollBar(pos);
}

} // namespace ltk
