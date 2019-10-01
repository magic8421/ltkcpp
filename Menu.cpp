//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Menu.h"
#include "Menu_p.h"
#include "Button.h"
#include "Window.h"
#include "TimerManager.h"

namespace ltk {

const static float ITEM_HEIGHT = 30.f;
const static float MENU_WIDTH = 300.f;
const static float ICON_WIDTH = 16.f;
const static float PADDING = 5.f;
const float AniDelta = 1.f / 300.f;

PopupMenu::PopupMenu() : Sprite(new PopupMenuPrivate(this))
{
}

PopupMenu::PopupMenu(PopupMenuPrivate *d) : Sprite(d)
{
}

PopupMenu::~PopupMenu()
{

}

PopupMenuPrivate::~PopupMenuPrivate()
{
	//ltk::KillTimer(this->hoverTimer);

	for (auto item : this->vecItems) {
		delete item->sub_menu;
		delete item;
	}
}

PopupMenuPrivate::PopupMenuPrivate(PopupMenu *q) :
	SpritePrivate(q),
	TextColor("item_text_clr"),
	TextFormat("popup_menu_fmt"),
	HoverColor("menu_hover_clr"),
	Background("popup_menu_bg"),
	textColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	hoverColor(D2D1::ColorF(D2D1::ColorF::Cyan))
{
}

void PopupMenu::AddItem(LPCWSTR text)
{
	LTK_D(PopupMenu);
	auto item = new MenuItem;
	item->text = text;
	d->vecItems.push_back(item);
}

UINT PopupMenu::GetChildCount()
{
	LTK_D(PopupMenu);
	return d->vecItems.size();
}

void PopupMenu::AttachItemClickedDelegate(UINT idx, const Delegate0<> &cb)
{
	LTK_D(PopupMenu);
	d->vecItems[idx]->clickedDelegate += cb;
}

void PopupMenu::SetWidth(float w)
{
	LTK_D(PopupMenu);
	d->width = w;
}

float PopupMenu::GetWidth()
{
	LTK_D(PopupMenu);
	return d->width;
}

void PopupMenu::SetSubMenu(UINT idx, PopupMenu *popup)
{
	LTK_D(PopupMenu);
	LTK_ASSERT(d->vecItems[idx]->sub_menu == nullptr);
	popup->d_func()->parent = this;
	d->vecItems[idx]->sub_menu = popup;
}

void PopupMenuPrivate::SetMenuBar(MenuBar *b)
{
	this->menuBar = b;
}

void PopupMenu::Show(Window* wnd, const RectF& rc)
{
	LTK_D(PopupMenu);
	if (!wnd) {
		return;
	}
	auto root = wnd->GetRootSprite();
	root->AddChild(this);
	this->SetRect(rc);
	wnd->SetFocusSprite(this);
	d->trackingIdx = -1;
	
	d->state = PopupMenuPrivate::sSlideIn;
	d->aniProgress = 0.f;
	d->lastTick = TickCount();
	BeginAnimation();
}

void PopupMenuPrivate::Hide()
{
	LTK_Q(PopupMenu);
	this->state = PopupMenuPrivate::sHide;
	this->bHiding = true;
	if (q->GetParent()) {
		q->GetParent()->RemoveChild(q);
		// Invalidate(); // because GetWindow() will return null, this does not work.
	}
	if (this->parent) {
		this->parent->SetFocus();
	}
	if (this->menuBar) {
		this->menuBar->d_func()->OnMenuHide();
	}
	this->bHiding = false;
}

void PopupMenuPrivate::HideAll()
{
	LTK_Q(PopupMenu);
	this->state = PopupMenuPrivate::sHide;
	this->bHiding = true;
	if (q->GetParent()) {
		q->GetParent()->RemoveChild(q);
		// Invalidate(); // because GetWindow() will return null, this does not work.
	}
	if (this->parent) {
		this->parent->d_func()->HideAll();
	}
	if (this->menuBar) {
		this->menuBar->d_func()->OnMenuHide();
	}
	this->bHiding = false;
}

void PopupMenu::OnThemeChanged()
{
	LTK_D(PopupMenu);
	auto sm = StyleManager::Instance();
	d->format = sm->GetTextFormat(d->TextFormat);
	d->textColor = sm->GetColor(d->TextColor);
	d->hoverColor = sm->GetColor(d->HoverColor);
	d->background = sm->GetBackground(d->Background);

	for (auto item : d->vecItems) {
		if (item->sub_menu) {
			item->sub_menu->OnThemeChanged();
		}
	}
}

void PopupMenu::OnParentChanged(Sprite* old, Sprite* new_)
{
	LTK_D(PopupMenu);
	d->hoverIdx = -1;
}

bool PopupMenu::OnPaint(PaintEvent *ev)
{
	LTK_D(PopupMenu);
	auto rcbg = this->GetClientRect();
	rcbg.Inflate(7, 7);

	float slide_h = 0.f;
	if (d->state == PopupMenuPrivate::sSlideIn) {
		d->aniProgress += (TickCount() - d->lastTick) * AniDelta;
		d->aniProgress = min(1.0f, d->aniProgress);
		slide_h = -this->GetHeight() + this->GetHeight() * d->aniProgress;
		ev->target->PushAxisAlignedClip(ltk::D2D1RectF(rcbg),
			D2D1_ANTIALIAS_MODE_ALIASED);
		ltk::TranslateTransform(ev->target, 0.f, slide_h);
	}

	d->background->Draw(GetWindow(), ev->target,
		rcbg, 
		AbstractBackground::Normal, 1.f);
	float y = 0;
	auto brush = GetWindow()->GetStockBrush();
	if (d->hoverIdx >= 0) {
		brush->SetColor(d->hoverColor);
		ev->target->FillRectangle(ltk::D2D1RectF(RectF(0.f, d->hoverIdx * ITEM_HEIGHT,
			this->GetWidth(), ITEM_HEIGHT)), brush);
	}
	brush->SetColor(d->textColor);
	for (auto item : d->vecItems) {
		ev->target->DrawText(item->text.c_str(), item->text.size(), d->format,
			D2D1::RectF(PADDING + ICON_WIDTH, y, this->GetWidth(), y + ITEM_HEIGHT),
			brush);
		y += ITEM_HEIGHT;
	}

	if (d->state == PopupMenuPrivate::sSlideIn) {
		ltk::TranslateTransform(ev->target, 0.f, -slide_h);
		ev->target->PopAxisAlignedClip();
		d->lastTick = TickCount();
		//LTK_LOG("d->aniProgress: %.2f", d->aniProgress);
		if (d->aniProgress >= 1.f) {
			EndAnimation();
			d->state = PopupMenuPrivate::sShow;
		}
	}
	return false;
}

bool PopupMenu::OnKillFocus(FocusEvent* ev)
{
	LTK_D(PopupMenu);
	Invalidate();
	if (d->trackingIdx < 0 && !d->bHiding) {
		d->Hide();
		auto menu = d->parent;
		while (menu) {
			menu->d_func()->Hide();
			menu = menu->d_func()->parent;
		}
	}
	return false;
}

bool PopupMenu::OnLBtnDown(MouseEvent* ev)
{
	LTK_D(PopupMenu);
	auto wnd = GetWindow();
	wnd->DisableFocusChange();
	int hit = (int)(ev->y / ITEM_HEIGHT);
	if (hit < 0 || hit >= (int)d->vecItems.size()) {
		return false;
	}
	if (hit == d->trackingIdx) {
		return false;
	}
	auto item = d->vecItems[hit];
	if (!item->sub_menu) {
		item->clickedDelegate();
	}
	int tracking = d->trackingIdx;
	PopupMenu* menu = this;
	while (tracking >= 0) {
		menu = menu->d_func()->vecItems[tracking]->sub_menu;
		tracking = menu->d_func()->trackingIdx;
	}
	menu->d_func()->HideAll();
	::InvalidateRect(wnd->Handle(), NULL, FALSE);
	return true;
}

void PopupMenuPrivate::TrackPopupMenu(UINT idx)
{
	LTK_Q(PopupMenu);
	auto menu = this->vecItems[idx]->sub_menu;
	if (menu) {
		this->trackingIdx = idx;
		auto arc = q->GetAbsRect();
		menu->Show(q->GetWindow(), RectF(
			arc.X + q->GetWidth(), arc.Y + idx * ITEM_HEIGHT,
			menu->GetWidth(), menu->GetChildCount() * ITEM_HEIGHT));
		q->Invalidate();
	}
}

bool PopupMenu::OnMouseMove(MouseEvent* ev)
{
	LTK_D(PopupMenu);
	TrackMouseLeave();
	int hover = (int)(ev->y / ITEM_HEIGHT);
	if (hover != d->hoverIdx) {
		d->hoverIdx = hover;
		Invalidate();
		//d->hoverTimer = ltk::SetOnceTimer(0, d->hoverTimer, [this, hover, d]() {
			//LTK_LOG("hover %d", hover);
			if (d->trackingIdx >= 0 && d->trackingIdx != hover) {
				auto sub_menu = d->vecItems[d->trackingIdx]->sub_menu;
				sub_menu->d_func()->Hide();
				d->trackingIdx = -1;
				d->TrackPopupMenu(hover);
				this->Invalidate();
			}
			else if (d->trackingIdx < 0){
				d->TrackPopupMenu(hover);
			}
			d->hoverTimer = 0;
		//});
		if (d->parent) {
			//ltk::KillTimer(d->parent->d_func()->hoverTimer);
			d->parent->d_func()->hoverTimer = 0;
		}
	}
	return true;
}

bool PopupMenu::OnMouseLeave(MouseEvent* ev)
{
	LTK_D(PopupMenu);
	d->hoverIdx = -1;
	Invalidate();
	return false;
}



//////////////////////////////////////////////////////////////////////////

MenuBar::MenuBar() : Sprite(new MenuBarPrivate(this))
{
}

MenuBar::MenuBar(MenuBarPrivate *d) : Sprite(d)
{
}

MenuBar::~MenuBar()
{
}

MenuBarPrivate::~MenuBarPrivate()
{
	for (auto &item : this->vecMenuItems) {
		delete item.sub_menu;
	}
}

void MenuBar::AddItem(LPCWSTR text)
{
	LTK_D(MenuBar);
	Button *btn = new Button;
	btn->SetText(text);
	btn->SetBackground("menu_bar_btn_bg");
	//btn->ObjectName = "menu_btn";
	this->AddChild(btn);
	MenuButtonParam param;
	param.button = btn;
	d->vecMenuItems.push_back(param);
	UINT idx = d->vecMenuItems.size() - 1;
	btn->AttachClickedDelegate(MakeDelegate(d, &MenuBarPrivate::OnMenuBtnClicked));
	btn->AttachMouseEventDelegate(MakeDelegate(d, &MenuBarPrivate::OnButtonMouseEvent));
}

void MenuBar::SetPopupMenu(UINT idx, PopupMenu *menu)
{
	LTK_D(MenuBar);
	LTK_ASSERT(d->vecMenuItems[idx].sub_menu == nullptr);
	menu->OnThemeChanged();
	d->vecMenuItems[idx].sub_menu = menu;
	menu->d_func()->SetMenuBar(this);
}

void MenuBarPrivate::OnMenuBtnClicked()
{
	LTK_Q(MenuBar);
	Button* btn = Object::GetDelegateInvoker()->As<Button>();
	int idx = this->FindMenuButtonIdx(btn);
	LTK_ASSERT(idx < (int)this->vecMenuItems.size());
	auto menu = this->vecMenuItems[idx].sub_menu;
	if (!menu) {
		return;
	}
	auto arc = this->vecMenuItems[idx].button->GetAbsRect();

	menu->Show(q->GetWindow(), RectF(arc.X, arc.Y + arc.Height,
		this->vecMenuItems[idx].sub_menu->GetWidth(), menu->GetChildCount() * ITEM_HEIGHT));
	this->trackingIdx = idx;
}

int MenuBarPrivate::FindMenuButtonIdx(Button *btn)
{
	int idx = -1;
	for (size_t i = 0; i < this->vecMenuItems.size(); i++) {
		if (this->vecMenuItems[i].button == btn) {
			idx = i;
			break;
		}
	}
	return idx;
}

void MenuBarPrivate::OnButtonMouseEvent(MouseEvent* ev, bool& bHandled)
{
	Button* btn = Object::GetDelegateInvoker()->As<Button>();

	if (ev->id == eMouseMove && this->trackingIdx >= 0) {
		int idx = this->FindMenuButtonIdx(btn);
		if (this->trackingIdx != (int)idx) {
			this->OnMenuBtnClicked();
		}
	}
}

void MenuBar::DoLayout()
{
	LTK_D(MenuBar);
	float x = 0;
	for (UINT i = 0; i < d->vecMenuItems.size(); i++) {
		auto param = d->vecMenuItems[i];
		SizeF size = param.button->GetPreferredSize();
		RectF rc(x, 0.f, size.Width, this->GetHeight());
		//LTK_LOG("menu_btn: %.1f,%.1f,%.1f,%.1f", rc.X, rc.Y, rc.Width, rc.Height);
		param.button->SetRect(rc);
		x += size.Width;
	}
}

void MenuBarPrivate::OnMenuHide()
{
	this->trackingIdx = -1;
}

bool MenuBar::OnSize(SizeEvent *ev)
{
	this->DoLayout();
	return false;
}

void MenuBar::OnThemeChanged()
{
	LTK_D(MenuBar);
	for (auto &item : d->vecMenuItems) {
		if (item.sub_menu) {
			item.sub_menu->OnThemeChanged();
		}
	}
}

} // namespace

