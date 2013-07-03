#include <QtCore/QUuid>
#include <QtGui/QMouseEvent>
#include <QtCore/QtAlgorithms>
#include <QtWidgets/QVBoxLayout>

#include "draggableElement.h"
#include "../mainWindow.h"
#include "paletteTree.h"
#include "../../qrkernel/settingsManager.h"
#include "../dialogs/metamodelingOnFly/propertiesDialog.h"
#include "../../qrkernel/definitions.h"
#include "../view/editorView.h"
#include "../view/editorViewScene.h"

using namespace qReal;
using namespace gui;

DraggableElement::DraggableElement(
		MainWindow &mainWindow
		, Id const &id
		, QString const &name
		, QString const &description
		, QIcon const &icon
		, QSize const &preferredSize
		, bool iconsOnly
		, EditorManagerInterface &editorManagerProxy
		, QWidget *parent
		)
	: QWidget(parent)
	, mId(id)
	, mIcon(icon)
	, mPreferredSize(preferredSize)
	, mText(name)
	, mEditorManagerProxy(editorManagerProxy)
	, mMainWindow(mainWindow)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 4, 0, 4);

	int const size = iconsOnly ? 50 : 30;
	mLabel = new QLabel(this);
	mLabel->setPixmap(mIcon.pixmap(size - 2, size - 2));
	layout->addWidget(mLabel);
	if (!iconsOnly) {
		QLabel *text = new QLabel(this);
		text->setText(mText);
		layout->addWidget(text);
		layout->addStretch();
	}

	setLayout(layout);
	QString modifiedDescription = description;
	if (!modifiedDescription.isEmpty()) {
		modifiedDescription.insert(0, "<body>");  //turns alignment on
		setToolTip(modifiedDescription);
	}
	setCursor(Qt::OpenHandCursor);
}

QIcon DraggableElement::icon() const
{
	return mIcon;
}

QString DraggableElement::text() const
{
	return mText;
}

Id DraggableElement::id() const
{
	return mId;
}

QSize DraggableElement::iconsPreferredSize() const
{
	return mPreferredSize;
}

void DraggableElement::setIconSize(int size)
{
	mLabel->setPixmap(mIcon.pixmap(size , size));
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
	mMainWindow.openShapeEditor(id, propertyValue, &mEditorManagerProxy);
}

void DraggableElement::deleteElementPaletteActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	mDeletedElementId = action->data().value<Id>();
	QMessageBox messageBox(
			tr("Deleting an element: ") + mEditorManagerProxy.friendlyName(mDeletedElementId)
			, tr("Do you really want to delete this item and all its graphical representation from the scene and from the palette?")
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
				, tr("The deleted element ") + mEditorManagerProxy.friendlyName(mDeletedElementId) + tr(" is the element of root digram. Continue to delete?")
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
						+ tr("If you delete it, its properties will be removed from the elements-inheritors. Continue to delete?")
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
			QMenu menu;
			QAction * const changePropertiesPaletteAction = menu.addAction(tr("Change Properties"));
			connect(changePropertiesPaletteAction, SIGNAL(triggered()), SLOT(changePropertiesPaletteActionTriggered()));
			changePropertiesPaletteAction->setData(elementId.toVariant());
			QAction * const changeAppearancePaletteAction = menu.addAction(tr("Change Appearance"));
			connect(changeAppearancePaletteAction, SIGNAL(triggered()), SLOT(changeAppearancePaletteActionTriggered()));
			changeAppearancePaletteAction->setData(elementId.toVariant());
			QAction * const deleteElementPaletteAction = menu.addAction(tr("Delete Element"));
			connect(deleteElementPaletteAction, SIGNAL(triggered()), SLOT(deleteElementPaletteActionTriggered()));
			deleteElementPaletteAction->setData(elementId.toVariant());
			menu.exec(QCursor::pos());
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

		QMimeData *mimeData = new QMimeData;
		mimeData->setData("application/x-real-uml-data", itemData);

		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);

		QPixmap p = child->icon().pixmap(96, 96);

		if (!p.isNull()) {
			drag->setPixmap(child->icon().pixmap(96, 96));
		}

		if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction) {
			child->close();
		} else {
			child->show();
		}
	}
}
