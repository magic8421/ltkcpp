#pragma once

#include "Common.h"
#include "RTTI.h"

namespace ltk {

class Object;

/// Reference count structure.
class RefCount
{
public:
	RefCount(Object* obj) :
		refs_(0),
		weakRefs_(0),
		obj_(obj)
	{
	}

	~RefCount()
	{
	}

	void AddRef()
	{
		LTK_ASSERT(weakRefs_ >= 0);
		++weakRefs_;
	}

	void Release()
	{
		LTK_ASSERT(weakRefs_ > 0);

		if (weakRefs_ == 1) {
			delete this;
		}
		else {
			--weakRefs_;
		}
	}

	bool Expired() const { return obj_ == nullptr; }

	void Lock(Object** ppObj); // TODO how to inline this function?
	
	friend class Object;

private:
    /// Reference count. If below zero, the object has been destroyed.
    long refs_;
    /// Weak reference count.
    long weakRefs_;

	Object* obj_;

#ifdef _DEBUG
	unsigned long threadId_;
#endif
};

class Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	static void Init();
	static void Free();

	Object() : refCount_(new RefCount(this)) 
	{
		refCount_->AddRef();
	}

	virtual ~Object()
	{
		refCount_->Release();
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
			refCount_->Release();
			delete this;
		}
		else {
			(refCount_->refs_)--;
		}
	}

	void GetWeakRef(RefCount** ppWeak)
	{
		refCount_->AddRef();
		*ppWeak = refCount_;
	}

	virtual void Dispose() {}

	RefCount* RefCountPtr() { return refCount_; }

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetName(LPCSTR name);
	LPCSTR GetName() { return m_name; }

	static LPCSTR InternString(LPCSTR str);

	friend class RefCount;

private:
	RefCount * const refCount_;

	LPCSTR m_name = nullptr; // TODO: move this to Widget class

	DISALLOW_COPY_AND_ASSIGN(Object);
};


} // namespace ltk

