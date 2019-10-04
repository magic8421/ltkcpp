#include "stdafx.h"
#include "Splitter.h"

namespace ltk {

static const float GRIP_SIZE = 5.f;
static const float MIN_SIZE = 15.f;

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
		m_dragDelta = ev->x - PosFromIdx(m_dragIdx);
		LTK_LOG("m_dragDelta %.2f m_dragIdx %d", m_dragDelta, m_dragIdx);
	}
	return false;
}

bool Splitter::OnLBtnUp(MouseEvent *ev)
{
	if (m_bCapture) {
		m_dragDelta = 0.f;
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

float Splitter::PosFromIdx(UINT idx)
{
	float sum = 0.f;
	for (UINT i = 0; i <= idx; i ++) {
		sum += m_vecItems[i].size;
		sum += GRIP_SIZE;
	}
	sum -= GRIP_SIZE;
	return sum;
}

bool Splitter::OnMouseMove(MouseEvent *ev)
{
	if (!m_bCapture) {
		auto idx = HitTest(ev->x, ev->y);
		if (idx >= 0) {
			//LTK_LOG("HIT: %d", idx);
			if (m_mode == Horizontal) {
				::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			} else {
				::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			}
		}
	}
	else {
		if (m_mode == Horizontal) {
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		} else {
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		}
		float cx = ev->x;
		for (int i = 0; i < m_dragIdx; i++) {
			cx -= m_vecItems[i].size;
			cx -= GRIP_SIZE;
		}
		//cx += GRIP_SIZE;
		//m_vecItems[m_dragIdx + 1].size = 
		float new_size = cx - m_dragDelta;
		float size_delta = new_size - m_vecItems[m_dragIdx].size;
		UINT idx = m_dragIdx;
		if (size_delta > 0.f) {
			while (idx < m_vecItems.size() - 1 && size_delta > 0.f) {
				float old_size = m_vecItems[idx + 1].size;
				m_vecItems[idx + 1].size -= size_delta;
				m_vecItems[idx + 1].size = max(MIN_SIZE, m_vecItems[idx + 1].size);
				size_delta -= old_size - m_vecItems[idx + 1].size;
				idx++;
			}
		} else {
			if (idx < m_vecItems.size() - 1) {
				m_vecItems[idx + 1].size -= size_delta;
			}
		}
		m_vecItems[m_dragIdx].size = max(MIN_SIZE, cx - m_dragDelta);

		float move_left = cx - m_dragDelta - MIN_SIZE;
		if (move_left < 0.f) {
			UINT idx = m_dragIdx;
			move_left = -move_left;
			while (idx > 0 && move_left > 0.f) {
				float old_size = m_vecItems[idx - 1].size;
				m_vecItems[idx - 1].size -= move_left;
				m_vecItems[idx - 1].size = max(MIN_SIZE, m_vecItems[idx - 1].size);
				move_left -= old_size - m_vecItems[idx - 1].size;
				idx--;
			}
		}

		//float sum = PosFromIdx(m_vecItems.size() - 1);
		//for (int i = m_vecItems.size() - 1; i >= 0; i --) {
		//	auto &item = m_vecItems[i];
		//	item.size -= sum - this->GetWidth();
		//	item.size = max(MIN_SIZE, item.size);
		//	sum = PosFromIdx(m_vecItems.size() - 1);
		//}
		DoLayout();
	}
	return false;
}

} // namespace ltk
