#include "paletteTreeWidgets.h"
#include "paletteTree.h"
#include "draggableElement.h"

using namespace qReal;
using namespace gui;

EditorManagerInterface *PaletteTreeWidgets::mEditorManager = NULL;

PaletteTreeWidgets::PaletteTreeWidgets(PaletteTree &parent, MainWindow &mainWindow
		, EditorManagerInterface &editorManagerProxy)
	: mParentPalette(&parent)
	, mMainWindow(&mainWindow)
	, mEditorTree(new PaletteTreeWidget(parent, mainWindow, editorManagerProxy))
	, mUserTree(new PaletteTreeWidget(parent, mainWindow, editorManagerProxy))
{
	mEditorManager = &editorManagerProxy;
	initWidget();
}

PaletteTreeWidgets::PaletteTreeWidgets(PaletteTree &parent, MainWindow &mainWindow
		, EditorManagerInterface &editorManagerProxy
		, Id const &editor, Id const &diagram)
	: mParentPalette(&parent)
	, mMainWindow(&mainWindow)
	, mEditor(editor)
	, mDiagram(diagram)
	, mEditorTree(new PaletteTreeWidget(parent, mainWindow, editorManagerProxy))
	, mUserTree(new PaletteTreeWidget(parent, mainWindow, editorManagerProxy))
{
	mEditorManager = &editorManagerProxy;
	initWidget();
	initEditorTree();
	initUserTree();
}

void PaletteTreeWidgets::initWidget()
{
	QVBoxLayout *layout = new QVBoxLayout;
	initWidget(mEditorTree, layout);
	initWidget(mUserTree, layout);
	setLayout(layout);
}

void PaletteTreeWidgets::initWidget(PaletteTreeWidget * const tree, QVBoxLayout * const layout)
{
	tree->setHeaderHidden(true);
	tree->setSelectionMode(QAbstractItemView::NoSelection);
	layout->addWidget(tree);
}

	void PaletteTreeWidgets::initEditorTree()
{
	IdList elements = mEditorManager->elements(mDiagram) + mEditorManager->groups(mDiagram);
	qSort(elements.begin(), elements.end(), idLessThan);

	if (!mEditorManager->paletteGroups(mEditor, mDiagram).empty()) {
		int expandedCount = 0;
		foreach (QString const &group, mEditorManager->paletteGroups(mEditor, mDiagram)) {
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(0, group);
			item->setToolTip(0, mEditorManager->paletteGroupDescription(mEditor, mDiagram, group));

			IdList tmpIdList;

			foreach (QString const &elementName, mEditorManager->paletteGroupList(mEditor, mDiagram, group)) {
				foreach (Id const &element, elements) {
					if (element.element() == elementName) {
						tmpIdList.append(element);
						break;
					}
				}
			}

			qSort(tmpIdList.begin(), tmpIdList.end(), idLessThan);

			addItemsRow(tmpIdList, mEditorTree, item);

			mEditorTree->addTopLevelItem(item);

			if (SettingsManager::value(mEditorManager->friendlyName(mDiagram) + group, 0).toBool()) {
				++expandedCount;
				mEditorTree->expandItem(item);
			}
		}
		if (expandedCount == 0) {
			expand(mEditorTree);
		}
	} else {
		foreach (Id const &element, elements) {
			addTopItemType(element, mEditorManager->friendlyName(element)
					, mEditorManager->description(element)
					, mEditorManager->icon(element)
					, mEditorManager->iconSize(element)
					, mEditorTree);
		}
	}
}

void PaletteTreeWidgets::initUserTree()
{
}

void PaletteTreeWidgets::addItemsRow(IdList const &tmpIdList, QTreeWidget *editorTree, QTreeWidgetItem *item)
{
	if (mParentPalette->itemsCountInARow() == 1 || !mParentPalette->iconsView()) {
		foreach (const Id &element, tmpIdList) {
			addItemType(element, mEditorManager->friendlyName(element)
					, mEditorManager->description(element)
					, mEditorManager->icon(element)
					, mEditorManager->iconSize(element)
					, editorTree, item);
		}
		return;
	}
	for (IdList::ConstIterator it = tmpIdList.begin(); it != tmpIdList.end();) {
		QWidget *field = new QWidget;
		QHBoxLayout *layout = new QHBoxLayout;
		int count = mParentPalette->itemsCountInARow();
		for (; it != tmpIdList.end() && count-- > 0; ++it) {
			DraggableElement *element = new DraggableElement(
					*mMainWindow
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
		editorTree->setItemWidget(leaf, 0, field);
	}
}

void PaletteTreeWidgets::addItemType(const Id &id, QString const &name, QString const &description
		, const QIcon &icon, QSize const &preferredSize, QTreeWidget *tree, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *leaf = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(*mMainWindow, id, name
			, description, icon, preferredSize
			, mParentPalette->iconsView(), *mEditorManager
			);

	parent->addChild(leaf);
	tree->setItemWidget(leaf, 0, element);
}

void PaletteTreeWidgets::addTopItemType(Id const &id, QString const &name
		, QString const &description, QIcon const &icon
		, QSize const &preferredSize, QTreeWidget *tree)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(*mMainWindow, id, name
			, description, icon, preferredSize
			, mParentPalette->iconsView(), *mEditorManager
			);

	tree->addTopLevelItem(item);
	tree->setItemWidget(item, 0, element);
}

void PaletteTreeWidgets::resizeIcons()
{
	if (mParentPalette->iconsView() && mParentPalette->itemsCountInARow() > 1) {
		const int iconSize = 48;
		const int widgetSize = this->size().width() - (iconSize << 1);
		const int itemsCount = maxItemsCountInARow();
		const int newSize = (widgetSize < itemsCount * iconSize)
				? (widgetSize / itemsCount) : iconSize;
		for (int i = 0; i < mEditorTree->topLevelItemCount(); i++) {
			for (int j = 0; j < mEditorTree->topLevelItem(i)->childCount(); j++) {
				QWidget *field = mEditorTree->itemWidget(mEditorTree->topLevelItem(i)->child(j), 0);
				if (!field) {
					break;
				}

				foreach (QObject *child, field->children()) {
					DraggableElement *element = dynamic_cast<DraggableElement*>(child);
					if (element) {
						element->setIconSize(newSize);
					}
				}
			}
		}
	}
}

int PaletteTreeWidgets::maxItemsCountInARow() const
{
	int max = 0;
	for (int i = 0; i < mEditorTree->topLevelItemCount(); i++) {
		for (int j = 0; j < mEditorTree->topLevelItem(i)->childCount(); j++) {
			QWidget *field = mEditorTree->itemWidget(mEditorTree->topLevelItem(i)->child(j), 0);
			if (!field) {
				break;
			}

			int itemsCount = field->children().count();
			if (itemsCount > max) {
				max = itemsCount;
			}
		}
	}
	return max;
}

void PaletteTreeWidgets::expand()
{
	expand(mEditorTree);
	expand(mUserTree);
}

void PaletteTreeWidgets::expand(QTreeWidget const *tree)
{
	for (int i = 0; i < tree->topLevelItemCount(); i++) {
		if (tree->topLevelItem(i)) {
			expandChildren(tree->topLevelItem(i));
		}
	}
}

void PaletteTreeWidgets::expandChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++) {
		if (item->child(i)) {
			expandChildren(item->child(i));
		}
	}
	item->treeWidget()->expandItem(item);
}

void PaletteTreeWidgets::collapse()
{
	collapse(mEditorTree);
	collapse(mUserTree);
}

void PaletteTreeWidgets::collapse(QTreeWidget const *tree)
{
	for (int i = 0; i < tree->topLevelItemCount(); i++) {
		if (tree->topLevelItem(i)) {
			collapseChildren(tree->topLevelItem(i));
		}
	}
}

void PaletteTreeWidgets::collapseChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++) {
		if (item->child(i)) {
			collapseChildren(item->child(i));
		}
	}
	item->treeWidget()->collapseItem(item);
}

void PaletteTreeWidgets::saveConfiguration(QString const &title) const
{
	saveConfiguration(mEditorTree, title);
	saveConfiguration(mUserTree, title);
}

void PaletteTreeWidgets::saveConfiguration(PaletteTreeWidget const *tree, QString const &title) const
{
	for (int j = 0; j < tree->topLevelItemCount(); j++) {
		QTreeWidgetItem const *topItem = tree->topLevelItem(j);
		if (topItem) {
			SettingsManager::setValue(title, topItem->isExpanded());
		}
	}
}

bool PaletteTreeWidgets::idLessThan(Id const &s1, Id const &s2)
{
	return mEditorManager->friendlyName(s1).toLower() < mEditorManager->friendlyName(s2).toLower();
}
