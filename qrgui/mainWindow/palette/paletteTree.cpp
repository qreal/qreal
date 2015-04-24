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

#include "paletteTree.h"

#include <QtCore/QUuid>
#include <QtGui/QMouseEvent>
#include <QtCore/QtAlgorithms>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QMimeData>
#include <QtGui/QDrag>

#include <qrkernel/settingsManager.h>
#include <qrkernel/definitions.h>
#include <qrkernel/settingsManager.h>

#include "mainWindow/palette/draggableElement.h"
#include "dialogs/metamodelingOnFly/propertiesDialog.h"

using namespace qReal;
using namespace gui;

PaletteTree::PaletteTree(QWidget *parent)
	: QWidget(parent)
	, mCurrentEditor(0)
{
	initUi();
	createPaletteTree();
}

void PaletteTree::initUi()
{
	mLayout = new QVBoxLayout(this);
	mLayout->setSpacing(0);
	QHBoxLayout * const controlButtonsLayout = new QHBoxLayout;
	controlButtonsLayout->setSpacing(0);

	mComboBox = new QComboBox;
	mComboBox->setGeometry(0, 0, 300, 50);
	mLayout->addWidget(mComboBox);
	mLayout->addLayout(controlButtonsLayout);

	mNodesStateButtonExpands = SettingsManager::value("nodesStateButtonExpands").toBool();
	mChangeExpansionState = new QToolButton;
	mChangeExpansionState->setGeometry(0, 0, 30, 30);
	setExpansionButtonAppearance();
	mChangeExpansionState->setIconSize(QSize(30, 30));
	connect(mChangeExpansionState, SIGNAL(clicked()), this, SLOT(changeExpansionState()));
	controlButtonsLayout->addWidget(mChangeExpansionState);

	mChangeRepresentation = new QToolButton;
	mChangeRepresentation->setGeometry(0, 0, 30, 30);
	mChangeRepresentation->setIcon(QIcon(":/mainWindow/images/changeRepresentation.png"));
	mChangeRepresentation->setToolTip(tr("Change representation"));
	mChangeRepresentation->setIconSize(QSize(30, 30));
	connect(mChangeRepresentation, SIGNAL(clicked()), this, SLOT(changeRepresentation()));
	controlButtonsLayout->addWidget(mChangeRepresentation);

	setMinimumSize(200, 100);
}

void PaletteTree::expand()
{
	mTree->expand();
}

void PaletteTree::collapse()
{
	mTree->collapse();
}

void PaletteTree::setActiveEditor(int index)
{
	if (0 <= index && index < mEditorsTrees.count()) {
		mCurrentEditor = index;
		if (mTree) {
			mTree->hide();
		}

		mTree = mEditorsTrees[index];
		mTree->show();
	}
}

void PaletteTree::setActiveEditor(const Id &id)
{
	setActiveEditor(mCategories.value(id, 0));
}

void PaletteTree::addEditorElements(EditorManagerInterface &editorManagerProxy, const Id &editor, const Id &diagram)
{
	mEditorManager = &editorManagerProxy;
	mEditorsNames.push_back(mEditorManager->friendlyName(diagram));

	mCategories[diagram] = mEditorsTrees.size();

	mComboBox->addItem(mEditorManager->friendlyName(diagram));

	PaletteTreeWidgets *editorTree = new PaletteTreeWidgets(*this, mMainWindow, *mEditorManager, editor, diagram);
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
		mComboBox->show();
	}
	setActiveEditor(SettingsManager::value("CurrentIndex", 0).toInt());
	mTree->resizeIcons();
}

void PaletteTree::setComboBox(const Id &id)
{
	mComboBox->setCurrentIndex(mCategories.value(id, -1));
}

QComboBox* PaletteTree::comboBox() const
{
	return mComboBox;
}

QList<QString> PaletteTree::editorsNames() const
{
	return mEditorsNames;
}

void PaletteTree::deleteEditor(const Id &id)
{
	if (mCategories.contains(id)) {
		PaletteTreeWidgets *tree = mEditorsTrees[mCategories[id]];
		if (mTree == tree) {
			mTree = nullptr;
		}

		mComboBox->removeItem(mCategories[id]);
		mEditorsNames.removeAt(mCategories[id]);
		mEditorsTrees.removeAt(mCategories[id]);

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
	mTree = new PaletteTreeWidgets(*this, mMainWindow, *mEditorManager);
	mTree->setMinimumHeight(0);
	mLayout->addWidget(mTree);
}

void PaletteTree::deletePaletteTree()
{
	mComboBox->clear();
	if (mTree) {
		mEditorsTrees.removeAll(mTree);
		mLayout->removeWidget(mTree);
		delete mTree;
	}

	qDeleteAll(mEditorsTrees);
	mEditorsTrees.clear();
	mEditorsNames.clear();
	mCategories.clear();
	mCurrentEditor = 0;
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
	foreach (const PaletteTreeWidgets *editorTree, mEditorsTrees) {
		editorTree->saveConfiguration(mComboBox->itemText(diagramIndex));
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
	foreach (const Id &editor, editorManagerProxy.editors()) {
		foreach (const Id &diagram, editorManagerProxy.diagrams(editor)) {
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

void PaletteTree::resizeEvent(QResizeEvent *)
{
	mTree->resizeIcons();
}

int PaletteTree::maxItemsCountInARow() const
{
	const int max = mTree->maxItemsCountInARow();
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
		// TODO: Can it really be nullptr?
		loadEditors(*mEditorManager);
	}

	initDone();
	setComboBoxIndex();
}

void PaletteTree::initMainWindow(MainWindow *mainWindow)
{
	mMainWindow = mainWindow;
}

void PaletteTree::changeExpansionState()
{
	mNodesStateButtonExpands = !mNodesStateButtonExpands;
	if (mNodesStateButtonExpands) {
		expand();
	} else {
		collapse();
	}
	setExpansionButtonAppearance();
}

void PaletteTree::setExpansionButtonAppearance()
{
	if (mNodesStateButtonExpands) {
		mChangeExpansionState->setIcon(QIcon(":/mainWindow/images/collapseAll.png"));
		mChangeExpansionState->setToolTip(tr("Collapse all"));
	} else {
		mChangeExpansionState->setIcon(QIcon(":/mainWindow/images/expandAll.png"));
		mChangeExpansionState->setToolTip(tr("Expand all"));
	}
}

void PaletteTree::installEventFilter(QObject *obj)
{
	QWidget::installEventFilter(obj);
	comboBox()->installEventFilter(obj);
}

void PaletteTree::setElementVisible(const Id &metatype, bool visible)
{
	mTree->setElementVisible(metatype, visible);
}

void PaletteTree::setVisibleForAllElements(bool visible)
{
	mTree->setVisibleForAllElements(visible);
}

void PaletteTree::setElementEnabled(const Id &metatype, bool enabled)
{
	mTree->setElementEnabled(metatype, enabled);
}

void PaletteTree::setEnabledForAllElements(bool enabled)
{
	mTree->setEnabledForAllElements(enabled);
}

void PaletteTree::refreshUserPalettes()
{
	for (PaletteTreeWidgets * const tree : mEditorsTrees) {
		tree->refreshUserPalette();
	}
}

void PaletteTree::customizeExplosionTitles(const QString &userGroupTitle, const QString &userGroupDescription)
{
	for (PaletteTreeWidgets * const tree : mEditorsTrees) {
		tree->customizeExplosionTitles(userGroupTitle, userGroupDescription);
	}
}
