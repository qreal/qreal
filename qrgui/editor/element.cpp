#include "element.h"
#include <QtGui/QKeyEvent>

#include <QtWidgets/QGraphicsColorizeEffect>

#include "models/commands/changePropertyCommand.h"

using namespace qReal;

qreal const disabledEffectStrength = 0.9;

Element::Element(ElementImpl *elementImpl
		, Id const &id
		, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
		, qReal::models::LogicalModelAssistApi &logicalAssistApi
		)
	: mMoving(false)
	, mEnabled(true)
	, mId(id)
	, mElementImpl(elementImpl)
	, mLogicalAssistApi(logicalAssistApi)
	, mGraphicalAssistApi(graphicalAssistApi)
	, mController(nullptr)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable | ItemClipsChildrenToShape |
			ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptDrops(true);
	setCursor(Qt::PointingHandCursor);

	updateEnabledState();
}

Id Element::id() const
{
	return mId;
}

qReal::Id Element::logicalId() const
{
	return mGraphicalAssistApi.logicalId(mId);
}

QString Element::name() const
{
	return mGraphicalAssistApi.name(id());
}

void Element::updateData()
{
	setToolTip(mGraphicalAssistApi.toolTip(id()));
}

QList<ContextMenuAction*> Element::contextMenuActions(const QPointF &pos)
{
	Q_UNUSED(pos)
	return QList<ContextMenuAction*>();
}

QString Element::logicalProperty(QString const &roleName) const
{
	return mLogicalAssistApi.propertyByRoleName(logicalId(), roleName).toString();
}

void Element::setLogicalProperty(QString const &roleName, QString const &value, bool withUndoRedo)
{
	commands::AbstractCommand *command = new commands::ChangePropertyCommand(&mLogicalAssistApi
			, roleName, logicalId(), value);
	if (withUndoRedo) {
		mController->execute(command);
	} else {
		command->redo();
		delete command;
	}
}

void Element::setController(Controller *controller)
{
	mController = controller;
}

qReal::Controller * Element::controller() const
{
	return mController;
}

void Element::initTitles()
{
}

void Element::select(const bool singleSelected)
{
	if (singleSelected) {
		setSelectionState(true);
	}

	emit switchFolding(!singleSelected);
}

void Element::setSelectionState(const bool selected)
{
	if (isSelected() != selected) {
		setSelected(selected);
	}
	if (!selected) {
		select(false);
	}

	foreach (Label * const label, mLabels) {
		label->setParentSelected(selected);
	}
}

ElementImpl* Element::elementImpl() const
{
	return mElementImpl;
}

bool Element::createChildrenFromMenu() const
{
	return mElementImpl->createChildrenFromMenu();
}

void Element::updateEnabledState()
{
	bool const enabled = mLogicalAssistApi.editorManagerInterface().elements(
			Id(mId.editor(), mId.diagram())).contains(mId.type());

	mEnabled = enabled;
	if (mEnabled) {
		setGraphicsEffect(nullptr);
		setOpacity(1);
	} else {
		QGraphicsColorizeEffect * const grayScale = new QGraphicsColorizeEffect(this);
		grayScale->setColor(Qt::gray);
		grayScale->setStrength(disabledEffectStrength);
		setGraphicsEffect(grayScale);
		setOpacity(disabledEffectStrength);
	}
}

void Element::setTitlesVisible(bool visible)
{
	mTitlesVisible = visible;
	setTitlesVisiblePrivate(visible);
}

void Element::setTitlesVisiblePrivate(bool visible)
{
	foreach (Label const * const label, mLabels) {
		if (label->isSelected()) {
			return;
		}
	}

	foreach (Label * const label, mLabels) {
		label->setVisible(label->isHard() || visible);
	}
}

void Element::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_F2) {
		for (Label * const label : mLabels) {
			if (!label->isReadOnly()) {
				label->startTextInteraction();
				return;
			}
		}
	}
}
