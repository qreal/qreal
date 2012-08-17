#include <QtGui/QColor>
#include <QtGui/QGraphicsColorizeEffect>

#include "diffScene.h"
#include "diffColorPreferencesDialog.h"
#include "../model/diffState.h"
#include "../../../../qrkernel/settingsManager.h"


using namespace versioning;
using namespace versioning::details;

DiffScene::DiffScene(QObject *parent)
	: EditorViewScene(parent)
{
	mAddedRemovedColor = SettingsManager::value("diffAddedRemovedColor"
		, ui::DiffColorPreferencesDialog::defaultAddedRemovedColor()).value<QColor>();
	mModifiedColor = SettingsManager::value("diffModifiedColor"
		, ui::DiffColorPreferencesDialog::defaultModifiedColor()).value<QColor>();
	mHintColor = SettingsManager::value("diffHintColor"
		, ui::DiffColorPreferencesDialog::defaultHintColor()).value<QColor>();
}

void DiffScene::initForDiff()
{
	foreach(QGraphicsItem *item, items()) {
		item->setEnabled(false);
	}
}

void DiffScene::highlight(const qReal::Id &graphicalId, DiffState const state)
{
	if (Same == state) {
		dehighlight(graphicalId);
		return;
	}
	QColor const color = Modified == state
			? mModifiedColor : mAddedRemovedColor;
	EditorViewScene::highlight(graphicalId, false, color);
}

void DiffScene::hint(const qReal::Id &graphicalId)
{
	EditorViewScene::highlight(graphicalId, false, mHintColor);
}

void DiffScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
}

void DiffScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
}

void DiffScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
}

void DiffScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
}

void DiffScene::keyPressEvent(QKeyEvent *event)
{
	event->ignore();
}

void DiffScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	event->accept();
}

void DiffScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	event->accept();
}

void DiffScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	event->accept();
}

void DiffScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseDoubleClickEvent(event);
	event->accept();
}
