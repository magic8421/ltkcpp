#pragma once
#include "RTTI.h"
#include "Common.h"
#ifndef LTK_NO_CINTERFACE
#include "LtkInterface.h"
#endif

namespace ltk {

class Object;

class ObserverCtrl {
public:
	explicit ObserverCtrl(Object *p) : pobj(p) {}
	void AddRef() {
		InterlockedIncrement(&refCount);
	}
	void Release() {
		long ref = InterlockedDecrement(&refCount);
		if (ref == 0) {
			delete this;
		}
	}
	Object *Get() {
		return pobj;
	}
	void Set(Object *p){
		InterlockedExchangePointer((void**)&pobj, p);
	}

private:
	Object *pobj;
	long refCount = 1;
};

template<class T>
class Ptr {
public:
	explicit Ptr(ObserverCtrl *ctrl) :m_obctrl(ctrl) {
		m_obctrl->AddRef();
	}
	Ptr(const Ptr &rhs) {
		m_obctrl = rhs.m_obctrl;
		m_obctrl->AddRef();
	}
	Ptr operator=(const Ptr &rhs) {
		m_obctrl->Release();
		m_obctrl = rhs.m_obctrl;
		m_obctrl->AddRef();
		return *this;
	}
	template<class T2, 
	class = typename std::enable_if<std::is_convertible<T2 *, T *>::value, void>::type>
	Ptr(const Ptr<T2> &rhs) {
		Ptr *prhs = (Ptr<T> *)&rhs;
		m_obctrl = prhs->m_obctrl;
		m_obctrl->AddRef();
	}
	~Ptr() {
		m_obctrl->Release();
	}
	T *operator->() {
		return (T*)(m_obctrl->Get());
	}
	T *Get() {
		return (T*)(m_obctrl->Get());
	}
private:
	ObserverCtrl *m_obctrl;
};

template<class T>
class Owner {
public:
	~Owner() { delete m_ptr; }
	Owner() : m_ptr(nullptr) {}
	Owner(T* p) : m_ptr(p) {}
	Owner(const Owner& rhs) = delete;
	void operator=(const Owner& rhs) = delete;

	Owner(Owner &&rhs) {
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;
	}
	void operator=(Owner &&rhs) {
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;
	}
	T* operator->() {
		return m_ptr;
	}
	operator bool() {
		return m_ptr ? true : false;
	}
	bool operator==(const Owner& rhs) {
		return m_ptr == rhs.m_ptr;
	}
	T* reset(T *p) {
		T* old = m_ptr;
		m_ptr = p;
		return old;
	}
	T* reset() {
		delete m_ptr;
	}
	T* get() {
		return m_ptr;
	}
	Ptr<T> ToPtr(){
		return m_ptr->GetPtr<T>();
	}
private:
	T *m_ptr;
};

template <class T>
Ptr<T> ToPtr(T *obj)
{
	return obj->GetPtr<T>();
}

class LTK_CPP_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object();
	virtual ~Object();

	template<typename T>
	Ptr<T> GetPtr() {
		return Ptr<T>(m_obctrl);
	}

	void SetInvalid();

#ifndef LTK_NO_CINTERFACE
	struct CallbackInfo
	{
		LtkCallback callback = nullptr;
		void* userdata = nullptr;
	};

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void RegisterCallback(UINT event_id, LtkCallback cb, void* userdata);
	void InvokeCallback(UINT event_id, ...);

	virtual void DoInvokeCallback(UINT event_id, LtkCallback cb,
		void* userdata, va_list args) {
	}

	static Object *GetEventSender();
#endif

private:
	ObserverCtrl *m_obctrl = nullptr;
#ifndef LTK_NO_CINTERFACE
	std::map<UINT, std::vector<CallbackInfo>> m_mapCallbacks;
	//const char *m_source = nullptr; // 好像没必要 外部使用者应该用umdh来查内存泄漏
	//int m_line = -1;
#endif

	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

