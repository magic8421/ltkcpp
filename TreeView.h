//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"

namespace ltk {

class ScrollBar;
class TreeView;

class TreeNode
{
public:
    TreeNode() {}
    virtual ~TreeNode();

    void SetTreeView(TreeView *tree);

    void AddChild(TreeNode *);
    void InsertChild(TreeNode *);
    UINT GetChildCount();
    TreeNode *GetNthChild(UINT i);

    void SetRect(const RectF &rc);
    RectF GetRect();

    LPCWSTR GetText();
    void SetText(LPCWSTR);

    bool IsExpand();

    virtual void OnPaint(ID2D1RenderTarget *target);
    virtual void OnLBtnDown(PointF pt);

private:
    bool m_bExpand = true;
    bool m_bMouseHover = false;
    bool m_bChecked = false;

    TreeView *m_treeView = nullptr;
    TreeNode *m_parent = nullptr;
    std::vector<TreeNode *> m_children;
    RectF m_rect;
    RectF m_rcExpandBtn;
    std::wstring m_text;

    static const float m_padding;
    static const float m_btn_size;

    DISALLOW_COPY_AND_ASSIGN(TreeNode)
};

class TreeView : public Sprite
{
public:
    TreeView();
    virtual ~TreeView();

    void DoLayout();

    void TraverseTree(TreeNode *node, int depth,
        const std::function<void(TreeNode *, int)> &cb);

    ID2D1SolidColorBrush *GetBrush();
    IDWriteTextFormat *GetTextFormat();
    TreeNode *GetRootNode();

protected:
    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual void RecreateResouce(ID2D1RenderTarget *target) override;



private:
    ScrollBar *m_vsb = nullptr;
    TreeNode m_root;

    static const float m_itemHeight;
    static const float m_indent;

    ID2D1SolidColorBrush *m_brush = nullptr;
    IDWriteTextFormat *m_format = nullptr;

    DISALLOW_COPY_AND_ASSIGN(TreeView)
};

} // namespace ltk
