//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Animation.h"
#include "Common.h"
#include "ImmutableString.h"

namespace ltk {

class ScrollBar;
class TreeView;
class TreeViewPrivate;
class TreeNodePrivate;
struct TreeViewColors;

class TreeNode : public Object
{
public:
    TreeNode();
    virtual ~TreeNode();

    void SetTreeView(TreeView *tree);

    void AddChild(TreeNode *);
    void InsertChild(TreeNode *);

    void SetRect(const RectF &rc);
    RectF GetRect();

    LPCWSTR GetText();
    void SetText(LPCWSTR);

    bool IsExpand();

    virtual void OnPaint(ID2D1RenderTarget *target, float scroll);
    virtual void OnLBtnDown(PointF pt);

protected:
	TreeNode(TreeNodePrivate *d);
private:
	LTK_DECLARE_PRIVATE(TreeNode);
};

class TreeView : public Sprite
{
public:
    TreeView();
    virtual ~TreeView();

    void DoLayout();

    ID2D1SolidColorBrush *GetBrush();
    IDWriteTextFormat *GetTextFormat();
	TreeViewColors *GetColorScheme();
    TreeNode *GetRootNode();

protected:
    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnMouseWheel(MouseEvent *ev) override;
    virtual void OnRecreateResouce(ID2D1RenderTarget *target) override;
	virtual void OnThemeChanged() override;

private:
	LTK_DECLARE_PRIVATE(TreeView);

protected:
	TreeView(TreeViewPrivate *d);
};

} // namespace ltk
