#include "draggableElement.h"

#include <QtCore/QUuid>
#include <QtGui/QMouseEvent>
#include <QtCore/QtAlgorithms>
#include <QtWidgets/QVBoxLayout>

#include <qrkernel/settingsManager.h>
#include <qrkernel/definitions.h>

#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/paletteTree.h"
#include "dialogs/metamodelingOnFly/propertiesDialog.h"
#include "view/editorView.h"
#include "view/editorViewScene.h"

using namespace qReal;
using namespace gui;

DraggableElement::DraggableElement(
		MainWindow &mainWindow
		, PaletteElement const &data
		, bool iconsOnly
		, EditorManagerInterface &editorManagerProxy
		, QWidget *parent
		)
	: QWidget(parent)
	, mData(data)
	, mEditorManagerProxy(editorManagerProxy)
	, mMainWindow(mainWindow)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 4, 0, 4);

	int const size = iconsOnly ? 50 : 30;
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
	QString modifiedDescription = mData.description();
	if (!modifiedDescription.isEmpty()) {
		modifiedDescription.insert(0, "<body>");  //turns alignment on
		setToolTip(modifiedDescription);
	}
	setCursor(Qt::OpenHandCursor);

	setAttribute(Qt::WA_AcceptTouchEvents);
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

void DraggableElement::changePropertiesPaletteActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id id = action->data().value<Id>();
	PropertiesDialog *propDialog = new PropertiesDialog(mMainWindow, mEditorManagerProxy, id);
	propDialog->setModal(true);
	propDialog->show();
}

void DraggableElement::changeAppearancePaletteActionTriggered()
{
	QAction const * const action = static_cast<QAction *>(sender());
	Id const id = action->data().value<Id>();
	QString const propertyValue = mEditorManagerProxy.shape(id);
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
	mEditorManagerProxy.deleteElement(&mMainWindow, mDeletedElementId);
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
	IdList const children = mEditorManagerProxy.children(mDeletedElementId);
	if (!children.isEmpty()) {
		QString childrenNames;
		foreach (Id const child, children) {
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

bool DraggableElement::event(QEvent *event)
{
	QTouchEvent *touchEvent = dynamic_cast<QTouchEvent *>(event);
	if (!touchEvent || touchEvent->touchPoints().count() != 1) {
		return QWidget::event(event);
	}

	QPoint const pos(touchEvent->touchPoints()[0].pos().toPoint());

	switch(event->type()) {
	case QEvent::TouchBegin: {
		QMouseEvent* mouseEvent = new QMouseEvent(QEvent::MouseButtonPress, pos
				, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
		QApplication::postEvent(touchEvent->target(), mouseEvent);
		break;
	}
	case QEvent::TouchEnd: {
		QMouseEvent* mouseEvent = new QMouseEvent(QEvent::MouseButtonRelease, pos
				, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
		QApplication::postEvent(touchEvent->target(), mouseEvent);
		break;
	}
	case QEvent::TouchUpdate: {
		// TODO: This must move mouse cursor but no effect...
		QCursor::setPos(mapToGlobal(pos));
		break;
	}
	default:
		break;
	}
	event->accept();
	return true;
}

void DraggableElement::mousePressEvent(QMouseEvent *event)
{
	QWidget *atMouse = childAt(event->pos());
	if (!atMouse || atMouse == this) {
		return;
	}

	DraggableElement *child = dynamic_cast<DraggableElement *>(atMouse->parent());
	if (!child) {
		child = dynamic_cast<DraggableElement *>(atMouse);
	}
	if (!child) {
		return;
	}

	Q_ASSERT(child->id().idSize() == 3);  // it should be element type

	// new element's ID is being generated here
	// may this epic event should take place in some more appropriate place

	Id elementId(child->id(), QUuid::createUuid().toString());

	if (event->button() == Qt::RightButton) {
		if (mEditorManagerProxy.isInterpretationMode()) {
			QMenu *menu = new QMenu();
			QAction * const changePropertiesPaletteAction = menu->addAction(tr("Change Properties"));
			connect(changePropertiesPaletteAction, SIGNAL(triggered()), SLOT(changePropertiesPaletteActionTriggered()));
			changePropertiesPaletteAction->setData(elementId.toVariant());
			QAction * const changeAppearancePaletteAction = menu->addAction(tr("Change Appearance"));
			connect(changeAppearancePaletteAction, SIGNAL(triggered()), SLOT(changeAppearancePaletteActionTriggered()));
			changeAppearancePaletteAction->setData(elementId.toVariant());
			QAction * const deleteElementPaletteAction = menu->addAction(tr("Delete Element"));
			connect(deleteElementPaletteAction, SIGNAL(triggered()), SLOT(deleteElementPaletteActionTriggered()), Qt::QueuedConnection);
			deleteElementPaletteAction->setData(elementId.toVariant());
			menu->exec(QCursor::pos());
		}
	} else {
		QByteArray itemData;
		bool isFromLogicalModel = false;

		QDataStream stream(&itemData, QIODevice::WriteOnly);
		stream << elementId.toString();  // uuid
		stream << Id::rootId().toString();  // pathToItem
		stream << QString("(" + child->text() + ")");
		stream << QPointF(0, 0);
		stream << isFromLogicalModel;
		stream << mData.explosionTarget().toString();

		QMimeData *mimeData = new QMimeData;
		mimeData->setData("application/x-real-uml-data", itemData);

		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);

		QPixmap const pixmap = child->icon().pixmap(mData.preferredSize());

		if (!pixmap.isNull()) {
			drag->setPixmap(pixmap);
		}

		if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction) {
			child->close();
		} else {
			child->show();
		}
	}
}
