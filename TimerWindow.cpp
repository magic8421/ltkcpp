#include "stdafx.h"
#include "TimerWindow.h"
#include "BoxLayout.h"
#include "Button.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

TimerWindow::TimerWindow()
{
	m_timer1.SetInterval(1000);
	m_timer1.TimeoutDelegate += MakeDelegate(this, &TimerWindow::OnTimer);

	ltk::BoxLayout *vbox1 = new ltk::BoxLayout(ltk::Vertical);
	this->SetClientSprite(vbox1);

	m_lableTime = new ltk::Label;
	m_lableTime->SetText(L"00:00:00");
	m_lableTime->TextFormat = "timer_label_fmt";
	vbox1->AddLayoutItem(m_lableTime, 100, 0);

	m_listView = new ltk::ListView;
	vbox1->AddLayoutItem(m_listView, 100, 1);

	ltk::BoxLayout *hbox1 = new ltk::BoxLayout(ltk::Horizontal);
	vbox1->AddLayoutItem(hbox1, 40, 0);
	vbox1->AddSpaceItem(10, 0);

	hbox1->AddSpaceItem(5, 0);
	ltk::Button *btnAdd = new ltk::Button;
	btnAdd->SetText(L"Ìí¼Ó");
	btnAdd->ClickedDelegate += MakeDelegate(this, &TimerWindow::OnAddBtnClicked);
	hbox1->AddLayoutItem(btnAdd, 100);
}

TimerWindow::~TimerWindow()
{
}

void TimerWindow::OnTimer()
{

}

void TimerWindow::OnAddBtnClicked()
{
	m_listView->AddItem(L"Ãû³Æ");
	m_listView->SetSubItemText(m_listView->GetItemCount() - 1, 1, L"00:00:00");
}

void TimerWindow::OnDestroy()
{
	::PostQuitMessage(0);
}

