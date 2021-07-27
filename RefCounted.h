#pragma once

#include "RTTI.h"
#include "Common.h"
#include "stdafx.h"

namespace ltk {
/*
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
*/
class RefCounted : public RTTI
{
public:
    RefCounted()
    {
        m_refCount = 0;
    }
    virtual ~RefCounted()
    {
        LTK_ASSERT(m_refCount == 0);
    }
	ULONG AddRef()
    {
        m_refCount++;
		LTK_ASSERT(m_refCount < 9999999);
		return m_refCount;
    }
	ULONG Release()
    {
        LTK_ASSERT(m_refCount > 0);

		if (--m_refCount == 0)
        {
            delete this;
        }
        return m_refCount;
    }
    bool HasOneRef()
    {
        return m_refCount == 1;
    }

	virtual void Dispose() {}

    RTTI_DECLARATIONS(RefCounted, RTTI);

private:
    DISALLOW_COPY_AND_ASSIGN(RefCounted);
	ULONG m_refCount;
};

template<typename T>
class Ptr
{
public:
    Ptr() : m_ptr(nullptr) {
    }
    explicit Ptr(T *ptr) : m_ptr(ptr) {
        m_ptr->AddRef();
    }

    Ptr(const Ptr &rhs) {
        m_ptr = rhs.m_ptr;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    Ptr(Ptr &&rhs) {
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
    }

    ~Ptr() {
        if (m_ptr) {
            m_ptr->Release();
        }
    }

    T *operator->() {
        return m_ptr;
    }

    operator bool() {
        return m_ptr != nullptr;
    }

    void operator=(const Ptr &rhs) {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = rhs.m_ptr;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    void operator=(Ptr &&rhs) {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
    }

    /*
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
    */

    bool operator==(const Ptr<T>& rhs) const
    {
        return this->m_ptr == rhs.m_ptr;
    }

    bool operator!=(const Ptr<T>& rhs) const
    {
        return this->m_ptr != rhs.m_ptr;
    }

    bool operator==(T* rhs) const
    {
        return this->m_ptr == rhs;
    }

    bool operator!=(T* rhs) const
    {
        return this->m_ptr != rhs;
    }

    template <typename Q>
    T* operator=(const Ptr<Q>& ptr) throw()
    {
        if (ptr->Is<T>()) {
            if (m_ptr) {
                m_ptr->Release();
            }
            m_ptr = ptr->As<T>();
        }
    }

    template <typename Q>
    operator Ptr<Q>()
    {
        Q *q = static_cast<Q*>(m_ptr);
        return Ptr<Q>(q);
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

} // namespace ltk