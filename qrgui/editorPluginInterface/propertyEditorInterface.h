#pragma once

#include <QtCore/QString>

class PropertyEditorInterface
{
public:
	virtual ~PropertyEditorInterface() {}

	virtual QString binding() const = 0;
	virtual void setPropertyValue(QString const &value) = 0;
	virtual void setEnumValues(QStringList const &values) = 0;

};
