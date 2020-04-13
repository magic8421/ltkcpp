#pragma once

#define LTK_NOTHROW __declspec(nothrow)

struct ILtkObject
{
	STDMETHOD_(void, Delete)() PURE;
	STDMETHOD_(void, MemberDeleted)(ILtkObject* sender) PURE;
	STDMETHOD_(void, RegisterDeleteNotify)(ILtkObject* recv) PURE;
	STDMETHOD_(void*, QueryInterface)(REFGUID guid) PURE;
	STDMETHOD_(void, AddChild)(ILtkObject* child) PURE;
	STDMETHOD_(void, RemoveChild)(ILtkObject* child) PURE;
	STDMETHOD_(ILtkObject*, GetNthChild)(UINT idx) PURE;
	STDMETHOD_(UINT, GetChildCount)() PURE;
	STDMETHOD_(ILtkObject*, GetParent)() PURE;
	STDMETHOD_(void, SetParent)(ILtkObject*) PURE;
	STDMETHOD_(LPCSTR, GetName)() PURE;
	STDMETHOD_(void, SetName)(LPCSTR name) PURE;
};