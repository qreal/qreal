#include "control.h"

Control::Control(const QString &propertyName) :
	propertyName(propertyName)
{
}

QString Control::getProperty() const {
	return propertyName;
}
