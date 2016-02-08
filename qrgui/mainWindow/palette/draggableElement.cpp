/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "draggableElement.h"

#include <QtCore/QUuid>
#include <QtCore/QtAlgorithms>
#include <QtCore/QBuffer>
#include <QtGui/QMouseEvent>
#include <QtGui/QDrag>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>

#include <qrkernel/settingsManager.h>
#include <qrkernel/definitions.h>

#include <qrgui/models/models.h>

#include "mainWindow/mainWindow.h"
#include "mainWindow/palette/paletteTree.h"
#include "dialogs/metamodelingOnFly/propertiesDialog.h"
#include "mouseGestures/gesturePainter.h"
#include "editor/editorView.h"
#include "editor/editorViewScene.h"

using namespace qReal;
using namespace gui;

const int gestureTipSize = 30;

DraggableElement::DraggableElement(
		MainWindow &mainWindow
		, const PaletteElement &data
		, bool iconsOnly
		, const EditorManagerInterface &editorManagerProxy
		, QWidget *parent
		)
	: QWidget(parent)
	, mData(data)
	, mEditorManagerProxy(editorManagerProxy)
	, mMainWindow(mainWindow)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 4, 0, 4);

	const int size = iconsOnly ? 50 : 30;
	mLabel = new QLabel(this);
	mLabel->setPixmap(mData.icon().pixmap(size - 2, size - 2));
	layout->addWidget(mLabel);

	if (!iconsOnly) {
		QLabel *text = new QLabel(this);
		text->setText(mData.name());
		layout->addWidget(text);
		layout->addStretch();
	}

	setLayout(layout);

	QString description = mData.description();
	if (!description.isEmpty()) {
		const QString rawGesture = mEditorManagerProxy.mouseGesture(data.id());
		if (!rawGesture.isEmpty() && qReal::SettingsManager::value("gesturesEnabled").toBool()) {
			const QSize size(gestureTipSize, gestureTipSize);
			gestures::GesturePainter painter(rawGesture, Qt::white, Qt::blue, gestureTipSize);
			const QPixmap gesture = painter.pixmap(size, QIcon::Mode::Normal, QIcon::State::Off);
			QByteArray byteArray;
			QBuffer buffer(&byteArray);
			gesture.save(&buffer, "PNG");
			const QString gestureDescription = tr("Mouse gesture");
			description += QString("<br><table><tr><td valign='middle'>%1:&nbsp;&nbsp;&nbsp;</td>"\
					"<td><img src=\"data:image/png;base64,%2\"/></td></tr></table>")
							.arg(gestureDescription, QString(byteArray.toBase64()));
		}

		setToolTip(QString("<body>%1</body>").arg(description));
	}

	setCursor(Qt::OpenHandCursor);
	setAttribute(Qt::WA_AcceptTouchEvents);
	setObjectName(mData.name());
}

QIcon DraggableElement::icon() const
{
	return mData.icon();
}

QString DraggableElement::text() const
{
	return mData.name();
}

Id DraggableElement::id() const
{
	return mData.id();
}

Id DraggableElement::explosionTarget() const
{
	return mData.explosionTarget();
}

QSize DraggableElement::iconsPreferredSize() const
{
	return mData.preferredSize();
}

void DraggableElement::setIconSize(int size)
{
	mLabel->setPixmap(mData.icon().pixmap(size , size));
}

QMimeData *DraggableElement::mimeData(const Id &elementId) const
{
	const bool isEdge = mEditorManagerProxy.isNodeOrEdge(elementId.editor(), elementId.element()) < 0;
	return ElementInfo(elementId, Id(), text(), mData.explosionTarget(), isEdge).mimeData();
}

void DraggableElement::changePropertiesPaletteActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id id = action->data().value<Id>();
	PropertiesDialog *propDialog = new PropertiesDialog(mEditorManagerProxy
			, mMainWindow.models().mutableLogicalRepoApi(), id, &mMainWindow);
	propDialog->setModal(true);
	propDialog->show();
}

void DraggableElement::changeAppearancePaletteActionTriggered()
{
	const QAction * const action = static_cast<QAction *>(sender());
	const Id id = action->data().value<Id>();
	const QString propertyValue = mEditorManagerProxy.shape(id);
	mMainWindow.openShapeEditor(id, propertyValue, &mEditorManagerProxy, false);
}

void DraggableElement::deleteElementPaletteActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	mDeletedElementId = action->data().value<Id>();
	QMessageBox messageBox(
			tr("Deleting an element: ") + mEditorManagerProxy.friendlyName(mDeletedElementId)
			, tr("Do you really want to delete this item and all its graphical"
					"representation from the scene and from the palette?")
			, QMessageBox::Warning
			, QMessageBox::Ok
			, QMessageBox::Cancel
			, QMessageBox::NoButton
			);

	messageBox.button(QMessageBox::Ok)->setText(tr("Yes"));
	messageBox.button(QMessageBox::Cancel)->setText(tr("No"));
	if (messageBox.exec() == QMessageBox::Ok) {
		checkElementForChildren();
	}
}

void DraggableElement::deleteElement()
{
	mMainWindow.clearSelectionOnTabs();
	if (mIsRootDiagramNode) {
		mMainWindow.closeDiagramTab(mDeletedElementId);
	}

	mEditorManagerProxy.deleteElement(mDeletedElementId);
	/// @todo: Maybe we do not need to remove elements if we can restore them?
	/// We can make elements grayscaled by disabling corresponding element in palette.
	const IdList logicalIdList = mMainWindow.models().logicalRepoApi().logicalElements(mDeletedElementId.type());
	for (const Id &logicalId : logicalIdList) {
		const QModelIndex index = mMainWindow.models().logicalModelAssistApi().indexById(logicalId);
		mMainWindow.models().logicalModel()->removeRow(index.row(), index.parent());
	}

	mMainWindow.loadPlugins();
}

void DraggableElement::checkElementForRootDiagramNode()
{
	if (mEditorManagerProxy.isRootDiagramNode(mDeletedElementId)) {
		mIsRootDiagramNode = true;
		QMessageBox messageBox(
				tr("Warning")
				, tr("The deleted element ") + mEditorManagerProxy.friendlyName(mDeletedElementId)
						+ tr(" is the element of root digram. Continue to delete?")
				, QMessageBox::Warning
				, QMessageBox::Ok
				, QMessageBox::Cancel
				, QMessageBox::NoButton
				);

		messageBox.button(QMessageBox::Ok)->setText(tr("Yes"));
		messageBox.button(QMessageBox::Cancel)->setText(tr("No"));
		if (messageBox.exec() == QMessageBox::Ok) {
			deleteElement();
		}
	} else {
		deleteElement();
	}
}

void DraggableElement::checkElementForChildren()
{
	mIsRootDiagramNode = false;
	const IdList children = mEditorManagerProxy.children(mDeletedElementId);
	if (!children.isEmpty()) {
		QString childrenNames;
		foreach (const Id child, children) {
			childrenNames += " " + mEditorManagerProxy.friendlyName(child) + ",";
		}
		if (!childrenNames.isEmpty()) {
			childrenNames.replace(childrenNames.length() - 1, 1, ".");
		}

		QMessageBox messageBox(
				tr("Warning")
				, tr("The deleted element ")
						+ mEditorManagerProxy.friendlyName(mDeletedElementId)
						+ tr(" has inheritors:")
						+ childrenNames
						+ "\n"
						+ tr("If you delete it, its properties will be removed from"
								"the elements-inheritors. Continue to delete?")
				, QMessageBox::Warning
				, QMessageBox::Ok
				, QMessageBox::Cancel
				, QMessageBox::NoButton
				);

		messageBox.button(QMessageBox::Ok)->setText(tr("Yes"));
		messageBox.button(QMessageBox::Cancel)->setText(tr("No"));
		if (messageBox.exec() == QMessageBox::Ok) {
			checkElementForRootDiagramNode();
		}
	} else {
		checkElementForRootDiagramNode();
	}
}

void DraggableElement::hackTouchDrag()
{
#ifdef Q_OS_WIN
	HackTouchDragThread *thread = new HackTouchDragThread(this);
	thread->start(QThread::LowestPriority);
#endif
}

bool DraggableElement::event(QEvent *event)
{
	QTouchEvent *touchEvent = dynamic_cast<QTouchEvent *>(event);
	if (!touchEvent || touchEvent->touchPoints().count() != 1) {
		return QWidget::event(event);
	}

	const QPoint pos(touchEvent->touchPoints()[0].pos().toPoint());

	switch(event->type()) {
	case QEvent::TouchBegin: {
		QCursor::setPos(mapToGlobal(pos));
		QMouseEvent* mouseEvent = new QMouseEvent(QEvent::MouseButtonPress, pos
				, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
		QApplication::postEvent(touchEvent->target(), mouseEvent);
		hackTouchDrag();
		break;
	}
	case QEvent::TouchEnd: {
		QMouseEvent* mouseEvent = new QMouseEvent(QEvent::MouseButtonRelease, pos
				, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
		QApplication::postEvent(touchEvent->target(), mouseEvent);
#ifdef Q_OS_WIN
		HackTouchDragThread::simulateSystemRelease();
#endif
		break;
	}
	case QEvent::TouchUpdate: {
		QCursor::setPos(mapToGlobal(pos));
		break;
	}
	default:
		break;
	}

	return true;
}

void DraggableElement::mousePressEvent(QMouseEvent *event)
{
	Q_ASSERT(id().idSize() == 3);  // it should be element type

	// new element's ID is being generated here
	// may this epic event should take place in some more appropriate place

	const Id elementId = id().sameTypeId();

	if (event->button() == Qt::RightButton) {
		if (mEditorManagerProxy.isInterpretationMode()) {
			QMenu *menu = new QMenu();
			QAction * const changePropertiesPaletteAction = menu->addAction(tr("Change Properties"));
			connect(changePropertiesPaletteAction, &QAction::triggered
					, this, &DraggableElement::changePropertiesPaletteActionTriggered);
			changePropertiesPaletteAction->setData(elementId.toVariant());

			QAction * const changeAppearancePaletteAction = menu->addAction(tr("Change Appearance"));
			connect(changeAppearancePaletteAction, &QAction::triggered
					, this,  &DraggableElement::changeAppearancePaletteActionTriggered);
			changeAppearancePaletteAction->setData(elementId.toVariant());

			QAction * const deleteElementPaletteAction = menu->addAction(tr("Delete Element"));
			connect(deleteElementPaletteAction, &QAction::triggered
					, this, &DraggableElement::deleteElementPaletteActionTriggered
					, Qt::QueuedConnection);
			deleteElementPaletteAction->setData(elementId.toVariant());

			auto additionalMenuActions = mMainWindow.optionalMenuActionsForInterpretedPlugins();
			if (!additionalMenuActions.isEmpty()) {
				menu->addActions(additionalMenuActions);

				for (QAction *action : additionalMenuActions) {
					action->setData(elementId.toVariant());
				}
			}

			menu->exec(QCursor::pos());
		}
	} else {
		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData(elementId));

		const QPixmap pixmap = icon().pixmap(mData.preferredSize());

		if (!pixmap.isNull()) {
			drag->setPixmap(pixmap);
		}

		drag->exec(Qt::CopyAction);
	}
}

#ifdef Q_OS_WIN

#include <windows.h>
#include <winuser.h>

DraggableElement::HackTouchDragThread::HackTouchDragThread(QObject *parent)
	: QThread(parent)
{
}

void DraggableElement::HackTouchDragThread::simulateSystemPress()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void DraggableElement::HackTouchDragThread::simulateSystemMove()
{
	mouse_event(MOUSEEVENTF_MOVE, -1, -1, 0, 0);
}

void DraggableElement::HackTouchDragThread::simulateSystemRelease()
{
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void DraggableElement::HackTouchDragThread::run()
{
	// Simulating press for windows drag manager not to terminate drag as inconsistent
	// when it would be unfrozen
	simulateSystemPress();
	// Actually sheduller can move mouse till the drag is started so repeating it for sufficient times
	for (int i = 0; i < 10; ++i) {
		// This will unfreeze windows drag manager when drag instance is created but mouse didn`t move
		simulateSystemMove();
		QThread::msleep(3);
	}
}
#endif
