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
}

UINT TreeNode::GetChildCount()
{
    return m_children.size();
}

TreeNode * TreeNode::GetNthChild(UINT i)
{
    return m_children[i];
}

void TreeNode::SetRect(const RectF &rc)
{
    m_rect = rc;
    m_rcExpandBtn = RectF(rc.X + m_padding, rc.Y + (rc.Height - m_btn_size) / 2.0f,
        m_btn_size, m_btn_size);
}

RectF TreeNode::GetRect()
{
    return m_rect;
}

LPCWSTR TreeNode::GetText()
{
    return m_text.c_str();
}

void TreeNode::SetText(LPCWSTR t)
{
    m_text = t;
}

bool TreeNode::IsExpand()
{
    return m_bExpand;
}

void TreeNode::OnPaint(ID2D1RenderTarget *target, float scroll)
{
	auto rcItem = m_rect;
	rcItem.Y -= scroll;
	auto rcSprite = m_treeView->GetClientRect();
	if (rcItem.Y + rcItem.Height < 0.f || rcItem.Y > rcSprite.Height) {
		return;
	}

	auto rcExpandBtn = m_rcExpandBtn;
	rcExpandBtn.Y -= scroll;
	auto colors = m_treeView->GetColorScheme();

	auto brush = m_treeView->GetBrush();
	//brush->SetColor(StyleManager::ColorFromString("#cccccc"));
	//target->DrawRectangle(D2D1RectF(rcItem), brush);

	brush->SetColor(StyleManager::ColorFromString("#aaaaaa"));
	if (m_children.size() > 0) {
		target->FillRectangle(D2D1RectF(rcExpandBtn), brush);
	}
	else {
		target->DrawRectangle(D2D1RectF(rcExpandBtn), brush);
	}
	auto format = m_treeView->GetTextFormat();
	brush->SetColor(colors->TextColor);
	float space = m_padding * 2.0f + m_btn_size;
	auto rcText = RectF(
		space + rcItem.X, rcItem.Y, rcItem.Width - space, rcItem.Height);
	target->DrawText(
		m_text.c_str(), m_text.size(), format, D2D1RectF(rcText), brush);
}

void TreeNode::OnLBtnDown(PointF pt)
{
    if (m_rcExpandBtn.Contains(pt)) {
        m_bExpand = !m_bExpand;
    }
}

const float TreeNode::m_padding = 5;
const float TreeNode::m_btn_size = 15;

//////////////////////////////////////////////////////////////////////////

TreeView::TreeView() :
	TextColor("item_text_clr"),
	HoverColor("item_hover_clr"),
	SelectedColor("item_selected_clr"),
	SelectedTextColor("item_selected_text_clr"),
	TextFormat("tree_item_text_fmt")
{
    this->EnableClipChildren(true);
    m_root.SetTreeView(this);
    m_vsb = new ScrollBar(ltk::Vertical);
    this->AddChild(m_vsb);
}

TreeView::~TreeView()
{
}


void TreeView::DoLayout()
{
    float y = 0.0f;
    RectF rcSprite = GetClientRect();

    TraverseTree(&m_root, 0, [&, this](TreeNode *node, int depth) {
        RectF rc;
        rc.X = (depth - 1) * m_indent;
        rc.Y = y;
        rc.Width = rcSprite.Width - (depth - 1) * m_indent;
        rc.Height = m_itemHeight;
        node->SetRect(rc);
        y += m_itemHeight;
        m_maxHeight = y;
    });
    if (m_maxHeight > rcSprite.Height) {
        m_vsb->SetContentSize(m_maxHeight);
        m_vsb->SetVisible(true);
    } else {
        m_vsb->SetVisible(false);
        m_scrollAni.SetScroll(0);
    }
}

void TreeView::TraverseTree(TreeNode *node, int depth, 
    const std::function<void(TreeNode *, int)> &cb)
{
    if (node != &m_root) {
        cb(node, depth);
    }
    if (node->IsExpand()) {
        for (UINT i = 0; i < node->GetChildCount(); i++) {
            TraverseTree(node->GetNthChild(i), depth + 1, cb);
        }
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

TreeNode * TreeView::GetRootNode()
{
    return &m_root;
}

bool TreeView::OnPaint(PaintEvent *ev)
{
    auto rc = this->GetClientRect();
    if (m_scrollAni.UpdateScroll(m_maxHeight - rc.Height)) {
        this->EndAnimation();
    }
    m_vsb->SetPosition(m_scrollAni.GetScroll());

    TraverseTree(&m_root, 0, [&, this](TreeNode *node, int) {
        node->OnPaint(ev->target, m_scrollAni.GetScroll());
    });
    return true;
}

void TreeView::RecreateResouce(ID2D1RenderTarget *target)
{
}

void TreeView::OnThemeChanged()
{
	auto sm = StyleManager::Instance();
	m_colors.TextColor = sm->GetColor(TextColor);
	m_colors.HoverColor = sm->GetColor(HoverColor);
	m_colors.SelectedColor = sm->GetColor(SelectedColor);
	m_colors.SelectedTextColor = sm->GetColor(SelectedTextColor);

	m_format = sm->GetTextFormat(TextFormat);
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
    TraverseTree(&m_root, 0, 
		[ev, scroll](TreeNode *node, int) {
            node->OnLBtnDown(PointF(ev->x, ev->y + scroll));
    });
    this->DoLayout(); // TODO change callback to return bool, early abort.
    this->Invalidate();
    return true;
}

bool TreeView::OnSize(SizeEvent *ev)
{
    this->DoLayout();
    m_vsb->SetRect(RectF(ev->width - 8, 0, 6, ev->height));
    return false;
}

const float TreeView::m_itemHeight = 30.0f;
const float TreeView::m_indent = 10.0f;



} // namespace ltk
