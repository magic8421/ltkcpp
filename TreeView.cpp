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

static const float PADDING = 5;
static const float BTN_SIZE = 15;

TreeNode::TreeNode() : Object(new TreeNodePrivate(this))
{
	d_func()->SetTreeNodeType();
}

TreeNode::TreeNode(TreeNodePrivate *d)
{
	d->SetTreeNodeType();
}

TreeNode::~TreeNode()
{
}

TreeNodePrivate::TreeNodePrivate(TreeNode *q) : ObjectPrivate(q)
{
}

TreeNodePrivate::~TreeNodePrivate()
{

}

void TreeNode::SetTreeView(TreeView *tree)
{
	LTK_PUBLIC_DQ(TreeNode);
    d->treeView = tree;
}

void TreeNode::AddChild(TreeNode *node)
{
	LTK_PUBLIC_DQ(TreeNode);

	node->d_func()->treeView = d->treeView;
	Object::AddChild(node);
}

void TreeNode::SetRect(const RectF &rc)
{
	LTK_PUBLIC_DQ(TreeNode);

	d->rect = rc;
	d->rcExpandBtn = RectF(rc.X + PADDING, rc.Y + (rc.Height - BTN_SIZE) / 2.0f,
        BTN_SIZE, BTN_SIZE);
}

RectF TreeNode::GetRect()
{
	LTK_PUBLIC_DQ(TreeNode);
	return d->rect;
}

LPCWSTR TreeNode::GetText()
{
	LTK_PUBLIC_DQ(TreeNode);
	return d->text.c_str();
}

void TreeNode::SetText(LPCWSTR t)
{
	LTK_PUBLIC_DQ(TreeNode);
	d->text = t;
}

bool TreeNode::IsExpand()
{
	LTK_PUBLIC_DQ(TreeNode);
	return d->bExpand;
}

void TreeNode::OnPaint(ID2D1RenderTarget *target, float scroll)
{
	LTK_PUBLIC_DQ(TreeNode);
	auto rcItem = d->rect;
    rcItem.Y -= scroll;
	auto rcSprite = d->treeView->GetClientRect();
	if (rcItem.Y + rcItem.Height < 0.f || rcItem.Y > rcSprite.Height) {
		return;
	}

	auto rcExpandBtn = d->rcExpandBtn;
    rcExpandBtn.Y -= scroll;
	auto colors = d->treeView->GetColorScheme();

	auto brush = d->treeView->GetBrush();
    //brush->SetColor(StyleManager::ColorFromString("#cccccc"));
    //target->DrawRectangle(D2D1RectF(rcItem), brush);

    brush->SetColor(StyleManager::ColorFromString("#aaaaaa"));
    if (this->GetChildCount() > 0) {
        target->FillRectangle(D2D1RectF(rcExpandBtn), brush);
    } else {
        target->DrawRectangle(D2D1RectF(rcExpandBtn), brush);
    }
	auto format = d->treeView->GetTextFormat();
    brush->SetColor(colors->TextColor);
	float space = PADDING * 2.0f + BTN_SIZE;
	auto rcText = RectF(
		space + rcItem.X, rcItem.Y, rcItem.Width - space, rcItem.Height);
    target->DrawText(
		d->text.c_str(), d->text.size(), format, D2D1RectF(rcText), brush);
}

void TreeNode::OnLBtnDown(PointF pt)
{
	LTK_PUBLIC_DQ(TreeNode);

	if (d->rcExpandBtn.Contains(pt)) {
		d->bExpand = !d->bExpand;
    }
}

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
	LTK_PRIVATE_DQ(TreeView);
	q->EnableClipChildren(true);
	d->root.SetTreeView(q);
	d->vsb = new ScrollBar(ScrollBar::Vertical);
	q->AddChild(d->vsb);
}


void TreeView::DoLayout()
{
	LTK_PUBLIC_DQ(TreeView);

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
			auto obj = node->GetChildAt(i);
			LTK_ASSERT(obj->IsTreeNodeType());
            TraverseTree(static_cast<TreeNode*>(obj), depth + 1, cb);
        }
    }
}

ID2D1SolidColorBrush * TreeView::GetBrush()
{
    return GetWindow()->GetStockBrush();
}

IDWriteTextFormat * TreeView::GetTextFormat()
{
	LTK_PUBLIC_DQ(TreeView);

	return d->format;
}

TreeViewColors * TreeView::GetColorScheme()
{
	LTK_PUBLIC_DQ(TreeView);

	return &d->colors;
}

TreeNode * TreeView::GetRootNode()
{
	LTK_PUBLIC_DQ(TreeView);

	return &d->root;
}

bool TreeView::OnPaint(PaintEvent *ev)
{
	LTK_PUBLIC_DQ(TreeView);

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
	LTK_PUBLIC_DQ(TreeView);

	auto sm = StyleManager::Instance();
	d->colors.TextColor = sm->GetColor(d->TextColor);
	d->colors.HoverColor = sm->GetColor(d->HoverColor);
	d->colors.SelectedColor = sm->GetColor(d->SelectedColor);
	d->colors.SelectedTextColor = sm->GetColor(d->SelectedTextColor);

	d->format = sm->GetTextFormat(d->TextFormat);
}

bool TreeView::OnMouseWheel(MouseEvent *ev)
{
	LTK_PUBLIC_DQ(TreeView);

	d->scrollAni.BeginScroll(ev->delta);
    this->BeginAnimation();
    return true;
}

bool TreeView::OnLBtnDown(MouseEvent *ev)
{
	LTK_PUBLIC_DQ(TreeView);

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
	LTK_PUBLIC_DQ(TreeView);

    this->DoLayout();
	d->vsb->SetRect(RectF(ev->width - 8, 0, 6, ev->height));
    return false;
}


} // namespace ltk
