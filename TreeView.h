//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Animation.h"

namespace ltk {

class ScrollBar;
class TreeView;
class AbstractBackground;

struct TreeViewColors
{
	D2D1_COLOR_F TextColor;
	D2D1_COLOR_F HoverColor;
	D2D1_COLOR_F SelectedColor;
	D2D1_COLOR_F SelectedTextColor;
};

class LTK_CPP_API TreeNode : public Object
{
public:
	RTTI_DECLARATIONS(TreeNode, Object);

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

    virtual void OnPaint(ID2D1RenderTarget *target, float scroll);
    virtual void OnLBtnDown(PointF pt, float scroll);

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

class LTK_CPP_API TreeView : public Sprite
{
public:
	RTTI_DECLARATIONS(TreeView, Sprite);

    TreeView();
    virtual ~TreeView();

    void DoLayout();

    void TraverseTree(TreeNode *node, int depth,
        const std::function<void(TreeNode *, int)> &cb);

    ID2D1SolidColorBrush *GetBrush();
    IDWriteTextFormat *GetTextFormat();
	TreeViewColors *GetColorScheme();
	AbstractBackground *GetExpandIcon();
	AbstractBackground *GetCollapseIcon();
	float GetItemHeight();

    TreeNode *GetRootNode();
	void SetSelectedNode(TreeNode *);
	TreeNode *GetSelectedNode();

    void SetTextColor(LPCSTR style);
    void SetHoverColor(LPCSTR style);
    void SetSelectedColor(LPCSTR style);
    void SetSelectedTextColor(LPCSTR style);
    void SetTextFormat(LPCSTR style);

protected:
    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnMouseWheel(MouseEvent *ev) override;
    virtual void RecreateResouce(ID2D1RenderTarget *target) override;
	virtual void OnThemeChanged() override;

private:
    ScrollBar *m_vsb = nullptr;
    TreeNode m_root;
	TreeNode *m_selected = nullptr;

    static const float m_itemHeight;
    static const float m_indent;

    IDWriteTextFormat *m_format = nullptr;
	AbstractBackground *m_expandBg = nullptr;
	AbstractBackground *m_collapseBg = nullptr;

    ScrollAnimation m_scrollAni;
    float m_maxHeight = 0.0f;

	TreeViewColors m_colors;

    LPCSTR m_szTextColor = nullptr;
    LPCSTR m_szHoverColor = nullptr;
    LPCSTR m_szSelectedColor = nullptr;
    LPCSTR m_szSelectedTextColor = nullptr;
    LPCSTR m_szTextFormat = nullptr;

    DISALLOW_COPY_AND_ASSIGN(TreeView)
};

} // namespace ltk
