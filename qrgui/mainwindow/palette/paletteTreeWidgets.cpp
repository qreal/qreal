#include "paletteTreeWidgets.h"

#include "mainWindow/palette/paletteTree.h"
#include "mainWindow/palette/draggableElement.h"

using namespace qReal;
using namespace gui;

PaletteTreeWidgets::PaletteTreeWidgets(PaletteTree &parent, MainWindow *mainWindow
		, EditorManagerInterface &editorManagerProxy)
	: QSplitter(Qt::Vertical)
	, mEditorManager(&editorManagerProxy)
	, mParentPalette(&parent)
	, mMainWindow(mainWindow)
	, mEditorTree(new PaletteTreeWidget(parent, *mainWindow, editorManagerProxy, false))
	, mUserTree(new PaletteTreeWidget(parent, *mainWindow, editorManagerProxy, true))
{
	initWidgets();
}

PaletteTreeWidgets::PaletteTreeWidgets(PaletteTree &parent, MainWindow *mainWindow
		, EditorManagerInterface &editorManagerProxy
		, Id const &editor, Id const &diagram)
	: QSplitter(Qt::Vertical)
	, mParentPalette(&parent)
	, mMainWindow(mainWindow)
	, mEditor(editor)
	, mDiagram(diagram)
	, mEditorTree(new PaletteTreeWidget(parent, *mainWindow, editorManagerProxy, false))
	, mUserTree(new PaletteTreeWidget(parent, *mainWindow, editorManagerProxy, true))
{
	mEditorManager = &editorManagerProxy;
	initWidgets();
	initEditorTree();
	initUserTree();
}

void PaletteTreeWidgets::initWidgets()
{
	initWidget(mEditorTree);
	initWidget(mUserTree);
}

void PaletteTreeWidgets::initWidget(PaletteTreeWidget * const tree)
{
	tree->setHeaderHidden(true);
	tree->setSelectionMode(QAbstractItemView::NoSelection);
	addWidget(tree);
}

void PaletteTreeWidgets::initEditorTree()
{
	IdList elements = mEditorManager->elements(mDiagram) + mEditorManager->groups(mDiagram);
	bool const sort = mEditorManager->shallPaletteBeSorted(mEditor, mDiagram);
	if (sort) {
		PaletteTreeWidget::sortByFriendlyName(elements);
	}

	if (!mEditorManager->paletteGroups(mEditor, mDiagram).empty()) {
		QList<QPair<QString, QList<PaletteElement>>> groups;
		QMap<QString, QString> descriptions;
		for (QString const &group : mEditorManager->paletteGroups(mEditor, mDiagram)) {
			QStringList const paletteGroup = mEditorManager->paletteGroupList(mEditor, mDiagram, group);
			QList<PaletteElement> groupElements;
			for (QString const &name : paletteGroup) {
				for (Id const &element : elements) {
					if (element.element() == name) {
						groupElements << PaletteElement(*mEditorManager, element);
						break;
					}
				}
			}

			groups << qMakePair(group, groupElements);
			descriptions[group] = mEditorManager->paletteGroupDescription(mEditor, mDiagram, group);
		}

		mEditorTree->addGroups(groups, descriptions, false, mEditorManager->friendlyName(mDiagram), sort);
	} else {
		for (Id const &element : elements) {
			addTopItemType(PaletteElement(*mEditorManager, element), mEditorTree);
		}
	}
}

void PaletteTreeWidgets::initUserTree()
{
	refreshUserPalette();
	connect(&mMainWindow->models()->exploser(), &models::Exploser::explosionsSetCouldChange
			, this, &PaletteTreeWidgets::refreshUserPalette);
}

void PaletteTreeWidgets::addTopItemType(PaletteElement const &data, QTreeWidget *tree)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(*mMainWindow, data
			, mParentPalette->iconsView(), *mEditorManager);

	mPaletteElements.insert(data.id(), element);

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
	mEditorTree->expand();
	mEditorTree->expand();
}

void PaletteTreeWidgets::collapse()
{
	mEditorTree->collapse();
	mUserTree->collapse();
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

void PaletteTreeWidgets::setElementVisible(Id const &metatype, bool visible)
{
	if (mPaletteElements.contains(metatype)) {
		mPaletteElements[metatype]->setVisible(visible);
	} else {
		mEditorTree->setElementVisible(metatype, visible);
	}
}

void PaletteTreeWidgets::setVisibleForAllElements(bool visible)
{
	foreach (QWidget * const element, mPaletteElements.values()) {
		element->setVisible(visible);
	}

	mEditorTree->setVisibleForAllElements(visible);
}

void PaletteTreeWidgets::setElementEnabled(Id const &metatype, bool enabled)
{
	if (mPaletteElements.contains(metatype)) {
		mPaletteElements[metatype]->setEnabled(enabled);
	} else {
		mEditorTree->setElementEnabled(metatype, enabled);
	}
}

void PaletteTreeWidgets::setEnabledForAllElements(bool enabled)
{
	foreach (QWidget * const element, mPaletteElements.values()) {
		element->setEnabled(enabled);
	}

	mEditorTree->setEnabledForAllElements(enabled);
}

void PaletteTreeWidgets::customizeExplosionTitles(QString const &userGroupTitle, QString const &userGroupDescription)
{
	mUserGroupTitle = userGroupTitle;
	mUserGroupDescription = userGroupDescription;
}

void PaletteTreeWidgets::refreshUserPalette()
{
	QList<QPair<QString, QList<gui::PaletteElement>>> groups;
	QMap<QString, QString> descriptions = { { mUserGroupTitle, mUserGroupDescription } };

	QMultiMap<Id, Id> const types = mMainWindow->models()->exploser().explosions(mDiagram);
	for (Id const &source : types.keys()) {
		QList<gui::PaletteElement> groupElements;
		for (Id const &target : types.values(source)) {
			groupElements << gui::PaletteElement(source
					, mMainWindow->models()->logicalRepoApi().name(target)
					, QString(), mEditorManager->icon(source)
					, mEditorManager->iconSize(source)
					, target);
		}

		groups << qMakePair(mUserGroupTitle, groupElements);
	}

	mUserTree->addGroups(groups, descriptions, true, mEditorManager->friendlyName(mDiagram), true);
}
