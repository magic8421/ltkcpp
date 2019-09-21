//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&) = delete;             \
	void operator=(const TypeName&) = delete;

#define SAFE_RELEASE(p) if (p) { (p)->Release(); (p) = nullptr; }

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

#ifdef _DEBUG
#define LTK_ASSERT(expr) if (!(expr)) {__debugbreak();} // super KISS
#else
#define LTK_ASSERT(expr) (void)(expr)
#endif

#define LTK_D(Class) Class##Private * const d = d_func()
#define LTK_Q(Class) Class * const q = q_func()

#define LTK_DECLARE_PRIVATE(Class)\
    inline Class##Private* d_func() {\
        return (Class##Private*)(d_ptr);\
    }\
	friend class Class##Private;
 //   inline const Class##Private d_func() const {\
 //       return (const Class##Private *)(d_ptr);\
 //   }
	//

#ifdef LTK_EXPORTS
#define LTK_API __declspec(dllexport)
#else
#define LTK_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////

#define  LTK_LOG(...) LtkLogImpl(__FILE__, __LINE__, __VA_ARGS__)

void LTK_API LtkLogInit();
void LTK_API LtkLogImpl(const char *source, int line, const char *format, ...);

//////////////////////////////////////////////////////////////////////////

namespace ltk {

class SizeF
{
public:
	SizeF()
	{
		Width = Height = 0.0f;
	}

	SizeF(const SizeF& size)
	{
		Width = size.Width;
		Height = size.Height;
	}

	SizeF(float width, float height)
	{
		Width = width;
		Height = height;
	}

	SizeF operator+(const SizeF& sz) const
	{
		return SizeF(Width + sz.Width,
			Height + sz.Height);
	}

	SizeF operator-(const SizeF& sz) const
	{
		return SizeF(Width - sz.Width,
			Height - sz.Height);
	}

	bool Equals(const SizeF& sz) const
	{
		return (Width == sz.Width) && (Height == sz.Height);
	}

	bool Empty() const
	{
		return (Width == 0.0f && Height == 0.0f);
	}

public:
	float Width;
	float Height;
};

class PointF
{
public:
	PointF()
	{
		X = Y = 0.0f;
	}

	PointF(const PointF &point)
	{
		X = point.X;
		Y = point.Y;
	}

	PointF(const SizeF &size)
	{
		X = size.Width;
		Y = size.Height;
	}

	PointF(float x, float y)
	{
		X = x;
		Y = y;
	}

	PointF operator+(const PointF& point) const
	{
		return PointF(X + point.X,
			Y + point.Y);
	}

	PointF operator-(const PointF& point) const
	{
		return PointF(X - point.X,
			Y - point.Y);
	}

	BOOL Equals(const PointF& point)
	{
		return (X == point.X) && (Y == point.Y);
	}

public:

	float X;
	float Y;
};

class RectF
{
public:

	RectF()
	{
		X = Y = Width = Height = 0.0f;
	}

	RectF(float x, float y, float width, float height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}

	RectF(const PointF& location, const SizeF& size)
	{
		X = location.X;
		Y = location.Y;
		Width = size.Width;
		Height = size.Height;
	}

	RectF* Clone() const
	{
		return new RectF(X, Y, Width, Height);
	}

	void GetLocation(PointF* point) const
	{
		point->X = X;
		point->Y = Y;
	}

	void GetSize(SizeF* size) const
	{
		size->Width = Width;
		size->Height = Height;
	}

	void GetBounds(RectF* rect) const
	{
		rect->X = X;
		rect->Y = Y;
		rect->Width = Width;
		rect->Height = Height;
	}

	float GetLeft() const
	{
		return X;
	}

	float GetTop() const
	{
		return Y;
	}

	float GetRight() const
	{
		return X + Width;
	}

	float GetBottom() const
	{
		return Y + Height;
	}

	bool IsEmptyArea() const
	{
		const float epsilon = 1.192092896e-07F;
		return (Width <= epsilon) || (Height <= epsilon);
	}

	bool Equals(const RectF & rect) const
	{
		return X == rect.X &&
			Y == rect.Y &&
			Width == rect.Width &&
			Height == rect.Height;
	}

	bool Contains(float x, float y) const
	{
		return x >= X && x < X + Width &&
			y >= Y && y < Y + Height;
	}

	bool Contains(const PointF& pt) const
	{
		return Contains(pt.X, pt.Y);
	}

	bool Contains(const RectF& rect) const
	{
		return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
			(Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
	}

	void Inflate(float dx, float dy)
	{
		X -= dx;
		Y -= dy;
		Width += 2 * dx;
		Height += 2 * dy;
	}

	void Inflate(const PointF& point)
	{
		Inflate(point.X, point.Y);
	}

	bool Intersect(const RectF& rect)
	{
		return Intersect(*this, *this, rect);
	}

	static bool Intersect(RectF& c, const RectF& a, const RectF& b)
	{
		float right = min(a.GetRight(), b.GetRight());
		float bottom = min(a.GetBottom(), b.GetBottom());
		float left = max(a.GetLeft(), b.GetLeft());
		float top = max(a.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	bool IntersectsWith(const RectF& rect) const
	{
		return (GetLeft() < rect.GetRight() &&
			GetTop() < rect.GetBottom() &&
			GetRight() > rect.GetLeft() &&
			GetBottom() > rect.GetTop());
	}

	static bool Union(RectF& c, const RectF& a, const RectF& b)
	{
		float right = max(a.GetRight(), b.GetRight());
		float bottom = max(a.GetBottom(), b.GetBottom());
		float left = min(a.GetLeft(), b.GetLeft());
		float top = min(a.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	void Offset(const PointF& point)
	{
		Offset(point.X, point.Y);
	}

	void Offset(float dx, float dy)
	{
		X += dx;
		Y += dy;
	}

public:

	float X;
	float Y;
	float Width;
	float Height;
};

} // namespace ltk
