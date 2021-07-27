#include "stdafx.h"
#include "Splitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static const float GRIP_SIZE = 5.f;
static const float MIN_SIZE = 15.f;

void Splitter::Resize(UINT n)
{
	if (n < m_vecItems.size()) {
		// TODO test this branch.
		for (UINT i = m_vecItems.size(); i > 0 && i > n; i --) {
			auto &item = m_vecItems[i - 1];
			if (item.client) {
				LTK_LOG("Splitter::Resize() failed: non-NULL clients.");
				return;
			}
			m_vecItems.pop_back();
		}
	}
	else  {
		m_vecItems.resize(n);
	}
}

void Splitter::SetClientAt(UINT idx, Ptr<Widget> sp)
{
	auto &item = m_vecItems.at(idx);
	auto old_sp = item.client;
	if (old_sp) {
		GetParent()->RemoveChild(Ptr(old_sp));
	}
	Widget::AddChild(sp);
	item.client = sp;
}

size_t Splitter::AddClient(Ptr<Widget> sp)
{
	SplitterItem item;
	item.client = sp;
	this->AddChild(sp);
	m_vecItems.push_back(item);
	return m_vecItems.size() - 1;
}

void Splitter::SetClientSize(UINT idx, float size)
{
	m_vecItems[idx].size = size;
}

float Splitter::GetTotolSize()
{
	float sum_size = 0.f;
	for (const auto &item : m_vecItems) {
		sum_size += item.size;
		sum_size += GRIP_SIZE;
	}
	sum_size -= GRIP_SIZE;
	return sum_size;
}

void Splitter::DoLayout()
{
	if (m_vecItems.size() == 0) {
		return;
	}
	RectF rcWidget = this->GetClientRect();

	float size_limit = 10.f;
	if (m_mode == Horizontal) {
		size_limit = rcWidget.Width;
	} else {
		size_limit = rcWidget.Height;
	}
	if (m_vecItems.size() >= 2) {
		int i = m_vecItems.size() - 1;
		while (size_limit < GetTotolSize() && i >= 0) {
			m_vecItems[i].size -= GetTotolSize() - size_limit;
			m_vecItems[i].size = max(MIN_SIZE, m_vecItems[i].size);
			if (size_limit >= GetTotolSize()) {
				break;
			}
			i--;
		}
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
		if (item.client) {
			item.client->SetRect(rc);
		}
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
	if (m_dragIdx >= 0 && m_dragIdx < (int)m_vecItems.size() - 1) {
		this->SetCapture();
		m_bCapture = true;
		if (m_mode == Horizontal) {
			m_dragDelta = ev->x - PosFromIdx(m_dragIdx);
		} else {
			m_dragDelta = ev->y - PosFromIdx(m_dragIdx);
		}
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
		if (idx >= 0 && idx < (int)m_vecItems.size() - 1) {
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
		float input_pos;
		float total_size;
		if (m_mode == Horizontal) {
			input_pos = ev->x;
			total_size = this->GetWidth();
		} else {
			input_pos = ev->y;
			total_size = this->GetHeight();
		}
		if (input_pos - m_dragDelta > total_size -
			(m_vecItems.size() - m_dragIdx - 1) * (MIN_SIZE + GRIP_SIZE))
		{
			input_pos = total_size -
				(m_vecItems.size() - m_dragIdx - 1) * (MIN_SIZE + GRIP_SIZE)
				+ m_dragDelta;
		}

		for (int i = 0; i < m_dragIdx; i++) {
			input_pos -= m_vecItems[i].size;
			input_pos -= GRIP_SIZE;
		}

		float new_size = input_pos - m_dragDelta;

		float size_delta = new_size - m_vecItems[m_dragIdx].size;
		UINT idx = m_dragIdx;
		if (size_delta > 0.0001f) {
			//LTK_LOG("size_delta %.2f", size_delta);
			while (idx < m_vecItems.size() - 1 && size_delta > 0.f) {
				float old_size = m_vecItems[idx + 1].size;
				m_vecItems[idx + 1].size -= size_delta;
				m_vecItems[idx + 1].size = max(MIN_SIZE, m_vecItems[idx + 1].size);
				size_delta -= old_size - m_vecItems[idx + 1].size;
				idx++;
			}
		} /*else if (size_delta < -0.0001f) {
			if (idx < m_vecItems.size() - 1) {
				LTK_LOG("size_delta %.2f", size_delta);
				m_vecItems[idx + 1].size -= size_delta;
			}
		}*/
		m_vecItems[m_dragIdx].size = max(MIN_SIZE, new_size);

		float move_left = new_size - MIN_SIZE;
		bool bCanMoveLeft = true;
		if (move_left < 0.f) {
			UINT idx2 = m_dragIdx;
			move_left = -move_left;
			while (idx2 > 0 && move_left > 0.f) {
				float old_size = m_vecItems[idx2 - 1].size;
				m_vecItems[idx2 - 1].size -= move_left;
				m_vecItems[idx2 - 1].size = max(MIN_SIZE, m_vecItems[idx2 - 1].size);
				move_left -= old_size - m_vecItems[idx2 - 1].size;
				idx2--;
			}
			if (move_left > 0.0001f) {
				bCanMoveLeft = false;
			}
		}
		if (size_delta < -0.0001f && bCanMoveLeft) {
			if (m_dragIdx < (int)m_vecItems.size() - 1) {
				//LTK_LOG("size_delta [2] %.2f", size_delta);
				m_vecItems[m_dragIdx + 1].size -= size_delta;
			}
		}
		DoLayout();
	}
	return false;
}

} // namespace ltk
