#pragma once

#include <QDomElement>

class Property
{
public:
	bool init(QDomElement const &element);
	QString name();

private:
	QString mName;
	QString mType;
	bool mIsEnum;
	bool mIsReference;
	QString mDescription;
	QString mDefaultValue;

	bool initReferenceType(QString typeName, QDomElement const &element);
};