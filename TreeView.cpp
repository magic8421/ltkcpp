#include "stdafx.h"
#include "TreeView.h"
#include "TreeView_p.h"
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
    } else {
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

TreeView::TreeView() : Sprite(new TreeViewPrivate(this))
{
	d_func()->Init();
}

TreeView::TreeView(TreeViewPrivate *d)
{
	d->Init();
}

TreeView::~TreeView()
{
}

TreeViewPrivate::TreeViewPrivate(TreeView *q) : SpritePrivate(q),
	TextColor("item_text_clr"),
	HoverColor("item_hover_clr"),
	SelectedColor("item_selected_clr"),
	SelectedTextColor("item_selected_text_clr"),
	TextFormat("tree_item_text_fmt")
{
}

void TreeViewPrivate::Init()
{
	auto q = q_func();
	auto d = this;
	q->EnableClipChildren(true);
	d->root.SetTreeView(q);
	d->vsb = new ScrollBar(ScrollBar::Vertical);
	q->AddChild(d->vsb);
}


void TreeView::DoLayout()
{
	LTK_D(TreeView);

    float y = 0.0f;
    RectF rcSprite = GetClientRect();

    d->TraverseTree(&d->root, 0, [&, this](TreeNode *node, int depth) {
        RectF rc;
		rc.X = (depth - 1) * d->indent;
        rc.Y = y;
		rc.Width = rcSprite.Width - (depth - 1) * d->indent;
		rc.Height = d->itemHeight;
        node->SetRect(rc);
		y += d->itemHeight;
		d->maxHeight = y;
    });
	if (d->maxHeight > rcSprite.Height) {
		d->vsb->SetContentSize(d->maxHeight);
		d->vsb->SetVisible(true);
    } else {
		d->vsb->SetVisible(false);
		d->scrollAni.SetScroll(0);
    }
}

void TreeViewPrivate::TraverseTree(TreeNode *node, int depth, 
    const std::function<void(TreeNode *, int)> &cb)
{
	auto d = this;

	if (node != &d->root) {
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
	LTK_D(TreeView);

	return d->format;
}

TreeViewColors * TreeView::GetColorScheme()
{
	LTK_D(TreeView);

	return &d->colors;
}

TreeNode * TreeView::GetRootNode()
{
	LTK_D(TreeView);

	return &d->root;
}

bool TreeView::OnPaint(PaintEvent *ev)
{
	LTK_D(TreeView);

    auto rc = this->GetClientRect();
	d->scrollAni.UpdateScroll(d->maxHeight - rc.Height);
	if (!d->scrollAni.IsRunning()) {
		this->EndAnimation();
	}
	d->vsb->SetPosition(d->scrollAni.GetScroll());

	d->TraverseTree(&d->root, 0, [&, this](TreeNode *node, int) {
		node->OnPaint(ev->target, d->scrollAni.GetScroll());
    });
    return true;
}

void TreeView::OnRecreateResouce(ID2D1RenderTarget *target)
{
}

void TreeView::OnThemeChanged()
{
	LTK_D(TreeView);

	auto sm = StyleManager::Instance();
	d->colors.TextColor = sm->GetColor(d->TextColor);
	d->colors.HoverColor = sm->GetColor(d->HoverColor);
	d->colors.SelectedColor = sm->GetColor(d->SelectedColor);
	d->colors.SelectedTextColor = sm->GetColor(d->SelectedTextColor);

	d->format = sm->GetTextFormat(d->TextFormat);
}

bool TreeView::OnMouseWheel(MouseEvent *ev)
{
	LTK_D(TreeView);

	d->scrollAni.BeginScroll(ev->delta);
    this->BeginAnimation();
    return true;
}

bool TreeView::OnLBtnDown(MouseEvent *ev)
{
	LTK_D(TreeView);

	auto scroll = d->scrollAni.GetScroll();
	d->TraverseTree(&d->root, 0,
		[ev, scroll](TreeNode *node, int) {
            node->OnLBtnDown(PointF(ev->x, ev->y + scroll));
    });
    this->DoLayout(); // TODO change callback to return bool, early abort.
    this->Invalidate();
    return true;
}

bool TreeView::OnSize(SizeEvent *ev)
{
	LTK_D(TreeView);

    this->DoLayout();
	d->vsb->SetRect(RectF(ev->width - 8, 0, 6, ev->height));
    return false;
}


} // namespace ltk
