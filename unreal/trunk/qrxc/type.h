#pragma once

#include <QDomElement>
#include <QMap>

class Property;

namespace utils {
	class OutFile;
}

class Type
{
public:
	Type(bool isResolved);
	virtual ~Type();
	virtual bool init(QDomElement const &element);
	virtual bool resolve() = 0;
	bool isResolved();
	QString name();
	QMap<QString, Property*> properties();
	virtual void generateCode(utils::OutFile &out) = 0;
	virtual void generateNameMapping(utils::OutFile &out) = 0;
	virtual bool generateObjectRequestString(utils::OutFile &out, bool notIsFirst) = 0;
	virtual bool generateProperties(utils::OutFile &out, bool notIsFirst) = 0;

protected:
	QMap<QString, Property*> mProperties;
	bool mResolvingFinished;
	QString mName;
};
