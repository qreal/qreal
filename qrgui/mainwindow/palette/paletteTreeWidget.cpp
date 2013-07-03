#include <QtGui/QMouseEvent>
#include <QtWidgets/QMenu>

#include "paletteTreeWidget.h"
#include "paletteTree.h"
#include "draggableElement.h"
#include "../dialogs/metamodelingOnFly/chooseTypeDialog.h"

using namespace qReal;
using namespace gui;

EditorManagerInterface *PaletteTreeWidget::mEditorManager = NULL;

PaletteTreeWidget::PaletteTreeWidget(PaletteTree &palette, MainWindow &mainWindow
		, EditorManagerInterface &editorManagerProxy)
	: mMainWindow(mainWindow)
	, mPaletteTree(palette)
{
	mEditorManager = &editorManagerProxy;
}

void PaletteTreeWidget::addGroups(QMap<QString, QStringList> const &groups
		, QMap<QString, QString> const &descriptions
		, IdList const &allElements, bool hideIfEmpty
		, QString const &diagramFriendlyName)
{
	if (groups.isEmpty() && hideIfEmpty) {
		hide();
		return;
	}
	int expandedCount = 0;
	foreach (QString const &group, groups.keys()) {
		QTreeWidgetItem *item = new QTreeWidgetItem;
		item->setText(0, group);
		item->setToolTip(0, descriptions[group]);

		IdList tmpIdList;
		foreach (QString const &elementName, groups[group]) {
			foreach (Id const &element, allElements) {
				if (element.element() == elementName) {
					tmpIdList << element;
					break;
				}
			}
		}
		qSort(tmpIdList.begin(), tmpIdList.end(), idLessThan);

		addItemsRow(tmpIdList, item);

		addTopLevelItem(item);

		if (SettingsManager::value(diagramFriendlyName + group, 0).toBool()) {
			++expandedCount;
			expandItem(item);
		}
	}
	if (expandedCount == 0) {
		expand();
	}
}

void PaletteTreeWidget::addItemType(Id const &id, QString const &name, QString const &description
		, QIcon const &icon, QSize const &preferredSize, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *leaf = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(mMainWindow, id, name
			, description, icon, preferredSize
			, mPaletteTree.iconsView(), *mEditorManager
			);

	parent->addChild(leaf);
	setItemWidget(leaf, 0, element);
}

void PaletteTreeWidget::addItemsRow(IdList const &tmpIdList, QTreeWidgetItem *item)
{
	if (mPaletteTree.itemsCountInARow() == 1 || !mPaletteTree.iconsView()) {
		foreach (Id const &element, tmpIdList) {
			addItemType(element, mEditorManager->friendlyName(element)
					, mEditorManager->description(element)
					, mEditorManager->icon(element)
					, mEditorManager->iconSize(element)
					, item);
		}
		return;
	}
	for (IdList::ConstIterator it = tmpIdList.begin(); it != tmpIdList.end();) {
		QWidget *field = new QWidget;
		QHBoxLayout *layout = new QHBoxLayout;
		int count = mPaletteTree.itemsCountInARow();
		for (; it != tmpIdList.end() && --count > 0; ++it) {
			DraggableElement *element = new DraggableElement(
					mMainWindow
					, *it
					, mEditorManager->friendlyName(*it)
					, mEditorManager->description(*it)
					, mEditorManager->icon(*it)
					, mEditorManager->iconSize(*it)
					, true
					, *mEditorManager
					);

			element->setToolTip(mEditorManager->friendlyName(*it));
			layout->addWidget(element, count > 0 ? 50 : 0);
		}
		field->setLayout(layout);
		field->setMinimumHeight(80);
		QTreeWidgetItem *leaf = new QTreeWidgetItem;
		item->addChild(leaf);
		setItemWidget(leaf, 0, field);
	}
}

void PaletteTreeWidget::addElementPaletteActionTriggered()
{
	ChooseTypeDialog *chooseTypeDialog = new ChooseTypeDialog(mMainWindow
			, mPaletteTree.currentEditor(), *mEditorManager);
	chooseTypeDialog->setModal(true);
	chooseTypeDialog->show();
}

void PaletteTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		if (mEditorManager->isInterpretationMode()) {
			QMenu menu;
			QAction * const addElementPaletteAction = menu.addAction(tr("Add Element"));
			connect(addElementPaletteAction, SIGNAL(triggered()), this, SLOT(addElementPaletteActionTriggered()));
			menu.exec(QCursor::pos());
		}
	}

	QTreeWidget::mousePressEvent(event);
	QTreeWidgetItem *item = itemAt(event->pos());
	DraggableElement *child = item ? dynamic_cast<DraggableElement *>(itemWidget(item, 0)) : NULL;
	if (!child) {
		return;
	}

	Q_ASSERT(child->id().idSize() == 3);  // it should be element type

	// new element's ID is being generated here
	// may this epic event should take place in some more appropriate place

	Id elementId(child->id(), QUuid::createUuid().toString());

	QByteArray itemData;
	bool isFromLogicalModel = false;

	QDataStream stream(&itemData, QIODevice::WriteOnly);
	stream << elementId.toString();  // uuid
	stream << Id::rootId().toString();  // pathToItem
	stream << QString(child->text());
	stream << QPointF(0, 0);
	stream << isFromLogicalModel;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-real-uml-data", itemData);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);

	QPixmap const pixmap = child->icon().pixmap(child->iconsPreferredSize());

	if (!pixmap.isNull()) {
		drag->setPixmap(pixmap);
	}

	if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction) {
		child->close();
	} else {
		child->show();
	}
}

void PaletteTreeWidget::expand()
{
	for (int i = 0; i < topLevelItemCount(); i++) {
		if (topLevelItem(i)) {
			expandChildren(topLevelItem(i));
		}
	}
}

void PaletteTreeWidget::sortByFriendlyName(IdList &ids)
{
	qSort(ids.begin(), ids.end(), idLessThan);
}

void PaletteTreeWidget::expandChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++) {
		if (item->child(i)) {
			expandChildren(item->child(i));
		}
	}
	item->treeWidget()->expandItem(item);
}

void PaletteTreeWidget::collapse()
{
	for (int i = 0; i < topLevelItemCount(); i++) {
		if (topLevelItem(i)) {
			collapseChildren(topLevelItem(i));
		}
	}
}

void PaletteTreeWidget::collapseChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++) {
		if (item->child(i)) {
			collapseChildren(item->child(i));
		}
	}
	item->treeWidget()->collapseItem(item);
}

bool PaletteTreeWidget::idLessThan(Id const &s1, Id const &s2)
{
	return mEditorManager->friendlyName(s1).toLower() < mEditorManager->friendlyName(s2).toLower();
}
