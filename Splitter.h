#pragma once
#include "ltk.h"
#include "Sprite.h"

namespace ltk {

class LTK_CPP_API Splitter :  public ILtkSplitter, public Widget
{
public:
	RTTI_DECLARATIONS(Splitter, Widget);

	explicit Splitter(Orientation m) : m_mode(m) {}
	virtual ~Splitter() {}

	//void Resize(UINT n);
	//Widget* SetClientAt(UINT idx, Widget* sp);

	LTK_OBJECT_IMPL()
	LTK_WIDGET_IMPL()

	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override { return E_NOTIMPL; }

	STDMETHOD_(void, AddClient)(ILtkWidget* w) override;
	STDMETHOD(SetClientSize)(UINT idx, float size) override;
	
	float GetTotolSize();
	void DoLayout();
	int HitTest(float x, float y);
	float PosFromIdx(UINT idx);

protected:
	virtual bool OnMouseMove(MouseEvent *ev) override;
	virtual bool OnLBtnDown(MouseEvent *ev) override;
	virtual bool OnLBtnUp(MouseEvent *ev) override;
	virtual bool OnSize(SizeEvent *ev) override;

private:
	struct SplitterItem
	{
		float size = 15.f; // TODO MIN_SIZE
		bool bFixed = false;
		ILtkWidget* client = nullptr;
	};
	std::vector<SplitterItem> m_vecItems;
	Orientation m_mode;
	bool m_bCapture = false;
	float m_dragDelta = 0.f;
	int m_dragIdx = -1;
};

} // namespace ltk
