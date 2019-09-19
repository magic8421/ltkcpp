#pragma once
#include "Sprite.h"

namespace ltk {

class Splitter : public Sprite
{
public:
	enum Mode { Horizontal, Vertical };
	explicit Splitter(Mode m) : m_mode(m) {}
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
		float size = 10.f;
		bool bFixed = false;
		Sprite* client = nullptr;
	};
	std::vector<SplitterItem> m_vecItems;
	Mode m_mode;
	bool m_bCapture = false;
	float m_dragPos = 0.f;
	int m_dragIdx = -1;
};

} // namespace ltk
