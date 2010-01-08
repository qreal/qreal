#pragma once

#include <QDomElement>

class Property
{
public:
	bool init(QDomElement const &element);
	QString name();
	Property *clone();
	bool operator == (Property const &other) const;
	bool operator != (Property const &other) const;
	void dump() const;

private:
	QString mName;
	QString mType;
	bool mIsEnum;
	bool mIsReference;
	QString mDescription;
	QString mDefaultValue;

	bool initReferenceType(QString typeName, QDomElement const &element);
};
