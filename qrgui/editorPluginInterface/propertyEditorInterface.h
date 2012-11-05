#pragma once

#include <QtCore/QVariant>

class PropertyEditorInterface
{
public:
	virtual ~PropertyEditorInterface() {}

	virtual QString binding() const = 0;
	virtual QString value() const = 0;
	virtual void setValue(QString const &value) = 0;

};
