#pragma once
#include "ltk.h"
#include "Sprite.h"

namespace ltk {

class LTK_CPP_API Splitter : public Sprite
{
public:
	explicit Splitter(Orientation m) : m_mode(m) {}
	virtual ~Splitter() {}


	void AddClient(Sprite *sp);
	void SetClientSize(UINT idx, float size);
	
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
		float size = 20.f;
		bool bFixed = false;
		Sprite* client = nullptr;
	};
	std::vector<SplitterItem> m_vecItems;
	Orientation m_mode;
	bool m_bCapture = false;
	float m_dragDelta = 0.f;
	int m_dragIdx = -1;
};

} // namespace ltk
