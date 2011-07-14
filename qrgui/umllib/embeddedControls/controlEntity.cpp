#include "controlEntity.h"

ControlEntity::ControlEntity(const QString &propertyName) :
	propertyName(propertyName)
{}

QString ControlEntity::getPropertyName() const {
	return propertyName;
}
