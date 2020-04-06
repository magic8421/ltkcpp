#include "stdafx.h"
#include "Builder.h"
#include "ltk.h"
#include "Object.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Builder* Builder::m_sInst = nullptr;

Builder* Builder::Instance()
{
	if (!m_sInst) {
		m_sInst = new Builder;
	}
	return m_sInst;
}

void Builder::Free()
{
	delete m_sInst;
}

void Builder::RegisterType(LPCSTR xml_tag, FactoryMethod func)
{
	//auto tag_intern = ltk::InternString(xml_tag);
	// https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map
	m_mapFactory[xml_tag] = func;
}

Object* Builder::WidgetFromXml(LPCSTR path)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(path) != XML_SUCCESS) return nullptr;
	m_buildingPath.clear();
	auto elm = doc.FirstChildElement();
	return WidgetFromXmlRec(elm);
}

Object* Builder::WidgetFromXmlRec(tinyxml2::XMLElement* elm)
{
	Object* parent = nullptr;
	auto name = elm->Name();
	auto iter = m_mapFactory.find(name);
	if (iter != m_mapFactory.end()) {
		m_buildingPath.push_back(name);
		auto hltk = iter->second();
		LTK_ASSERT(Object::CheckValid((Object*)hltk));
		parent = (Object*)hltk;
		auto attr = elm->FirstAttribute();
		while (attr) {
			parent->SetAttribute(ltk::InternString(attr->Name()), attr->Value());
			attr = attr->Next();
		}
		auto child_elm = elm->FirstChildElement();
		while (child_elm) {
			auto child = WidgetFromXmlRec(child_elm);
			if (child) {
				parent->AddChild(child);
			}
			child_elm = child_elm->NextSiblingElement();
		}
		m_buildingPath.pop_back();
	}
	return parent;
}

} // namespace ltk
