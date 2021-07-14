#pragma once

#include "RTTI.h"
#include "Common.h"
#include "stdafx.h"

namespace ltk {

class RefCounted : public RTTI
{
public:
    RefCounted()
    {
        m_refCount = 1;
    }
	ULONG AddRef()
    {
		InterlockedIncrement(&m_refCount);
		LTK_ASSERT(m_refCount < 9999999);
		return m_refCount;
    }
	ULONG Release()
    {
		// https://docs.microsoft.com/en-us/office/client-developer/outlook/mapi/implementing-iunknown-in-c-plus-plus
		ULONG ulRefCount = InterlockedDecrement(&m_refCount);
		if (0 == m_refCount)
		{
			delete this;
		}
		return ulRefCount;
    }
    bool HasOneRef()
    {
        return m_refCount == 1;
    }

	virtual void Dispose() {}

    RTTI_DECLARATIONS(RefCounted, RTTI);

protected:
	virtual ~RefCounted() {}

private:
    DISALLOW_COPY_AND_ASSIGN(RefCounted);
	ULONG volatile m_refCount;
};

template<typename T>
class RefPtr
{
public:
    RefPtr() : m_ptr(nullptr) {
    }
    explicit RefPtr(T *ptr) : m_ptr(ptr) {
    }

    RefPtr(const RefPtr &rhs) {
        m_ptr = rhs.m_ptr;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    RefPtr(RefPtr &&rhs) {
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
    }

    ~RefPtr() {
        if (m_ptr) {
            m_ptr->Release();
        }
    }

    //Attach to an existing RefCounted (does not AddRef)
	/*
    void Attach(T *p) {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = p;
    }

    T *Detach() {
        T *tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }
	*/

    T *operator->() {
        return m_ptr;
    }

    operator bool() {
        return m_ptr != nullptr;
    }

    void operator=(const RefPtr &rhs) {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = rhs.m_ptr;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    void operator=(RefPtr &&rhs) {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
    }

    void operator=(T *p)
    {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = p;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    bool operator==(const RefPtr<T>& rhs) const
    {
        return this->m_ptr == rhs.m_ptr;
    }

    template <typename Q>
    T* operator=(const RefPtr<Q>& ptr) throw()
    {
        if (ptr->Is<T>()) {
            if (m_ptr) {
                m_ptr->Release();
            }
            m_ptr = ptr->As<T>();
        }
    }

    template <typename Q>
    operator RefPtr<Q>()
    {
        Q *q = static_cast<Q*>(m_ptr);
        return RefPtr<Q>(q);
    }

    T *Get() const {
        return m_ptr;
    }

    void Reset(T *p)
    {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = p;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    void Reset()
    {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = nullptr;
    }

    T **PtrAddress()
    {
        return &m_ptr;
    }

private:
    T *m_ptr;
};

template<typename T>
RefPtr<T> RefPtrFromThis(T *thiz)
{
	thiz->AddRef();
	return RefPtr<T>(this);
}

} // namespace ltk