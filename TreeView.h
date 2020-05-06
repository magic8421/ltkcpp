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
    TreeNode() {}
    virtual ~TreeNode();

    void SetTreeView(TreeView *tree);

    void AddChild(TreeNode *);
    void InsertChild(TreeNode *);
    UINT GetChildCount();
    TreeNode *GetNthChild(UINT i);

    RectF GetRect();
    
    void SetDepth(int);

    LPCWSTR GetText();
    void SetText(LPCWSTR);

    void* GetUserData();
    void SetUserData(void*);

    bool IsExpand();
    virtual void OnPaint(ID2D1RenderTarget *target, float scroll, UINT idx);
    virtual void OnLBtnDown(PointF pt, float scroll, UINT idx);

    RectF GetRect(float scroll, UINT idx);
    RectF GetExpandButtonRect(const RectF &rc);

private:
    std::vector<TreeNode*> m_children;
    TreeView* m_treeView = nullptr;
    TreeNode* m_parent = nullptr;

    bool m_bExpand = true;
    bool m_bMouseHover = false;
    bool m_bChecked = false;

    int m_depth = 0;
    std::wstring m_text;
    void* m_userdata = nullptr;

    static const float m_padding;
    static const float m_btn_size;

    DISALLOW_COPY_AND_ASSIGN(TreeNode)
};

class LTK_CPP_API TreeView : public Widget
{
public:
	RTTI_DECLARATIONS(TreeView, Widget);

    TreeView();
    virtual ~TreeView();

    void DoLayout();

    ID2D1SolidColorBrush *GetBrush();
    IDWriteTextFormat *GetTextFormat();
	TreeViewColors *GetColorScheme();
	AbstractBackground *GetExpandIcon();
	AbstractBackground *GetCollapseIcon();
	float GetItemHeight();

    TreeNode *GetRootNode();
	void SetSelectedNode(TreeNode *);
	TreeNode *GetSelectedNode();

    void UpdateLinearView();
    void SetDirty();

    void SetTextColor(LPCSTR style);
    void SetHoverColor(LPCSTR style);
    void SetSelectedColor(LPCSTR style);
    void SetSelectedTextColor(LPCSTR style);
    void SetTextFormat(LPCSTR style);

    void SetVScroll(float);

protected:
    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnMouseWheel(MouseEvent *ev) override;
    virtual void RecreateResouce(ID2D1RenderTarget *target) override;
	virtual void OnThemeChanged() override;

private:
    void UpdateLinearViewRec(TreeNode* node, int depth);

    ScrollBar *m_vsb = nullptr;
    TreeNode *m_root;
	TreeNode *m_selected = nullptr;
    std::vector<TreeNode*> m_vecLinear;

    static const float m_itemHeight;
    static const float m_indent;

    IDWriteTextFormat *m_format = nullptr;
	AbstractBackground *m_expandBg = nullptr;
	AbstractBackground *m_collapseBg = nullptr;

    ScrollAnimation m_scrollAni;
    float m_maxHeight = 0.0f;

	TreeViewColors m_colors;

    bool m_bDirty = true;

    LPCSTR m_szTextColor = nullptr;
    LPCSTR m_szHoverColor = nullptr;
    LPCSTR m_szSelectedColor = nullptr;
    LPCSTR m_szSelectedTextColor = nullptr;
    LPCSTR m_szTextFormat = nullptr;

    DISALLOW_COPY_AND_ASSIGN(TreeView)
};

} // namespace ltk
