#pragma once

#include <QtCore/QVariant>

class PropertyEditorInterface
{
public:
	virtual ~PropertyEditorInterface() {}

	virtual QString binding() const = 0;
	virtual void setPropertyValue(QVariant const &value) = 0;
	virtual void setEnumValues(QStringList const &values) = 0;

};
