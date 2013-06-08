#include <QtCore/QUuid>
#include <QtGui/QMouseEvent>
#include <QtCore/QtAlgorithms>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QMimeData>
#include <QtGui/QDrag>

#include "../../qrkernel/settingsManager.h"
#include "../../qrkernel/definitions.h"
#include "paletteTree.h"
#include "../../qrkernel/settingsManager.h"
#include "draggableElement.h"
#include "paletteTreeWidget.h"
#include "../dialogs/metamodelingOnFly/propertiesDialog.h"

using namespace qReal;
using namespace gui;

EditorManagerInterface * PaletteTree::mEditorManager = NULL;

PaletteTree::PaletteTree(QWidget *parent)
		: QWidget(parent)
		, mCurrentEditor(0)
{
	createPaletteTree();
}

void PaletteTree::addItemType(const Id &id, QString const &name, QString const &description
		, const QIcon &icon, QSize const &preferredSize, QTreeWidget *tree, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *leaf = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(*mMainWindow, id, name
			, description, icon, preferredSize
			, mIconsView, *mEditorManager
			);

	parent->addChild(leaf);
	tree->setItemWidget(leaf, 0, element);
}

void PaletteTree::addTopItemType(Id const &id, QString const &name
		, QString const &description, QIcon const &icon
		, QSize const &preferredSize, QTreeWidget *tree)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(*mMainWindow, id, name
			, description, icon, preferredSize
			, mIconsView, *mEditorManager
			);

	tree->addTopLevelItem(item);
	tree->setItemWidget(item, 0, element);
}

bool PaletteTree::idLessThan(const Id &s1, const Id &s2)
{
	return mEditorManager->friendlyName(s1).toLower() < mEditorManager->friendlyName(s2).toLower();
}

void PaletteTree::collapseChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++) {
		if (item->child(i)) {
			collapseChildren(item->child(i));
		}
	}
	item->treeWidget()->collapseItem(item);
}

void PaletteTree::collapse()
{
	for (int i = 0; i < mTree->topLevelItemCount(); i++) {
		if (mTree->topLevelItem(i)) {
			collapseChildren(mTree->topLevelItem(i));
		}
	}
}

void PaletteTree::setActiveEditor(int index)
{
	if (0 <= index && index < mEditorsTrees.count()) {
		mCurrentEditor = index;
		if (mTree != NULL) {
			mTree->hide();
		}

		mTree = mEditorsTrees[index];
		mTree->show();
	}
}

void PaletteTree::setActiveEditor(Id id)
{
	setActiveEditor(mCategories.value(id, 0));
}

void PaletteTree::expandChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++) {
		if (item->child(i)) {
			expandChildren(item->child(i));
		}
	}
	item->treeWidget()->expandItem(item);
}

void PaletteTree::expand()
{
	expand(mTree);
}

void PaletteTree::expand(QTreeWidget const *tree)
{
	for (int i = 0; i < tree->topLevelItemCount(); i++) {
		if (tree->topLevelItem(i)) {
			expandChildren(tree->topLevelItem(i));
		}
	}
}

void PaletteTree::addItemsRow(IdList const &tmpIdList, QTreeWidget *editorTree, QTreeWidgetItem *item)
{
	if (mItemsCountInARow == 1 || !mIconsView) {
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
		int count = mItemsCountInARow;
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

void PaletteTree::addEditorElements(EditorManagerInterface &editorManagerProxy, const Id &editor, const Id &diagram)
{
	mEditorManager = &editorManagerProxy;
	mEditorsNames.push_back(mEditorManager->friendlyName(diagram));

	mComboBox->addItem(mEditorManager->friendlyName(diagram));

	QTreeWidget *editorTree = new PaletteTreeWidget(*this, *mMainWindow, *mEditorManager);
	editorTree->setHeaderHidden(true);
	editorTree->setSelectionMode(QAbstractItemView::NoSelection);

	IdList elements = mEditorManager->elements(diagram);
	IdList groups = mEditorManager->groups(diagram);
	elements.append(groups);
	qSort(elements.begin(), elements.end(), idLessThan);

	mCategories[diagram] = mEditorsTrees.size();

	if (!mEditorManager->paletteGroups(editor, diagram).empty()) {
		int expandedCount = 0;
		foreach (QString const &group, mEditorManager->paletteGroups(editor, diagram)) {
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(0, group);
			item->setToolTip(0, mEditorManager->paletteGroupDescription(editor, diagram, group));

			IdList tmpIdList;

			foreach (QString const &elementName, mEditorManager->paletteGroupList(editor, diagram, group)) {
				foreach (Id const &element, elements) {
					if (element.element() == elementName) {
						tmpIdList.append(element);
						break;
					}
				}
			}

			qSort(tmpIdList.begin(), tmpIdList.end(), idLessThan);

			addItemsRow(tmpIdList, editorTree, item);

			editorTree->addTopLevelItem(item);

			if (SettingsManager::value(mEditorManager->friendlyName(diagram) + group, 0).toBool()) {
				++expandedCount;
				editorTree->expandItem(item);
			}
		}
		if (expandedCount == 0) {
			expand(editorTree);
		}
	} else {
		foreach (Id const &element, elements) {
			addTopItemType(element, mEditorManager->friendlyName(element)
					, mEditorManager->description(element)
					, mEditorManager->icon(element)
					, mEditorManager->iconSize(element)
					, editorTree);
		}
	}
	editorTree->hide();

	mEditorsTrees.push_back(editorTree);

	mLayout->addWidget(editorTree);
}

void PaletteTree::initDone()
{
	if (mComboBox->count() == 1) {
		// In case of empty combobox we do not hide it for user`s ensuring in items absence
		mComboBox->hide();
	} else {
		connect(mComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveEditor(int)));
	}
	setActiveEditor(SettingsManager::value("CurrentIndex", 0).toInt());
	resizeIcons();
}

void PaletteTree::setComboBox(Id const &id)
{
	mComboBox->setCurrentIndex(mCategories.value(id, -1));
}

QComboBox* PaletteTree::comboBox() const
{
	return mComboBox;
}

QVector<QString> PaletteTree::editorsNames() const
{
	return mEditorsNames;
}

void PaletteTree::deleteEditor(const Id &id)
{
	if (mCategories.contains(id)) {
		QTreeWidget * tree = mEditorsTrees[mCategories[id]];
		if (mTree == tree) {
			mTree = NULL;
		}

		mComboBox->removeItem(mCategories[id]);
		mEditorsNames.remove(mCategories[id]);
		mEditorsTrees.remove(mCategories[id]);

		delete tree;
		mCategories.remove(id);
	}
}

Id PaletteTree::currentEditor() const
{
	return mCategories.key(mCurrentEditor);
}

void PaletteTree::recreateTrees()
{
	deletePaletteTree();
	createPaletteTree();
}

void PaletteTree::createPaletteTree()
{
	mLayout = new QVBoxLayout(this);
	mLayout->setSpacing(0);

	mComboBox = new QComboBox;
	mComboBox->setGeometry(0,0,300,50);
	mLayout->addWidget(mComboBox);

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(0);

	mExpandAll = new QToolButton;
	mExpandAll->setGeometry(0,0,30,30);
	mExpandAll->setIcon(QIcon(":/icons/expandAll.png"));
	mExpandAll->setToolTip(tr("Expand all"));
	mExpandAll->setIconSize(QSize(30, 30));
	connect(mExpandAll,SIGNAL(clicked()),this,SLOT(expand()));
	hLayout->addWidget(mExpandAll);

	mCollapseAll = new QToolButton;
	mCollapseAll->setGeometry(0,0,30,30);
	mCollapseAll->setIcon(QIcon(":/icons/collapseAll.png"));
	mCollapseAll->setToolTip(tr("Collapse all"));
	mCollapseAll->setIconSize(QSize(30, 30));
	connect(mCollapseAll,SIGNAL(clicked()),this,SLOT(collapse()));
	hLayout->addWidget(mCollapseAll);

	mChangeRepresentation = new QToolButton;
	mChangeRepresentation->setGeometry(0,0,30,30);
	mChangeRepresentation->setIcon(QIcon(":/icons/changeRepresentation.png"));
	mChangeRepresentation->setToolTip(tr("Change representation"));
	mChangeRepresentation->setIconSize(QSize(30, 30));
	connect(mChangeRepresentation, SIGNAL(clicked()), this, SLOT(changeRepresentation()));
	hLayout->addWidget(mChangeRepresentation);

	mLayout->addLayout(hLayout);

	mTree = new PaletteTreeWidget(*this, *mMainWindow, *mEditorManager);

	mTree->setHeaderHidden(true);
	mLayout->addWidget(mTree);
	setMinimumWidth(200);
}

void PaletteTree::deletePaletteTree()
{
	delete mCollapseAll;
	delete mExpandAll;
	delete mChangeRepresentation;
	delete mComboBox;
	delete mLayout;
	qDeleteAll(mEditorsTrees);
	mEditorsTrees.clear();
	mEditorsNames.clear();
	mCategories.clear();
}

PaletteTree::~PaletteTree()
{
	deletePaletteTree();
}

void PaletteTree::setComboBoxIndex(int index)
{
	mComboBox->setCurrentIndex(index);
}

void PaletteTree::setComboBoxIndex()
{
	mComboBox->setCurrentIndex(SettingsManager::value("CurrentIndex", 0).toInt());
}

void PaletteTree::saveConfiguration()
{
	SettingsManager::setValue("PaletteRepresentation", mIconsView);
	SettingsManager::setValue("PaletteIconsInARowCount", mItemsCountInARow);
	int diagramIndex = 0;
	foreach (const QTreeWidget *editorTree, mEditorsTrees) {
		for (int j = 0; j < editorTree->topLevelItemCount(); j++) {
			const QTreeWidgetItem *topItem = editorTree->topLevelItem(j);
			if (topItem) {
				SettingsManager::setValue(mComboBox->itemText(diagramIndex) + topItem->text(0), topItem->isExpanded());
			}
		}
		diagramIndex++;
	}

	SettingsManager::setValue("CurrentIndex", mComboBox->currentIndex() >= 0 ? mComboBox->currentIndex() : 0);
}

bool PaletteTree::iconsView() const
{
	return mIconsView;
}

void PaletteTree::setIconsView(bool iconsView)
{
	mIconsView = iconsView;
}

void PaletteTree::loadEditors(EditorManagerInterface &editorManagerProxy)
{
	foreach (Id const &editor, editorManagerProxy.editors()) {
		foreach (Id const &diagram, editorManagerProxy.diagrams(editor)) {
			addEditorElements(editorManagerProxy, editor, diagram);
		}
	}

	const int index = SettingsManager::value("CurrentIndex", 0).toInt();
	SettingsManager::setValue("CurrentIndex", index >= 0 ? index : 0);
}

void PaletteTree::setItemsCountInARow(int count)
{
	mItemsCountInARow = count;
}

int PaletteTree::itemsCountInARow() const
{
	return mItemsCountInARow;
}

void PaletteTree::resizeIcons()
{
	if (mIconsView && mItemsCountInARow > 1) {
		const int iconSize = 48;
		const int widgetSize = this->size().width() - (iconSize << 1);
		const int itemsCount = maxItemsCountInARow();
		const int newSize = (widgetSize < itemsCount * iconSize)
				? (widgetSize / itemsCount) : iconSize;
		for (int i = 0; i < mTree->topLevelItemCount(); i++) {
			for (int j = 0; j < mTree->topLevelItem(i)->childCount(); j++) {
				QWidget *field = mTree->itemWidget(mTree->topLevelItem(i)->child(j), 0);
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

void PaletteTree::resizeEvent(QResizeEvent *)
{
	resizeIcons();
}

int PaletteTree::maxItemsCountInARow() const
{
	int max = 0;
	for (int i = 0; i < mTree->topLevelItemCount(); i++) {
		for (int j = 0; j < mTree->topLevelItem(i)->childCount(); j++) {
			QWidget *field = mTree->itemWidget(mTree->topLevelItem(i)->child(j), 0);
			if (!field) {
				break;
			}

			int itemsCount = field->children().count();
			if (itemsCount > max) {
				max = itemsCount;
			}
		}
	}
	return max ? max : mItemsCountInARow;
}

void PaletteTree::changeRepresentation()
{
	loadPalette(!mIconsView, mItemsCountInARow, mEditorManager);
	SettingsManager::setValue("PaletteRepresentation", mIconsView);
	SettingsManager::setValue("PaletteIconsInARowCount", mItemsCountInARow);
	emit paletteParametersChanged();
}

void PaletteTree::loadPalette(bool isIconsView, int itemsCount, EditorManagerInterface *editorManagerProxy)
{
	if (mEditorManager) {
		recreateTrees();
	}

	mIconsView = isIconsView;
	mEditorManager = editorManagerProxy;
	mItemsCountInARow = itemsCount;
	if (mEditorManager) {
		// TODO: Can it really be NULL?
		loadEditors(*mEditorManager);
	}

	initDone();
	setComboBoxIndex();
}

void PaletteTree::initMainWindow(MainWindow *mainWindow)
{
	mMainWindow = mainWindow;
}
