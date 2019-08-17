#include "stdafx.h"
#include "TreeView.h"
#include "ScrollBar.h"
#include "StyleManager.h"

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

void TreeNode::OnPaint(ID2D1RenderTarget *target)
{
    auto brush = m_treeView->GetBrush();
    brush->SetColor(StyleManager::ColorFromString("#cccccc"));
    target->DrawRectangle(D2D1RectF(m_rect), brush);

    brush->SetColor(StyleManager::ColorFromString("#aaaaaa"));
    target->DrawRectangle(D2D1RectF(m_rcExpandBtn), brush);

    auto format = m_treeView->GetTextFormat();
    brush->SetColor(StyleManager::ColorFromString("#000000"));
    float space = m_padding * 2.0f + m_btn_size;
    target->DrawText(
        m_text.c_str(), m_text.size(), format, D2D1RectF(RectF(
            space + m_rect.X, m_rect.Y, m_rect.Width - space, m_rect.Height
        )), brush);
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

TreeView::TreeView()
{
    this->EnableClipChildren(true);
    m_root.SetTreeView(this);
}

TreeView::~TreeView()
{
    delete m_vsb;
    SAFE_RELEASE(m_brush);
    SAFE_RELEASE(m_format);
}


void TreeView::DoLayout()
{
    float y = 0.0f;
    RectF rcSprite = GetClientRect();

    TraverseTree(&m_root, 0, [&](TreeNode *node, int depth) {
        RectF rc;
        rc.X = (depth - 1) * m_indent;
        rc.Y = y;
        rc.Width = rcSprite.Width - (depth - 1) * m_indent;
        rc.Height = m_itemHeight;
        node->SetRect(rc);
        y += m_itemHeight;
    });
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
    return m_brush;
}

IDWriteTextFormat * TreeView::GetTextFormat()
{
    return m_format;
}

TreeNode * TreeView::GetRootNode()
{
    return &m_root;
}

bool TreeView::OnPaint(PaintEvent *ev)
{
    TraverseTree(&m_root, 0, [&](TreeNode *node, int) {
        node->OnPaint(ev->target);
    });
    return true;
}

void TreeView::RecreateResouce(ID2D1RenderTarget *target)
{
    HRESULT hr = E_FAIL;

    SAFE_RELEASE(m_format);
    hr = GetDWriteFactory()->CreateTextFormat(
        L"Î¢ÈíÑÅºÚ",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        16,
        L"zh-cn",
        &m_format
    );
    LTK_ASSERT(SUCCEEDED(hr));
    m_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    SAFE_RELEASE(m_brush);
    auto textColor = StyleManager::ColorFromString("#000000");
    hr = target->CreateSolidColorBrush(textColor, &m_brush);
    LTK_ASSERT(SUCCEEDED(hr));
}

bool TreeView::OnLBtnDown(MouseEvent *ev)
{
    TraverseTree(&m_root, 0, [ev](TreeNode *node, int) {
        node->OnLBtnDown(PointF(ev->x, ev->y));
    });
    this->DoLayout(); // TODO change callback to return bool, early abort.
    this->Invalidate();
    return true;
}

bool TreeView::OnSize(SizeEvent *ev)
{
    this->DoLayout();
    return false;
}

const float TreeView::m_itemHeight = 30.0f;
const float TreeView::m_indent = 10.0f;



} // namespace ltk
