#include <QtCore/QMetaProperty>

#include "propertyManager.h"
#include "../../../../../qrutils/xmlUtils.h"

using namespace qReal::widgetsEdit;

PropertyManager::PropertyManager(Tool *tool)
	: mTool(tool), mManager(new QtVariantPropertyManager)
{
	initProperties();
	connect(mManager, SIGNAL(valueChanged(QtProperty*,QVariant))
		, this, SLOT(propertyChanged(QtProperty*,QVariant)));
}

void PropertyManager::initProperties()
{
	QMetaObject const *metaObject = mTool->metaObject();
	for (int i = 0; i < metaObject->propertyCount(); ++i) {
		QMetaProperty metaProperty = metaObject->property(i);
		if (metaProperty.isUser()) {
			Property *property = propertyFromMetaProperty(metaProperty);
			mProperties.insert(property, i);
		}
	}
}

Property *PropertyManager::propertyFromMetaProperty(const QMetaProperty &metaProperty)
{
	QString const name = metaProperty.name();
	int type;
	QtVariantProperty *qtProperty;
	if (metaProperty.isEnumType()) {
		type = QtVariantPropertyManager::enumTypeId();
		qtProperty = mManager->addProperty(type, name);
		QMetaEnum metaEnum = metaProperty.enumerator();
		QStringList types;
		for (int i = 0; i < metaEnum.keyCount(); ++i) {
			types << QString(metaEnum.key(i));
		}
		qtProperty->setAttribute("enumNames", types);
	} else if (metaProperty.isFlagType()) {
		qtProperty = mManager->addProperty(QtVariantPropertyManager::flagTypeId(), name);
		// TODO: implement it
		qDebug() << "Got flag type in property editor";
	} else {
		type = metaProperty.type();
		qtProperty = mManager->addProperty(type, name);
	}
	return new Property(name, type, qtProperty);
}

QListIterator<Property *> PropertyManager::propertiesIterator() const
{
	return QListIterator<Property *>(mProperties.keys());
}

QtVariantPropertyManager *PropertyManager::qtPropertyManager() const
{
	return mManager;
}

void PropertyManager::changeProperty(const QString &name, const QVariant &value)
{
	Property *property = findProperty(name);
	if (!property) {
		return;
	}
	QtVariantProperty *qtProperty = property->qtProperty();
	QVariant const oldValue = qtProperty->value();
	if (oldValue != value) {
		qtProperty->setValue(value);
	}
}

void PropertyManager::onSelect()
{
	foreach(Property *property, mProperties.keys()) {
		QVariant const value = propertyValue(property->name());
		if (!value.isNull() && value.isValid()) {
			property->qtProperty()->setValue(value);
		}
	}
}

void PropertyManager::generateXml(QDomElement &element, QDomDocument &document)
{
	QListIterator<Property *> it = propertiesIterator();
	while (it.hasNext()) {
		Property *property = it.next();
		QDomElement propertyElement = document.createElement("property");
		propertyElement.setAttribute("propertyName", property->name());
		if (property->type() != QtVariantPropertyManager::enumTypeId()) {
			QVariant const value = propertyValue(property->name());
			utils::xmlUtils::qVariantToXml(propertyElement, value);
		} else {
			int const value = propertyValue(property->name()).toInt();
			utils::xmlUtils::enumToXml(propertyElement, value);
		}
		element.appendChild(propertyElement);
	}
}

void PropertyManager::deserializeProperty(QDomElement const &element)
{
	QString const name = element.attribute("propertyName", "invalid");
	if (name == "invalid") {
		return;
	}
	QVariant value = utils::xmlUtils::xmlToVariant(element);
	if (!value.isValid()) {
		int const enumValue = utils::xmlUtils::xmlToEnumInt(element);
		if (enumValue < 0) {
			return;
		}
		value = QVariant(enumValue);
	}
	onPropertyChanged(name, value);
}

QVariant PropertyManager::propertyValue(const QString &name)
{
	int index = metaPropertyIndex(name);
	if (index < 0) {
		return QVariant();
	}
	QMetaProperty const metaProperty = mTool->metaObject()->property(index);
	if (!metaProperty.isValid() || !metaProperty.isReadable()) {
		return QVariant();
	}
	return metaProperty.read(mTool);
}

void PropertyManager::propertyChanged(QtProperty *property, const QVariant &value)
{
	onPropertyChanged(property->propertyName(), value);
}

void PropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	int index = metaPropertyIndex(name);
	if (index < 0) {
		return;
	}
	QMetaProperty const metaProperty = mTool->metaObject()->property(index);
	if (!metaProperty.isValid() || !metaProperty.isWritable()) {
		return;
	}
	metaProperty.write(mTool, value);
}

Property *PropertyManager::findProperty(const QString &name)
{
	foreach (Property *property, mProperties.keys()) {
		if (property->name() == name) {
			return property;
		}
	}
	return NULL;
}

int PropertyManager::metaPropertyIndex(QString const &name)
{
	Property *property = findProperty(name);
	if (!property) {
		return -1;
	}
	return mProperties[property];
}
