#include <QtCore/QUuid>
#include <QtGui/QMouseEvent>
#include <QtCore/QtAlgorithms>
#include <QtGui/QVBoxLayout>

#include "draggableElement.h"
#include "mainWindow.h"
#include "paletteTree.h"
#include "../../qrkernel/settingsManager.h"
#include "../dialogs/propertiesDialog.h"
#include "../../qrkernel/definitions.h"
#include "../view/editorView.h"
#include "../view/editorViewScene.h"

using namespace qReal;
using namespace gui;

DraggableElement::DraggableElement(MainWindow *mainWindow, const Id &id, const QString &name
		, const QString &description, const QIcon &icon, bool iconsOnly, QWidget *parent)
		: QWidget(parent)
		, mId(id)
		, mIcon(icon)
		, mText(name)
		, mMainWindow(mainWindow)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 4, 0, 4);

	const int size = iconsOnly ? 50 : 30;
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

void DraggableElement::setIconSize(int size)
{
	mLabel->setPixmap(mIcon.pixmap(size , size));
}

void DraggableElement::setEditorManagerProxy(EditorManagerInterface *editorManagerProxy)
{
	mEditorManagerProxy = editorManagerProxy;
}

void DraggableElement::changePropertiesPaletteActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id id = action->data().value<Id>();
	PropertiesDialog *propDialog = new PropertiesDialog(mMainWindow);
	propDialog->init(mEditorManagerProxy, id);
	propDialog->setModal(true);
	propDialog->show();
}

void DraggableElement::changeAppearancePaletteActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id id = action->data().value<Id>();
	QString propertyValue = mEditorManagerProxy->getShape(id);
	mMainWindow->openShapeEditor(id, propertyValue, mEditorManagerProxy);
}

void DraggableElement::deleteElementPaletteActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	mDeletedElementId = action->data().value<Id>();
	QMessageBox *mb = new QMessageBox(tr("Deleting an element: ") + mEditorManagerProxy->friendlyName(mDeletedElementId), tr("Do you really want to delete this item and all its graphical representation from the scene and from the palette?"), QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
	mb->button(QMessageBox::Ok)->setText(tr("Yes"));
	mb->button(QMessageBox::Cancel)->setText(tr("No"));
	mb->show();
	connect(mb->button(QMessageBox::Ok), SIGNAL(clicked()), this, SLOT(checkElementForChildren()));
}

void DraggableElement::deleteElement()
{
	mMainWindow->clearSelectionOnTabs();
	if (mIsRootDiagramNode) {
		mMainWindow->closeDiagramTab(mDeletedElementId);
	}
	mEditorManagerProxy->deleteElement(mMainWindow, mDeletedElementId);
	mMainWindow->loadPlugins();
}

void DraggableElement::checkElementForRootDiagramNode()
{
	if (mEditorManagerProxy->isRootDiagramNode(mDeletedElementId)) {
		mIsRootDiagramNode = true;
		QMessageBox *mb = new QMessageBox(tr("Warning"), tr("The deleted element ") + mEditorManagerProxy->friendlyName(mDeletedElementId) + tr(" is the element of root digram. Continue to delete?"), QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
		mb->button(QMessageBox::Ok)->setText(tr("Yes"));
		mb->button(QMessageBox::Cancel)->setText(tr("No"));
		mb->show();
		connect(mb->button(QMessageBox::Ok), SIGNAL(clicked()), this, SLOT(deleteElement()));
	} else {
		deleteElement();
	}
}

void DraggableElement::checkElementForChildren()
{
	mIsRootDiagramNode = false;
	IdList const children = mEditorManagerProxy->getChildren(mDeletedElementId);
	if (!children.isEmpty()) {
		QString childrenNames;
		foreach (Id const child, children) {
			childrenNames += " " + mEditorManagerProxy->friendlyName(child) + ",";
		}
		if (!childrenNames.isEmpty()) {
			childrenNames.replace(childrenNames.length() - 1, 1, ".");
		}

		QMessageBox *mb = new QMessageBox(tr("Warning"), tr("The deleted element ") + mEditorManagerProxy->friendlyName(mDeletedElementId) + tr(" has inheritors:") + childrenNames + "\n" + tr("If you delete it, its properties will be removed from the elements-inheritors. Continue to delete?"), QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
		mb->button(QMessageBox::Ok)->setText(tr("Yes"));
		mb->button(QMessageBox::Cancel)->setText(tr("No"));
		mb->show();
		connect(mb->button(QMessageBox::Ok), SIGNAL(clicked()), this, SLOT(checkElementForRootDiagramNode()));
	} else {
		checkElementForRootDiagramNode();
	}
}

void DraggableElement::dragEnterEvent(QDragEnterEvent * /*event*/)
{
}

void DraggableElement::dropEvent(QDropEvent * /*event*/)
{
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
		if (mEditorManagerProxy->isInterpretationMode()) {
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
