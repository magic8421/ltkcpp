#pragma once
#include "LtkInterface.h"
#include "LtkWidget.h"

class LtkTreeNode : public LtkObject
{
public:
	void Create()
	{
		if (!m_bWeak) LtkDelete(m_hltk);
		m_hltk = LtkTreeNode_New();
	}
};

class LtkTreeView : public LtkWidget
{
public:
	void Create()
	{
		if (!m_bWeak) LtkDelete(m_hltk);
		m_hltk = LtkTreeView_New();
	}
	LtkTreeNode* GetRootNode() 
	{
		auto node = new LtkTreeNode;
		node->Attach(LtkTreeView_GetRootNode(m_hltk));
		return node;
	}
	void AddChild(LtkTreeNode* node)
	{
		LtkTreeNode_AddChild(m_hltk, node->GetHandle());
	}
	void SetText(LPCSTR text)
	{
		LtkTreeNode_SetText(m_hltk, text);
	}
};

