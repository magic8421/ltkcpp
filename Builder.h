#pragma once

#include "LtkInterface.h"

namespace ltk {

class Object;
class Widget;

typedef HLTK (CALLBACK *FactoryMethod)();

class Builder
{
protected:
	Builder() {}
	~Builder() {}

public:
	static Builder* Instance();
	static void Free();

	void RegisterType(LPCSTR xml_tag, FactoryMethod func);
	Object* WidgetFromXml(LPCSTR path);

private:
	Object* WidgetFromXmlRec(tinyxml2::XMLElement *elm);


	static Builder* m_sInst;

	std::unordered_map<std::string, FactoryMethod> m_mapFactory;
	std::vector<std::string> m_buildingPath;
};

} // namespace ltk
