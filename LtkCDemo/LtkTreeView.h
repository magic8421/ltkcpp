#pragma once
#include "LtkInterface.h"
#include "LtkWidget.h"

class LtkTreeNode : public LtkObject
{
public:
	LtkTreeNode() : LtkObject(LtkTreeNode_New())
	{}
	explicit LtkTreeNode(HLTK hltk) : LtkObject(hltk) {}
};

class LtkTreeView : public LtkWidget
{
public:
	LtkTreeView() : LtkWidget(LtkTreeView_New())
	{}
	LtkTreeNode* GetRootNode() 
	{
		auto node = new LtkTreeNode(LtkTreeView_GetRootNode(m_hltk));
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

