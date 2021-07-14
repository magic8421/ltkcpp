#pragma once
#include "RefCounted.h"
namespace ltk {

class Widget;

typedef RefPtr<Widget> (*FactoryMethod)();

class Builder
{
protected:
	Builder() {}
	~Builder() {}

public:
	static Builder* Instance();
	static void Free();

	void RegisterType(LPCSTR xml_tag, FactoryMethod func);
	RefPtr<Widget> WidgetFromXml(LPCSTR path);

private:
	RefPtr<Widget> WidgetFromXmlRec(tinyxml2::XMLElement* elm, Widget* parent);

	static Builder* m_sInst;

	std::unordered_map<std::string, FactoryMethod> m_mapFactory;
	std::vector<std::string> m_buildingPath;
};

} // namespace ltk
