#include "nodeElementLayoutFactory.h"

using namespace qReal::layouts;

NodeElementLayoutFactory::NodeElementLayoutFactory(const QString &bindingTarget
		, QGraphicsWidget *layoutHost)
	: LayoutHandlerFactory(layoutHost)
	, mBinding(bindingTarget)
	, mForestallingSize(4, 0)
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

void NodeElementLayoutFactory::setEnumValues(QList<QPair<QString, QString>> const &values)
{
	Q_UNUSED(values)
}

void NodeElementLayoutFactory::configure(ElementImpl *const impl)
{
	if (impl->isSortingContainer()) {
		forceChindrenSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	}
	mForestallingSize = impl->sizeOfForestalling();
	int const outerMargin = mForestallingSize[0];//left forestalling was implicated by old 'forestalling'?
	int const innerMargin = impl->sizeOfChildrenForestalling();
	if (outerMargin) {
		setOuterMargin(outerMargin, upperMargin(outerMargin)
			, outerMargin, outerMargin);
	}
	if (innerMargin) {
		setLayoutMargin(0, innerMargin/2, 0, innerMargin/2);
	}
	setPropertyValue(impl->layout());
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

void NodeElementLayoutFactory::processBeforeFirstPlacing(QGraphicsItem *element)
{
	//TODO ask what was meant by old forestalling???
	int legacyForstallingGuess = mForestallingSize[0];
	element->setPos(legacyForstallingGuess, upperMargin(legacyForstallingGuess));
}

int NodeElementLayoutFactory::upperMargin(int const baseMargin) const
{
	return baseMargin ? baseMargin + titlePadding : 0;
}
