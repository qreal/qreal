#include "diffScene.h"
#include "diffColorPreferencesDialog.h"
#include "../details/diffState.h"

#include <QSettings>
#include <QGraphicsColorizeEffect>

using namespace qReal::diffManager::diffView;
using namespace qReal::diffManager::details;

DiffScene::DiffScene(QObject *parent)
	: EditorViewScene(parent)
{
	QSettings settings("SPbSU", "QReal");
	mAddedRemovedColor = settings.value("diffAddedRemovedColor", DiffColorPreferencesDialog::defaultAddedRemovedColor()).value<QColor>();
	mModifiedColor = settings.value("diffModifiedColor", DiffColorPreferencesDialog::defaultModifiedColor()).value<QColor>();
	mHintColor = settings.value("diffHintColor", DiffColorPreferencesDialog::defaultHintColor()).value<QColor>();
}

void DiffScene::initForDiff()
{
	QList < QGraphicsItem * > list = items();
	for (QList < QGraphicsItem * >::Iterator it = list.begin(); it != list.end(); it++)
	{
		if (UML::Element * elem = dynamic_cast < UML::Element * >(*it))
		{
			elem->setEnabled(false);
		}
	}
}

void DiffScene::highlight(const qReal::Id &graphicalId, DiffState state)
{
	UML::Element *elem = getElem(graphicalId);
	if (!elem)
		return;
	if (qReal::diffManager::details::Same == state)
	{
		elem->setGraphicsEffect(NULL);
		return;
	}
	QColor color = (Modified == state) ? mModifiedColor : mAddedRemovedColor;
	highlight(elem, color);
}

void DiffScene::hint(const qReal::Id &graphicalId)
{
	UML::Element *elem = getElem(graphicalId);
	if (!elem)
		return;

	highlight(elem, mHintColor);
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

void DiffScene::highlight(QGraphicsItem *elem, const QColor &color)
{
	QList<QGraphicsEffect *> childEffects;
	foreach (QGraphicsItem *item, elem->childItems())
	{
		childEffects << item->graphicsEffect();
	}
	QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect;
	effect->setEnabled(true);
	effect->setColor(color);
	elem->setGraphicsEffect(effect);
	int i = 0;
	foreach (QGraphicsItem *item, elem->childItems())
	{
		item->setGraphicsEffect(childEffects[i++]);
	}
}
