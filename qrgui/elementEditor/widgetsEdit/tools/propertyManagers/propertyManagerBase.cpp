#include "propertyManagerBase.h"
#include "../../../../../qrutils/xmlUtils.h"

using namespace qReal::widgetsEdit;

PropertyManagerBase::PropertyManagerBase(Tool *tool)
	: mTool(tool), mManager(new QtVariantPropertyManager)
{
	connect(mManager, SIGNAL(valueChanged(QtProperty*,QVariant))
		, this, SLOT(propertyChanged(QtProperty*,QVariant)));
}

QListIterator<Property *> PropertyManagerBase::propertiesIterator() const
{
	return QListIterator<Property *>(mProperties);
}

QtVariantPropertyManager *PropertyManagerBase::qtPropertyManager() const
{
	return mManager;
}

void PropertyManagerBase::changeProperty(const QString &name, const QVariant &value)
{
	Property *property = findProperty(name);
	if (!property) {
		return;
	}
	QtVariantProperty *qtProperty =
		dynamic_cast<QtVariantProperty *>(property->qtProperty());
	if (!qtProperty) {
		return;
	}
	QVariant const oldValue = qtProperty->value();
	if (oldValue != value) {
		qtProperty->setValue(value);
	}
}

void PropertyManagerBase::onSelect()
{
	foreach(Property *property, mProperties) {
		QVariant const value = propertyValue(property->name());
		if (!value.isNull() && value.isValid()) {
			property->qtProperty()->setValue(value);
		}
	}
}

void PropertyManagerBase::generateXml(QDomElement &element, QDomDocument &document)
{
	QListIterator<Property *> *it = new QListIterator<Property *>(mProperties);
	while (it->hasNext()) {
		Property *property = it->next();
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

void PropertyManagerBase::deserializeProperty(QDomElement const &element)
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

void PropertyManagerBase::propertyChanged(QtProperty *property
		, const QVariant &value)
{
	onPropertyChanged(property->propertyName(), value);
}

void PropertyManagerBase::addProperty(const QString &name, int type)
{
	QtVariantProperty *qtProperty = mManager->addProperty(type, name);
	Property *property = new Property(name, type, qtProperty);
	mProperties.append(property);
}

void PropertyManagerBase::addEnumProperty(const QString &name
		, const QStringList &types)
{
	int const type = QtVariantPropertyManager::enumTypeId();
	QtVariantProperty *qtProperty = mManager->addProperty(type, name);
	qtProperty->setAttribute("enumNames", types);
	Property *property = new Property(name, type, qtProperty);
	mProperties.append(property);
}

void PropertyManagerBase::onPropertyChanged(const QString &name
		, const QVariant &value)
{
	Q_UNUSED(name)
	Q_UNUSED(value)
}

Property *PropertyManagerBase::findProperty(const QString &name)
{
	foreach (Property *property, mProperties) {
		if (property->name() == name) {
			return property;
		}
	}
	return NULL;
}
