#include "stdafx.h"
#include "TreeView.h"
#include "ScrollBar.h"
#include "StyleManager.h"

namespace ltk
{

void TreeNode::AddChild(TreeNode *node)
{
    LTK_ASSERT(node->m_parent == nullptr);
    node->m_parent = this;
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

    auto format = m_treeView->GetTextFormat();
    brush->SetColor(StyleManager::ColorFromString("#000000"));
    target->DrawText(
        m_text.c_str(), m_text.size(), format, D2D1RectF(m_rect), brush);
}


TreeView::~TreeView()
{
    delete m_vsb;
    SAFE_RELEASE(m_brush);
    SAFE_RELEASE(m_format);
}


void TreeView::DoLayout()
{
    bool bRoot = true;
    float y = 0.0f;
    RectF rcSprite = GetClientRect();

    TraverseTree(&m_root, 0, [&](TreeNode *node, int depth) {
        if (bRoot) {
            bRoot = false;
            return;
        }
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
    cb(node, depth);
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
    TraverseTree(&m_root, 0, [ev](TreeNode *node, int) {
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

    SAFE_RELEASE(m_brush);
    auto textColor = StyleManager::ColorFromString("#000000");
    hr = target->CreateSolidColorBrush(textColor, &m_brush);
    LTK_ASSERT(SUCCEEDED(hr));
}

} // namespace ltk
