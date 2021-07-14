#include "stdafx.h"
#include "TreeView.h"
#include "ScrollBar.h"
#include "StyleManager.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

TreeNode::~TreeNode()
{
    for (auto node : m_children) {
        delete node;
    }
}

void TreeNode::SetTreeView(TreeView *tree)
{
    m_treeView = tree;
}

void TreeNode::AddChild(TreeNode *node)
{
    LTK_ASSERT(node->m_parent == nullptr);
    LTK_ASSERT(node != this);

    node->m_parent = this;
    node->m_treeView = m_treeView;
    m_children.push_back(node);
    m_treeView->SetDirty();
}

UINT TreeNode::GetChildCount()
{
    return m_children.size();
}

TreeNode * TreeNode::GetNthChild(UINT i)
{
    return m_children[i];
}

RectF TreeNode::GetRect(float scroll, UINT idx)
{
    RectF rcItem;
    rcItem.X = m_depth * 15.f;
    rcItem.Y = idx * m_treeView->GetItemHeight() - scroll;
    rcItem.Height = m_treeView->GetItemHeight();
    rcItem.Width = m_treeView->GetWidth() - m_depth * 15.f;;
    return rcItem;
}

RectF TreeNode::GetExpandButtonRect(const RectF& rcItem)
{
    return RectF(rcItem.X + m_padding,
        rcItem.Y + (rcItem.Height - m_btn_size) / 2.0f,
        m_btn_size, m_btn_size);
}

void TreeNode::SetDepth(int d)
{
    m_depth = d;
}

LPCWSTR TreeNode::GetText()
{
    return m_text.c_str();
}

void TreeNode::SetText(LPCWSTR t)
{
    m_text = t;
}

void* TreeNode::GetUserData()
{
	return m_userdata;
}

void TreeNode::SetUserData(void* ud)
{
    m_userdata = ud;
}

bool TreeNode::IsExpand()
{
    return m_bExpand;
}

void TreeNode::OnPaint(ID2D1RenderTarget *target, float scroll, UINT idx)
{
    RectF rcItem = this->GetRect(scroll, idx);

	auto rcExpandBtn = RectF(rcItem.X + m_padding,
        rcItem.Y + (rcItem.Height - m_btn_size) / 2.0f,
        m_btn_size, m_btn_size);

	auto colors = m_treeView->GetColorScheme();

	auto brush = m_treeView->GetBrush();
	if (m_treeView->GetSelectedNode() == this) {
		brush->SetColor(colors->SelectedColor);
		auto rcSelected = rcItem;
		rcSelected.X = 0;
		rcSelected.Width = m_treeView->GetWidth();
		target->FillRectangle(D2D1RectF(rcSelected), brush);
	}

	if (m_children.size() > 0) {
		if (m_bExpand) {
			m_treeView->GetExpandIcon()->Draw(m_treeView->GetWindow(),
				target, RectF(rcExpandBtn.X, rcExpandBtn.Y, 16, 11),
				AbstractBackground::Normal, 0.f);
		} else {
			m_treeView->GetCollapseIcon()->Draw(m_treeView->GetWindow(),
			target, RectF(rcExpandBtn.X, rcExpandBtn.Y, 11, 15),
			AbstractBackground::Normal, 0.f);
		}
	}
	auto format = m_treeView->GetTextFormat();

	if (m_treeView->GetSelectedNode() == this) {
		brush->SetColor(colors->SelectedTextColor);
	}
	else {
		brush->SetColor(colors->TextColor);
	}
	float space = m_padding * 2.0f + m_btn_size;
	auto rcText = RectF(
		space + rcItem.X, rcItem.Y, /*rcItem.Width - space*/ 999, rcItem.Height);
	target->DrawText(
		m_text.c_str(), m_text.size(), format, D2D1RectF(rcText), brush);
}

void TreeNode::OnLBtnDown(PointF pt, float scroll, UINT idx)
{
    auto rcItem = this->GetRect(scroll, idx);
	auto rcExpandBtn = this->GetExpandButtonRect(rcItem);

    if (rcExpandBtn.Contains(pt)) {
        m_bExpand = !m_bExpand;
        m_treeView->SetDirty();
		return;
    }
	if (rcItem.Contains(pt)) {
		//LTK_LOG("SetSelectedNode %08x", this);
		m_treeView->SetSelectedNode(this);
	}
}

const float TreeNode::m_padding = 5;
const float TreeNode::m_btn_size = 15;

//////////////////////////////////////////////////////////////////////////

TreeView::TreeView() :
	m_szTextColor("item_text_clr"),
	m_szHoverColor("item_hover_clr"),
	m_szSelectedColor("item_selected_clr"),
	m_szSelectedTextColor("item_selected_text_clr"),
	m_szTextFormat("tree_item_text_fmt")
{
    this->EnableClipChildren(true);
    m_root = new TreeNode;
    m_root->SetTreeView(this);
    m_vsb = new ScrollBar(ltk::Vertical);
    m_vsb->ValueChangedDelegate += MakeDelegate(this, &TreeView::SetVScroll);
    this->AddChild(m_vsb);
}

TreeView::~TreeView()
{
	delete m_root;
}

void TreeView::DoLayout()
{
    float y = 0.0f;
    RectF rcWidget = GetClientRect();
    UpdateLinearView();
    m_maxHeight = m_vecLinear.size() * m_itemHeight;

    if (m_maxHeight > rcWidget.Height) {
        m_vsb->SetContentSize(m_maxHeight);
        m_vsb->SetVisible(true);
    } else {
        m_vsb->SetVisible(false);
        m_scrollAni.SetScroll(0);
    }
}

ID2D1SolidColorBrush * TreeView::GetBrush()
{
    return GetWindow()->GetStockBrush();
}

IDWriteTextFormat * TreeView::GetTextFormat()
{
    return m_format;
}

TreeViewColors * TreeView::GetColorScheme()
{
	return &m_colors;
}

AbstractBackground * TreeView::GetExpandIcon()
{
	return m_expandBg;
}

ltk::AbstractBackground * TreeView::GetCollapseIcon()
{
	return m_collapseBg;
}

float TreeView::GetItemHeight()
{
	return m_itemHeight;
}

TreeNode * TreeView::GetRootNode()
{
    return m_root;
}

void TreeView::SetSelectedNode(TreeNode* node)
{
	m_selected = node;
}

TreeNode * TreeView::GetSelectedNode()
{
	return m_selected;
}

void TreeView::UpdateLinearViewRec(TreeNode* node, int depth)
{
    node->SetDepth(depth);
    m_vecLinear.push_back(node);
    if (node->IsExpand()) {
        for (UINT i = 0; i < node->GetChildCount(); i++) {
            UpdateLinearViewRec(node->GetNthChild(i), depth + 1);
        }
    }
}

void TreeView::UpdateLinearView()
{
    m_vecLinear.clear();
    for (UINT i = 0; i < m_root->GetChildCount(); i++) {
        UpdateLinearViewRec(m_root->GetNthChild(i), 0);
    }
}

void TreeView::SetDirty()
{
    m_bDirty = true;
}

void TreeView::SetTextColor(LPCSTR style)
{
    this->m_szTextColor = ltk::InternString(style);
}

void TreeView::SetHoverColor(LPCSTR style)
{
    this->m_szHoverColor = ltk::InternString(style);
}

void TreeView::SetSelectedColor(LPCSTR style)
{
    this->m_szSelectedColor = ltk::InternString(style);
}

void TreeView::SetSelectedTextColor(LPCSTR style)
{
    this->m_szSelectedTextColor = ltk::InternString(style);
}

void TreeView::SetTextFormat(LPCSTR style)
{
    this->m_szTextFormat = ltk::InternString(style);
}

void TreeView::SetVScroll(float v)
{
    m_scrollAni.SetScroll(v);
    this->Invalidate();
}

bool TreeView::OnPaint(PaintEvent *ev)
{
    if (m_bDirty) {
        this->DoLayout();
        m_bDirty = false;
    }
    auto rc = this->GetClientRect();
    if (m_scrollAni.UpdateScroll(m_maxHeight - rc.Height)) {
        this->EndAnimation();
    }
    m_vsb->SetPosition(m_scrollAni.GetScroll());

    UINT begin = (UINT)(m_scrollAni.GetScroll() / m_itemHeight);
    UINT end = (UINT)((m_scrollAni.GetScroll() + rc.Height) / m_itemHeight + 1);
    for (UINT i = begin; i < end && i < m_vecLinear.size(); i++) {
        m_vecLinear[i]->OnPaint(ev->target, m_scrollAni.GetScroll(), i);
    }
    return true;
}

void TreeView::RecreateResouce(ID2D1RenderTarget *target)
{
}

void TreeView::OnThemeChanged()
{
	auto sm = StyleManager::Instance();
	m_colors.TextColor = sm->GetColor(m_szTextColor);
	m_colors.HoverColor = sm->GetColor(m_szHoverColor);
	m_colors.SelectedColor = sm->GetColor(m_szSelectedColor);
	m_colors.SelectedTextColor = sm->GetColor(m_szSelectedTextColor);

	m_format = sm->GetTextFormat(m_szTextFormat);
	m_expandBg = sm->GetBackground("tree_expand_bg");
	m_collapseBg = sm->GetBackground("tree_collapse_bg");
}

bool TreeView::OnMouseWheel(MouseEvent *ev)
{
    m_scrollAni.BeginScroll(ev->delta);
    this->BeginAnimation();
    return true;
}

bool TreeView::OnLBtnDown(MouseEvent *ev)
{
	auto scroll = m_scrollAni.GetScroll();

    size_t idx = (size_t)((ev->y + scroll) / m_itemHeight);
    if (idx >= m_vecLinear.size()) {
        return false;
    }
    m_vecLinear[idx]->OnLBtnDown(PointF(ev->x, ev->y), scroll, idx);

    this->Invalidate();
    return true;
}

bool TreeView::OnSize(SizeEvent *ev)
{
    m_vsb->SetRect(RectF(ev->width - 8, 0, 6, ev->height));
    return false;
}

const float TreeView::m_itemHeight = 25.0f;
const float TreeView::m_indent = 15.0f;

} // namespace ltk
