#pragma once

#include <QtXml/QDomElement>

class Property
{
public:
	bool init(const QDomElement &element);
	QString name() const;
	QString displayedName() const;
	QString type() const;
	QString defaultValue() const;
	QString description() const;
	bool isReferenceProperty() const;
	Property *clone();
	bool operator == (const Property &other) const;
	bool operator != (const Property &other) const;
	void dump() const;

private:
	QString mName;
	QString mDisplayedName;
	QString mType;
	bool mIsEnum;
	bool mIsReference;
	QString mDescription;
	QString mDefaultValue;

	bool initReferenceType(QString typeName, const QDomElement &element);
};
