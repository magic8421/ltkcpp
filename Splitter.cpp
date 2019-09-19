#include "stdafx.h"
#include "Splitter.h"

namespace ltk {

static const float GRIP_SIZE = 5.f;

void Splitter::AddClient(Sprite *sp)
{
	SplitterItem item;
	item.client = sp;
	this->AddChild(sp);
	m_vecItems.push_back(item);
}

void Splitter::SetClientSize(UINT idx, float size)
{
	m_vecItems[idx].size = size;
}

void Splitter::DoLayout()
{
	if (m_vecItems.size() == 0) {
		return;
	}
	float x = 0;
	float y = 0;
	for (UINT i = 0; i < m_vecItems.size(); i ++) {
		RectF rc;
		auto &item = m_vecItems[i];
		if (m_mode == Horizontal) {
			rc.X = x;
			rc.Y = y;
			if (i == m_vecItems.size() - 1) {
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
			if (i == m_vecItems.size() - 1) {
				rc.Height = this->GetHeight() - y;
			} else {
				rc.Height = item.size;
			}
			y += item.size + GRIP_SIZE;
		}
		item.client->SetRect(rc);
	}
}

int Splitter::HitTest(float x, float y)
{
	float pos = 0.f;
	for (UINT i = 0; i < m_vecItems.size(); i++) {
		SplitterItem &item = m_vecItems[i];
		if (m_mode == Horizontal) {
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
	m_dragIdx = HitTest(ev->x, ev->y);
	if (m_dragIdx >= 0) {
		this->SetCapture();
		m_bCapture = true;
		m_dragPos = ev->x;
	}
	return false;
}

bool Splitter::OnLBtnUp(MouseEvent *ev)
{
	if (m_bCapture) {
		m_dragPos = 0.f;
		m_dragIdx = -1;
		m_bCapture = false;
		this->ReleaseCapture();
	}
	return false;
}

bool Splitter::OnSize(SizeEvent *ev)
{
	this->DoLayout();
	return false;
}

bool Splitter::OnMouseMove(MouseEvent *ev)
{
	if (!m_bCapture) {
		auto idx = HitTest(ev->x, ev->y);
		if (idx >= 0) {
			LTK_LOG("HIT: %d", idx);
			if (m_mode == Horizontal) {
				::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			}
			else {
				::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			}
		}
	}
	else {
		auto dragDelta = ev->x - m_dragPos;
		//m_vecItems[m_dragIdx].size += dragDelta;
	}
	return false;
}

} // namespace ltk
