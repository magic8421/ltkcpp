#pragma once

#include "Object.h"
#include "Common.h"
#include "stdafx.h"

namespace ltk {

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
}; // Ptr


/// Weak pointer template class with intrusive reference counting. Does not keep the object pointed to alive.
template <class T> class Weak
{
public:
    /// Construct a null weak pointer.
    Weak() :
        refCount_(nullptr)
    {
    }

    /// Construct a null weak pointer.
    Weak(std::nullptr_t) :
        refCount_(nullptr)
    {
    }

    /// Copy-construct from another weak pointer.
    Weak(const Weak<T>& rhs) :
        refCount_(rhs.refCount_)
    {
        AddRef();
    }

    /// Copy-construct from another weak pointer allowing implicit upcasting.
    template <class U> Weak(const Weak<U>& rhs) :
        refCount_(rhs.refCount_)
    {
        static_assert(std::is_convertible<U*, T*>());
        AddRef();
    }

    /// Construct from a shared pointer.
    Weak(const Ptr<T>& rhs) :
        refCount_(rhs.Get()->RefCountPtr())
    {
        AddRef();
    }

    /// Construct from a raw pointer.

    explicit Weak(T* ptr) :
        refCount_(ptr ? ptr->RefCountPtr() : 0)
    {
        AddRef();
    }

    /// Destruct. Release the weak reference to the object.
    ~Weak()
    {
        ReleaseRef();
    }

    /// Assign from a shared pointer.
    Weak<T>& operator =(const Ptr<T>& rhs)
    {
        if (refCount_ == rhs.RefCountPtr())
            return *this;

        ReleaseRef();
        refCount_ = rhs.RefCountPtr();
        AddRef();

        return *this;
    }

    /// Assign from a weak pointer.
    Weak<T>& operator =(const Weak<T>& rhs)
    {
        if (refCount_ == rhs.refCount_)
            return *this;

        ReleaseRef();
        refCount_ = rhs.refCount_;
        AddRef();

        return *this;
    }

    /// Assign from another weak pointer allowing implicit upcasting.
    template <class U> Weak<T>& operator =(const Weak<U>& rhs)
    {
        static_assert(std::is_convertible<U*, T*>());

        if (refCount_ == rhs.refCount_)
            return *this;

        ReleaseRef();
        refCount_ = rhs.refCount_;
        AddRef();

        return *this;
    }

    /// Assign from a raw pointer.
    Weak<T>& operator =(T* ptr)
    {
        RefCount* refCount = ptr ? ptr->RefCountPtr() : 0;

        if (refCount_ == refCount)
            return *this;

        ReleaseRef();
        refCount_ = refCount;
        AddRef();

        return *this;
    }

    /// Convert to a shared pointer. If expired, return a null shared pointer.
    Ptr<T> Lock() const
    {
        if (refCount_) {
            Object* pObj;
            refCount_->Lock(&pObj);
            if (pObj) {
                Ptr<T> ret(static_cast<T*>(pObj));
                pObj->Release();
                return ret;
            }
        }
        return Ptr<T>();

        /*
        if (Expired())
            return Ptr<T>();
        else
            return Ptr<T>(static_cast<T*>(refCount_->obj_));
        */
    }

    /// Return raw pointer. If expired, return null.
    T* Get() const
    {
        return refCount_ ? static_cast<T*>(refCount_->obj_) : nullptr;
    }

    /// Test for equality with another weak pointer.
    template <class U> bool operator ==(const Weak<U>& rhs) const { return refCount_ == rhs.refCount_; }

    /// Test for inequality with another weak pointer.
    template <class U> bool operator !=(const Weak<U>& rhs) const { return refCount_ != rhs.refCount_; }

    /// Test for less than with another weak pointer.
    template <class U> bool operator <(const Weak<U>& rhs) const { return refCount_ < rhs.refCount_; }

    /// Convert to a raw pointer, null if the object is expired.
    //operator T* () const { return Get(); }

    /// Reset to null and release the weak reference.
    void Reset() { ReleaseRef(); }

    /// Perform a static cast from a weak pointer of another type.
    template <class U> void StaticCast(const Weak<U>& rhs)
    {
        static_assert(std::is_convertible<U*, T*>());

        ReleaseRef();
        refCount_ = rhs.refCount_;
        AddRef();
    }

    /// Perform a dynamic cast from a weak pointer of another type.
    template <class U> void DynamicCast(const Weak<U>& rhs)
    {
        ReleaseRef();
        auto ptr_ = dynamic_cast<T*>(rhs.Get());

        if (ptr_) {
            refCount_ = rhs.refCount_;
            AddRef();
        }
        else
            refCount_ = 0;
    }

    /// Check if the pointer is null.
    bool Null() const { return refCount_ == 0 || refCount_->obj_ == 0; }

    /// Check if the pointer is not null.
    bool NotNull() const { return refCount_ != 0 && refCount_->obj_ != 0; }

    /// Return the object's reference count, or 0 if null pointer or if object has expired.
    int Refs() const { return (refCount_ && refCount_->obj_ != 0) ? refCount_->refs_ : 0; }

    /// Return the object's weak reference count.
    int WeakRefs() const
    {
        return refCount_ ? refCount_->weakRefs_ : 0;
    }

    /// Return whether the object has expired. If null pointer, always return true.
    bool Expired() const { return refCount_ ? refCount_->Expired() : true; }

    /// Return pointer to the RefCount structure.
    RefCount* RefCountPtr() const { return refCount_; }

    /// Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return (unsigned)((size_t)refCount_->obj_ / sizeof(T)); }

private:
    template <class U> friend class Weak;

    /// Add a weak reference to the object pointed to.
    void AddRef()
    {
        if (refCount_) {
            refCount_->AddRef();
        }
    }

    /// Release the weak reference. Delete the Refcount structure if necessary.
    void ReleaseRef()
    {
        if (refCount_) {
            refCount_->Release();
        }
    }

    /// Pointer to the RefCount structure.
    RefCount* refCount_;
};

} // namespace ltk