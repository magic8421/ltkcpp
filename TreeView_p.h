#pragma once
#include "TreeView.h"
#include "Sprite_p.h"

namespace ltk {

struct TreeViewColors
{
	D2D1_COLOR_F TextColor;
	D2D1_COLOR_F HoverColor;
	D2D1_COLOR_F SelectedColor;
	D2D1_COLOR_F SelectedTextColor;
};

class TreeNodePrivate : public ObjectPrivate
{
public:
	TreeNodePrivate(TreeNode *q);
	virtual ~TreeNodePrivate();

private:
	bool bExpand = true;
	bool bMouseHover = false;
	bool bChecked = false;

	TreeView *treeView = nullptr;
	RectF rect;
	RectF rcExpandBtn;
	std::wstring text;

private:
	LTK_DECLARE_PUBLIC(TreeNode);
};

class TreeViewPrivate : public SpritePrivate
{
public:
	TreeViewPrivate(TreeView *q);
	virtual ~TreeViewPrivate() {}

	void Init();
	void TraverseTree(TreeNode *node, int depth,
		const std::function<void(TreeNode *, int)> &cb);

private:
	ScrollBar *vsb = nullptr;
	TreeNode root;

	float itemHeight = 30.f;
	float indent = 10.f;

	IDWriteTextFormat *format = nullptr;

	ScrollAnimation scrollAni;
	float maxHeight = 0.0f;

	TreeViewColors colors;

	ImmutableString TextColor;
	ImmutableString HoverColor;
	ImmutableString SelectedColor;
	ImmutableString SelectedTextColor;
	ImmutableString TextFormat;

	LTK_DECLARE_PUBLIC(TreeView);
};

} // namespace ltk


