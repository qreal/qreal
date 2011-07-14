#include "embeddedControl.h"

EmbeddedControl::EmbeddedControl(const QString &propertyName) :
	propertyName(propertyName)
{}

QString EmbeddedControl::getPropertyName() const {
	return propertyName;
}
