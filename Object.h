#pragma once

#include "Common.h"
#include "RTTI.h"

namespace ltk {

class Object;

/// Reference count structure.
struct RefCount
{
    /// Construct.
    RefCount(Object *obj) :
        refs_(0),
        weakRefs_(0),
		obj_(obj)
    {
    }

    /// Destruct.
    ~RefCount()
    {
    }

    /// Reference count. If below zero, the object has been destroyed.
    int refs_;
    /// Weak reference count.
    int weakRefs_;

	Object* obj_;
};

class LTK_CPP_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	static void Init();
	static void Free();

	Object() : refCount_(new RefCount(this)) 
	{
		// Hold a weak ref to self to avoid possible double delete of the refcount
		(refCount_->weakRefs_)++;
	}

	virtual ~Object()
	{
		LTK_ASSERT(refCount_);
		LTK_ASSERT(refCount_->weakRefs_ > 0);

		if (refCount_->weakRefs_ == 1) {
			delete refCount_;
		}
		else {
			(refCount_->weakRefs_)--; // 判断==1就可以自杀了 完了再-- 这样可以少污染一次缓存 
		}
	}

	void AddRef()
	{
		LTK_ASSERT(refCount_->refs_ >= 0);
		(refCount_->refs_)++;
	}
	void Release()
	{
		LTK_ASSERT(refCount_->refs_ > 0);

		if (refCount_->refs_ == 1) {
			delete this;
		}
		else {
			(refCount_->refs_)--;
		}
	}

	virtual void Dispose() {}

	RefCount* RefCountPtr() { return refCount_; }

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetName(LPCSTR name);
	LPCSTR GetName() { return m_name; }

	static LPCSTR InternString(LPCSTR str);

private:
	RefCount * const refCount_;

	LPCSTR m_name = nullptr; // TODO: move this to Widget class

	DISALLOW_COPY_AND_ASSIGN(Object);
};


} // namespace ltk

