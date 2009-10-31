#pragma once

#include <QDomElement>
#include <QMap>

class Property;
class OutFile;

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
	virtual void generateCode(OutFile &out) = 0;
	virtual void generateNameMapping(OutFile &out) = 0;
	virtual bool generateObjectRequestString(OutFile &out, bool notIsFirst) = 0;
	virtual bool generateProperties(OutFile &out, bool notIsFirst) = 0;

protected:
	QMap<QString, Property*> mProperties;
	bool mResolvingFinished;
	QString mName;
};
