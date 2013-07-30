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
		, EditorManagerInterface &editorManagerProxy, bool editable)
	: mMainWindow(mainWindow)
	, mPaletteTree(palette)
	, mEditable(editable)
{
	mEditorManager = &editorManagerProxy;
}

void PaletteTreeWidget::addGroups(QMap<QString, QList<PaletteElement> > &groups
		, QMap<QString, QString> const &descriptions
		, bool hideIfEmpty
		, QString const &diagramFriendlyName)
{
	if (groups.isEmpty() && hideIfEmpty) {
		hide();
		return;
	}

	clear();
	show();

	int expandedCount = 0;
	foreach (QString const &group, groups.keys()) {
		QTreeWidgetItem *item = new QTreeWidgetItem;
		item->setText(0, group);
		item->setToolTip(0, descriptions[group]);

		sortByFriendlyName(groups[group]);
		addItemsRow(groups[group], item);
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

void PaletteTreeWidget::addItemType(PaletteElement const &data, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *leaf = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(mMainWindow, data
			, mPaletteTree.iconsView(), *mEditorManager);

	parent->addChild(leaf);
	setItemWidget(leaf, 0, element);
}

void PaletteTreeWidget::addItemsRow(QList<PaletteElement> const &items, QTreeWidgetItem *parent)
{
	if (mPaletteTree.itemsCountInARow() == 1 || !mPaletteTree.iconsView()) {
		foreach (PaletteElement const &element, items) {
			addItemType(element, parent);
		}
		return;
	}
	for (QList<PaletteElement>::ConstIterator it = items.begin(); it != items.end();) {
		QWidget *field = new QWidget;
		QHBoxLayout *layout = new QHBoxLayout;
		int count = mPaletteTree.itemsCountInARow();
		for (; it != items.end() && --count > 0; ++it) {
			DraggableElement *element = new DraggableElement(mMainWindow, *it, true, *mEditorManager);
			element->setToolTip((*it).description());
			layout->addWidget(element, count > 0 ? 50 : 0);
		}
		field->setLayout(layout);
		field->setMinimumHeight(80);
		QTreeWidgetItem *leaf = new QTreeWidgetItem;
		parent->addChild(leaf);
		if (mEditable) {
			leaf->setFlags(leaf->flags() | Qt::ItemIsEditable);
		}
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

void PaletteTreeWidget::sortByFriendlyName(QList<PaletteElement> &elements)
{
	qSort(elements.begin(), elements.end(), paletteElementLessThan);
}

void PaletteTreeWidget::editItem(QTreeWidgetItem * const item)
{
	edit(indexFromItem(item));
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

bool PaletteTreeWidget::paletteElementLessThan(PaletteElement const &s1, PaletteElement const &s2)
{
	return idLessThan(s1.id(), s2.id());
}
