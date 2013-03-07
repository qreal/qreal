#include "nodeElementLayoutFactory.h"

using namespace qReal::layouts;

NodeElementLayoutFactory::NodeElementLayoutFactory(const QString &bindingTarget
		, QGraphicsWidget *layoutHost)
	: LayoutHandlerFactory(layoutHost), mBinding(bindingTarget)
{
}

NodeElementLayoutFactory::~NodeElementLayoutFactory()
{
}

QString NodeElementLayoutFactory::binding() const
{
	return mBinding;
}

void NodeElementLayoutFactory::setPropertyValue(QVariant const &value)
{
	setType(stringToType(value.toString()));
}

void NodeElementLayoutFactory::setEnumValues(QStringList const &values)
{
	Q_UNUSED(values)
}

LayoutType NodeElementLayoutFactory::stringToType(QString const &stringType)
{
	QString const normalizedType = stringType.toLower().trimmed();
	if (normalizedType.startsWith("no")) {
		return none;
	} else if (normalizedType.startsWith("vert")) {
		return vertical;
	} else if (normalizedType.startsWith("horiz")) {
		return horizontal;
	} else if (normalizedType.startsWith("grid")) {
		return grid;
	}
	return none;
}
