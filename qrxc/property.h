#pragma once

#include <QDomElement>

class Property
{
public:
	bool init(QDomElement const &element);
	QString name() const;
	QString displayedName() const;
	QString type() const;
	QString defaultValue() const;
	QString description() const;
	bool isReferenceProperty() const;
	Property *clone();
	bool operator == (Property const &other) const;
	bool operator != (Property const &other) const;
	void dump() const;

private:
	QString mName;
	QString mDisplayedName;
	QString mType;
	bool mIsEnum;
	bool mIsReference;
	QString mDescription;
	QString mDefaultValue;

	bool initReferenceType(QString typeName, QDomElement const &element);
};
