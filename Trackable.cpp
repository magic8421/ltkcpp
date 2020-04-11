#include "stdafx.h"
#include "Trackable.h"
#include "ltk.h"

namespace ltk {

static Trackable* g_track_head = nullptr;
static Trackable* g_track_tail = nullptr;

Trackable::Trackable()
{
	if (!g_track_head) {
		g_track_head = this;
		g_track_tail = this;
		m_next = nullptr;
		m_prev = nullptr;
	}
	else {
		g_track_head->m_prev = this;
		m_next = g_track_head;
		g_track_head = this;
		m_prev = nullptr;
	}
}

Trackable::~Trackable()
{
	if (m_prev) {
		m_prev->m_next = m_next;
	}
	else {
		g_track_head = m_next;
	}
	if (m_next) {
		m_next->m_prev = m_prev;
	}
	else {
		g_track_tail = m_prev;
	}
}

void Trackable::SetAllocInfo(LPCSTR info)
{
	m_alloc_info = ltk::InternString(info);
}

void Trackable::DumpLeaks()
{
	auto node = g_track_head;
	char buf[512];
	while (node) {
		::StringCbPrintfA(buf, sizeof(buf), 
			"LEAK: [%s] %s \r\n", node->TypeNameInstance(), node->m_alloc_info);
		::OutputDebugStringA(buf);
		node = node->m_next;
	}
}

} // namespace ltk

