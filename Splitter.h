#pragma once
#include "Sprite.h"

namespace ltk {

class SplitterPrivate;

class Splitter : public Sprite
{
public:
	enum Mode { Horizontal, Vertical };
	explicit Splitter(Mode m);
	virtual ~Splitter() {}

	void AddClient(Sprite *sp);
	void SetClientSize(UINT idx, float size);
	
	void DoLayout();

protected:
	virtual bool OnMouseMove(MouseEvent *ev) override;
	virtual bool OnLBtnDown(MouseEvent *ev) override;
	virtual bool OnLBtnUp(MouseEvent *ev) override;
	virtual bool OnSize(SizeEvent *ev) override;

private:
	LTK_DECLARE_PRIVATE(Splitter);
protected:
	Splitter(SplitterPrivate *q);
};

} // namespace ltk
