#pragma once

#include <QtCore/QVariant>

class PropertyEditorInterface
{
public:
	virtual ~PropertyEditorInterface() {}

	virtual QString binding() const = 0;
	virtual void setPropertyValue(QVariant const &value) = 0;
	virtual void setEnumValues(QList<QPair<QString, QString>> const &values) = 0;

};
