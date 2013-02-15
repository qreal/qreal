#include "element.h"

#include <QtGui>

using namespace qReal;

Element::Element(ElementImpl* elementImpl)
	: QGraphicsProxyWidget(), mMoving(false)
	, mElementImpl(elementImpl)
	, mLogicalAssistApi(NULL)
	, mGraphicalAssistApi(NULL)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);
	setAcceptDrops(true);
	setCursor(Qt::PointingHandCursor);
}

void Element::setId(qReal::Id &id)
{
	mId = id;
	update();
}

Id Element::id() const
{
	return mId;
}

qReal::Id Element::logicalId() const
{
	return mGraphicalAssistApi->logicalId(mId);
}

QString Element::name() const
{
	return mGraphicalAssistApi->name(id());
}

void Element::updateData()
{
	setToolTip(mGraphicalAssistApi->toolTip(id()));
}

QList<ContextMenuAction*> Element::contextMenuActions(const QPointF &pos)
{
	Q_UNUSED(pos)
	return QList<ContextMenuAction*>();
}

QVariant Element::logicalProperty(QString const &roleName) const
{
	return mLogicalAssistApi->propertyByRoleName(logicalId(), roleName);
}

void Element::setLogicalProperty(QString const &roleName, QVariant const &value)
{
	mLogicalAssistApi->setPropertyByRoleName(logicalId(), value, roleName);
}

void Element::setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi, qReal::models::LogicalModelAssistApi *logicalAssistApi)
{
	mGraphicalAssistApi = graphicalAssistApi;
	mLogicalAssistApi = logicalAssistApi;
}

void Element::initTitlesBy(QRectF const& contents)
{
	foreach (ElementTitle * const title, mTitles) {
		title->transform(contents);
	}
}

void Element::initTitles()
{
	initTitlesBy(boundingRect().adjusted(kvadratik, kvadratik, -kvadratik, -kvadratik));
}


void Element::singleSelectionState(const bool singleSelected) {
	if (singleSelected) {
		selectionState(true);
	}
	emit switchFolding(!singleSelected);
}

void Element::selectionState(const bool selected) {
	if (isSelected() != selected) {
		setSelected(selected);
	}
	if (!selected) {
		singleSelectionState(false);
	}
}

ElementImpl* Element::elementImpl() const
{
	return mElementImpl;
}

void Element::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsProxyWidget::mousePressEvent(event);
	event->accept();
	QGraphicsItem::mousePressEvent(event);
}

void Element::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsProxyWidget::mouseMoveEvent(event);
	QGraphicsItem::mouseMoveEvent(event);
}

void Element::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsProxyWidget::mouseReleaseEvent(event);
	QGraphicsItem::mouseReleaseEvent(event);
}
