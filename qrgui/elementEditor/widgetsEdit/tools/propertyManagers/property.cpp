#include "property.h"

using namespace Ui::WidgetsEdit;

Property::Property(const QString &name, int type, QtVariantProperty *qtProperty)
	: mName(name), mType(type), mQtProperty(qtProperty)
{
}

QString Property::name() const
{
	return mName;
}

void Property::setName(const QString &name)
{
	mName = name;
}

int Property::type() const
{
	return mType;
}

void Property::setType(int type)
{
	mType = type;
}

QtVariantProperty *Property::qtProperty() const
{
	return mQtProperty;
}

void Property::setQtProperty(QtVariantProperty *qtProperty)
{
	mQtProperty = qtProperty;
}
