#include "stdafx.h"
#include "Splitter.h"
#include "Splitter_p.h"

namespace ltk {

static const float GRIP_SIZE = 5.f;
static const float MIN_SIZE = 15.f;

Splitter::Splitter(Mode m) : Sprite(new SplitterPrivate(this, m))
{}

void Splitter::AddClient(Sprite *sp)
{
	LTK_PUBLIC_DQ(Splitter);
	SplitterItem item;
	item.client = sp;
	this->AddChild(sp);
	d->vecItems.push_back(item);
}

void Splitter::SetClientSize(UINT idx, float size)
{
	LTK_PUBLIC_DQ(Splitter);
	d->vecItems[idx].size = size;
}

void Splitter::DoLayout()
{
	LTK_PUBLIC_DQ(Splitter);
	if (d->vecItems.size() == 0) {
		return;
	}
	float x = 0;
	float y = 0;
	for (UINT i = 0; i < d->vecItems.size(); i++) {
		RectF rc;
		auto &item = d->vecItems[i];
		if (d->mode == Horizontal) {
			rc.X = x;
			rc.Y = y;
			if (i == d->vecItems.size() - 1) {
				rc.Width = this->GetWidth() - x;
			} else {
				rc.Width = item.size;
			}
			rc.Height = this->GetHeight();
			x += item.size + GRIP_SIZE;
		}
		else {
			rc.X = x;
			rc.Y = y;
			rc.Width = this->GetWidth();
			if (i == d->vecItems.size() - 1) {
				rc.Height = this->GetHeight() - y;
			} else {
				rc.Height = item.size;
			}
			y += item.size + GRIP_SIZE;
		}
		item.client->SetRect(rc);
	}
}

int SplitterPrivate::HitTest(float x, float y)
{
	float pos = 0.f;
	for (UINT i = 0; i < this->vecItems.size(); i++) {
		SplitterItem &item = this->vecItems[i];
		if (this->mode == Splitter::Horizontal) {
			if (x > pos + item.size && x < pos + item.size + GRIP_SIZE) {
				return (int)i;
			}
		}
		else {
			if (y > pos + item.size && y < pos + item.size + GRIP_SIZE) {
				return (int)i;
			}
		}
		pos += item.size + GRIP_SIZE;
	}
	return -1;
}

bool Splitter::OnLBtnDown(MouseEvent *ev)
{
	LTK_PUBLIC_DQ(Splitter);
	d->dragIdx = d->HitTest(ev->x, ev->y);
	if (d->dragIdx >= 0) {
		this->SetCapture();
		d->bCapture = true;
		d->dragPos = ev->x - d->PosFromIdx(d->dragIdx);
	}
	return false;
}

bool Splitter::OnLBtnUp(MouseEvent *ev)
{
	LTK_PUBLIC_DQ(Splitter);
	if (d->bCapture) {
		d->dragPos = 0.f;
		d->dragIdx = -1;
		d->bCapture = false;
		this->ReleaseCapture();
	}
	return false;
}

bool Splitter::OnSize(SizeEvent *ev)
{
	this->DoLayout();
	return false;
}

float SplitterPrivate::PosFromIdx(UINT idx)
{
	float sum = 0.f;
	for (UINT i = 0; i <= idx; i ++) {
		sum += this->vecItems[i].size;
		sum += GRIP_SIZE;
	}
	sum -= GRIP_SIZE;
	return sum;
}

bool Splitter::OnMouseMove(MouseEvent *ev)
{
	LTK_PUBLIC_DQ(Splitter);
	if (!d->bCapture) {
		auto idx = d->HitTest(ev->x, ev->y);
		if (idx >= 0) {
			LTK_LOG("HIT: %d", idx);
			if (d->mode == Horizontal) {
				::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			}
			else {
				::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			}
		}
	}
	else {
		for (int i = 0; i < d->dragIdx; i++) {
			ev->x -= d->vecItems[i].size;
		}
		d->vecItems[d->dragIdx].size = max(MIN_SIZE, ev->x - d->dragPos);
		float sum = d->PosFromIdx(d->vecItems.size() - 1);
		for (int i = d->vecItems.size() - 1; i >= 0; i--) {
			auto &item = d->vecItems[i];
			item.size -= sum - this->GetWidth();
			item.size = max(MIN_SIZE, item.size);
			sum = d->PosFromIdx(d->vecItems.size() - 1);
		}
		DoLayout();
	}
	return false;
}

} // namespace ltk
