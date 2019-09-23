//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Label.h"
#include "Label_p.h"
#include "ltk.h"
#include "StyleManager.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Label::Label() : Sprite(new LabelPrivate(this))
{}

Label::~Label()
{
}

LabelPrivate::LabelPrivate(Label *q) : SpritePrivate(q) ,
	TextColor("default_label_clr"),
	TextFormat("default_label_fmt")
{
}

bool Label::OnPaint(PaintEvent *ev)
{
	LTK_D(Label);
    if (d->text.length() == 0) {
        return true;
    }
    RectF rc = this->GetClientRect();

    auto brush = this->GetWindow()->GetStockBrush();
	brush->SetColor(d->color);
	ev->target->DrawText(d->text.c_str(), (UINT32)d->text.length(), d->textFormat,
        ltk::D2D1RectF(rc), brush);
    return true;
}

LPCWSTR Label::GetText()
{
	LTK_D(Label);
	return d->text.c_str();
}

void Label::SetText(LPCWSTR t)
{
	LTK_D(Label);
	d->text = t;
    this->Invalidate();
}

void Label::SetTextFormat(LPCSTR name)
{
	LTK_D(Label);
	d->TextFormat = name;
}

void Label::SetTextColor(LPCSTR name)
{
	LTK_D(Label);
	d->TextColor = name;
}

void Label::OnThemeChanged()
{
	LTK_D(Label);
	auto sm = StyleManager::Instance();
	d->textFormat = sm->GetTextFormat(d->TextFormat);
	d->color = sm->GetColor(d->TextColor);
}

} // namespace ltk
