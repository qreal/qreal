#include "control.h"

Control::Control(const QString &propertyName) :
	propertyName(propertyName)
{}

QString Control::getPropertyName() const {
	return propertyName;
}
