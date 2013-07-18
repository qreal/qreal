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
#include "../dialogs/metamodelingOnFly/propertiesDialog.h"

using namespace qReal;
using namespace gui;

PaletteTree::PaletteTree(QWidget *parent)
	: QWidget(parent)
	, mCurrentEditor(0)
{
	createPaletteTree();
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

void PaletteTree::setActiveEditor(Id const &id)
{
	setActiveEditor(mCategories.value(id, 0));
}

void PaletteTree::addEditorElements(EditorManagerInterface &editorManagerProxy, Id const &editor, Id const &diagram)
{
	mEditorManager = &editorManagerProxy;
	mEditorsNames.push_back(mEditorManager->friendlyName(diagram));

	mCategories[diagram] = mEditorsTrees.size();

	mComboBox->addItem(mEditorManager->friendlyName(diagram));

	PaletteTreeWidgets *editorTree = new PaletteTreeWidgets(*this, mMainWindow
			, *mEditorManager, editor, diagram);
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
	mTree->resizeIcons();
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

void PaletteTree::deleteEditor(Id const &id)
{
	if (mCategories.contains(id)) {
		PaletteTreeWidgets *tree = mEditorsTrees[mCategories[id]];
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

	mTree = new PaletteTreeWidgets(*this, mMainWindow, *mEditorManager);

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
	foreach (PaletteTreeWidgets const *editorTree, mEditorsTrees) {
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

void PaletteTree::resizeEvent(QResizeEvent *)
{
	mTree->resizeIcons();
}

int PaletteTree::maxItemsCountInARow() const
{
	int const max = mTree->maxItemsCountInARow();
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
