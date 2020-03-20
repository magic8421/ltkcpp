#pragma once
#include "ltk.h"
#include "Sprite.h"

namespace ltk {

class LTK_CPP_API Splitter : public Widget
{
public:
	RTTI_DECLARATIONS(Splitter, Widget);

	explicit Splitter(Orientation m) : m_mode(m) {}
	virtual ~Splitter() {}

	void Resize(UINT n);
	Widget* SetClientAt(UINT idx, Widget* sp);
	void AddClient(Widget*sp);
	void SetClientSize(UINT idx, float size);
	
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
		Widget* client = nullptr;
	};
	std::vector<SplitterItem> m_vecItems;
	Orientation m_mode;
	bool m_bCapture = false;
	float m_dragDelta = 0.f;
	int m_dragIdx = -1;
};

} // namespace ltk
