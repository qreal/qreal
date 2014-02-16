#include <QtCore/QMetaProperty>

#include "propertyManager.h"
#include<qrutils/xmlUtils.h>

using namespace qReal::widgetsEdit;

PropertyManager::PropertyManager(QObject *tool)
	: mTool(tool), mManager(new QtVariantPropertyManager)
	, mDeserializing(false)
{
	initProperties();
	connect(mManager, SIGNAL(valueChanged(QtProperty*,QVariant))
		, this, SLOT(propertyChanged(QtProperty*,QVariant)));
}

void PropertyManager::initProperties()
{
	QMetaObject const *metaObject = mTool->metaObject();
	for (int i = 0; i < metaObject->propertyCount(); ++i) {
		QMetaProperty const metaProperty = metaObject->property(i);
		Property *property = propertyFromMetaProperty(metaProperty);
		if (metaProperty.isUser()) {
			mUserProperties.insert(property, i);
			if (metaProperty.isDesignable()) {
				mDesignableProperties.append(property);
			}
		}
	}
}

Property *PropertyManager::propertyFromMetaProperty(const QMetaProperty &metaProperty)
{
	QString const name = metaProperty.name();
	int type;
	QtVariantProperty *qtProperty;
	if (metaProperty.isFlagType()) {
		// TODO: Add QtDesigner Alignment-like handling
		type = QtVariantPropertyManager::flagTypeId();
		qtProperty = mManager->addProperty(type, name);
		QMetaEnum metaEnum = metaProperty.enumerator();
		QStringList enumKeys;
		QList<int> enumValues;
		for (int i = 0; i < metaEnum.keyCount(); ++i) {
			enumKeys << metaEnum.key(i);
			enumValues << metaEnum.value(i);
		}
		qtProperty->setAttribute("flagNames", enumKeys);
		mFlagProperties << name;
	} else if (metaProperty.isEnumType()) {
		type = QtVariantPropertyManager::enumTypeId();
		qtProperty = mManager->addProperty(type, name);
		QMetaEnum metaEnum = metaProperty.enumerator();
		QStringList enumKeys;
		QList<int> enumValues;
		for (int i = 0; i < metaEnum.keyCount(); ++i) {
			enumKeys << metaEnum.key(i);
			enumValues << metaEnum.value(i);
		}
		qtProperty->setAttribute("enumNames", enumKeys);
		mEnumProperties.insert(name, enumValues);
	} else {
		type = metaProperty.type();
		qtProperty = mManager->addProperty(type, name);
	}
	return new Property(name, type, qtProperty);
}

QListIterator<Property *> PropertyManager::userPropertiesIterator() const
{
	return QListIterator<Property *>(mDesignableProperties);
}

QListIterator<Property *> PropertyManager::allPropertiesIterator() const
{
	return QListIterator<Property *>(mUserProperties.keys());
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
	foreach (Property *property, mDesignableProperties) {
		QVariant const value = propertyValue(property->name());
		if (!value.isNull() && value.isValid()) {
			property->qtProperty()->setValue(value);
		}
	}
}

void PropertyManager::generateXml(QDomElement &element, QDomDocument &document
		, QMap<QString, QString> const &outerBindings)
{
	QListIterator<Property *> it = allPropertiesIterator();
	while (it.hasNext()) {
		Property *property = it.next();
		QDomElement propertyElement = document.createElement("property");
		propertyElement.setAttribute("propertyName", property->name());
		QStringList const propertyBindings = outerBindings.values(property->name());
		foreach (QString const &binding, propertyBindings) {
			QDomElement bindingElement = document.createElement("outerBinding");
			bindingElement.setAttribute("target", binding);
			propertyElement.appendChild(bindingElement);
		}
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

void PropertyManager::deserializeProperty(QDomElement const &element
		, QMap<QString, QString> &outerBindings)
{
	mDeserializing = true;
	QString const name = element.attribute("propertyName", "!!%%invalid%%!!");
	if (name == "!!%%invalid%%!!") {
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
	QDomElement child = element.firstChildElement("outerBinding");
	while (!child.isNull()) {
		outerBindings.insertMulti(name, child.attribute("target"));
		child = child.nextSiblingElement("outerBinding");
	}
	onPropertyChanged(name, value);
	mDeserializing = false;
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
	if (mFlagProperties.contains(name)) {
		// TODO: Add QtDesigner Alignment-like handling
		metaProperty.write(mTool, value);
	} else if (mEnumProperties.contains(name)) {
		if (mDeserializing) {
			metaProperty.write(mTool, value.value<int>());
		} else {
			metaProperty.write(mTool, mEnumProperties[name][value.value<int>()]);
		}
	} else {
		metaProperty.write(mTool, value);
	}
}

Property *PropertyManager::findProperty(const QString &name)
{
	foreach (Property *property, mUserProperties.keys()) {
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
	return mUserProperties[property];
}
