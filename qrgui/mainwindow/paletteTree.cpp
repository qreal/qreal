#include <QtCore/QUuid>
#include <QtGui/QMouseEvent>
#include "../../qrkernel/definitions.h"
#include "paletteTree.h"
#include <QtCore/QtAlgorithms>
#include <QtGui/QVBoxLayout>
#include "../../qrkernel/settingsManager.h"

using namespace qReal;
using namespace gui;

EditorManagerInterface * PaletteTree::mEditorManagerProxy = NULL;

PaletteTree::DraggableElement::DraggableElement(const Id &id, const QString &name
		, const QString &description, const QIcon &icon, bool iconsOnly, QWidget *parent)
	: QWidget(parent)
	, mId(id)
	, mIcon(icon)
	, mText(name)
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

void PaletteTree::DraggableElement::setIconSize(int size)
{
	mLabel->setPixmap(mIcon.pixmap(size , size));
}

PaletteTree::PaletteTree(QWidget *parent)
	:QWidget(parent), mCurrentEditor(0)
{
	createPaletteTree();
}

void PaletteTree::addItemType(const Id &id, const QString &name, const QString &description
		, const QIcon &icon, QTreeWidget *tree, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *leaf = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(id, name, description, icon, mIconsView);
	parent->addChild(leaf);
	tree->setItemWidget(leaf, 0, element);
}

void PaletteTree::addTopItemType(const Id &id, const QString &name
		, const QString &description, const QIcon &icon, QTreeWidget *tree)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	DraggableElement *element = new DraggableElement(id, name, description, icon, mIconsView);
	tree->addTopLevelItem(item);
	tree->setItemWidget(item, 0, element);
}

void PaletteTree::DraggableElement::dragEnterEvent(QDragEnterEvent * /*event*/)
{
}

void PaletteTree::DraggableElement::dropEvent(QDropEvent * /*event*/)
{
}

void PaletteTree::DraggableElement::mousePressEvent(QMouseEvent *event)
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

bool PaletteTree::idLessThan(const Id &s1, const Id &s2)
{
	return mEditorManagerProxy->friendlyName(s1).toLower() <
			mEditorManagerProxy->friendlyName(s2).toLower();
}

void PaletteTree::collapseChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++) {
		if (item->child(i)) {
			collapseChildren(item->child(i));
		}
	}
	mTree->collapseItem(item);
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
		mTree->hide();
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
	mTree->expandItem(item);
}

void PaletteTree::expand()
{
	for (int i = 0; i < mTree->topLevelItemCount(); i++) {
		if (mTree->topLevelItem(i)) {
			expandChildren(mTree->topLevelItem(i));
		}
	}
}

void PaletteTree::addItemsRow(IdList const &tmpIdList, QTreeWidget *editorTree, QTreeWidgetItem *item)
{
	if (mItemsCountInARow == 1 || !mIconsView) {
		foreach (const Id &element, tmpIdList) {
			addItemType(element, mEditorManagerProxy->friendlyName(element)
					, mEditorManagerProxy->description(element)
					, mEditorManagerProxy->icon(element), editorTree, item);
		}
		return;
	}
	for (IdList::ConstIterator it = tmpIdList.begin(); it != tmpIdList.end();) {
		QWidget *field = new QWidget;
		QHBoxLayout *layout = new QHBoxLayout;
		int count = mItemsCountInARow;
		for (; it != tmpIdList.end() && count-- > 0; ++it) {
			DraggableElement *element = new DraggableElement(*it
					, mEditorManagerProxy->friendlyName(*it)
					, mEditorManagerProxy->description(*it)
					, mEditorManagerProxy->icon(*it)
					, true);
			element->setToolTip(mEditorManagerProxy->friendlyName(*it));
			layout->addWidget(element, count > 0 ? 50 : 0);
		}
		field->setLayout(layout);
		field->setMinimumHeight(80);
		QTreeWidgetItem *leaf = new QTreeWidgetItem;
		item->addChild(leaf);
		editorTree->setItemWidget(leaf, 0, field);

	}
}

void PaletteTree::addEditorElements(EditorManagerInterface *editorManagerProxy, const Id &editor, const Id &diagram)
{
	mEditorManagerProxy = editorManagerProxy;
	mEditorsNames.push_back(mEditorManagerProxy->friendlyName(diagram));

	mComboBox->addItem(mEditorManagerProxy->friendlyName(diagram));

	QTreeWidget *editorTree = new QTreeWidget(this);
	editorTree->setHeaderHidden(true);
	editorTree->setSelectionMode(QAbstractItemView::NoSelection);

	IdList list = mEditorManagerProxy->elements(diagram);
	qSort(list.begin(), list.end(), idLessThan);

	mCategories[diagram] = mEditorsTrees.size() - 1;

	if (!mEditorManagerProxy->paletteGroups(editor, diagram).empty()) {
		foreach (const QString &group, mEditorManagerProxy->paletteGroups(editor, diagram)) {
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(0, group);

			IdList tmpIdList;

			foreach (const QString &elementName, mEditorManagerProxy->paletteGroupList(editor, diagram, group)) {
				foreach (const Id &element, list) {
					if (element.element() == elementName) {
						tmpIdList.append (element);
						break;
					}
				}
			}
			qSort(tmpIdList.begin(), tmpIdList.end(), idLessThan);

			addItemsRow(tmpIdList, editorTree, item);

			editorTree->addTopLevelItem(item);

			if (mSettings->value(mEditorManagerProxy->friendlyName(diagram) + group, 0).toBool()) {
				editorTree->expandItem(item);
			}
		}
	} else {
		foreach (const Id &element, list) {
			addTopItemType(element, mEditorManagerProxy->friendlyName(element)
					, mEditorManagerProxy->description(element)
					, mEditorManagerProxy->icon(element), editorTree);
		}
	}
	editorTree->hide();

	mEditorsTrees.push_back(editorTree);

	mLayout->addWidget(editorTree);
}

void PaletteTree::initDone()
{
	connect(mComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveEditor(int)));
	setActiveEditor(mSettings->value("CurrentIndex", 0).toInt());
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
		QObject *w = mComboBox->children().value(mCategories[id]);
		mComboBox->removeItem(mCategories[id]);
		delete w;
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

	mTree = new QTreeWidget(this);
	mTree->setHeaderHidden(true);
	mLayout->addWidget(mTree);
	mSettings = new QSettings("QReal", "PaletteItems");
	setMinimumWidth(200);
}

void PaletteTree::deletePaletteTree()
{
	SettingsManager::setValue("PaletteRepresentation", mIconsView);
	SettingsManager::setValue("PaletteIconsInARowCount", mItemsCountInARow);
	int diagramIndex = 0;
	foreach(const QTreeWidget *editorTree, mEditorsTrees) {
		for (int j = 0; j < editorTree->topLevelItemCount(); j++) {
			const QTreeWidgetItem *topItem = editorTree->topLevelItem(j);
			if (topItem && topItem->isExpanded()) {
				mSettings->setValue(mComboBox->itemText(diagramIndex) + topItem->text(0), 1);
			}
		}
		diagramIndex++;
	}
	mSettings->setValue("CurrentIndex", mComboBox->currentIndex() >= 0 ? mComboBox->currentIndex() : 0);
	mSettings->sync();
	delete mSettings;
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
	mComboBox->setCurrentIndex(mSettings->value("CurrentIndex", 0).toInt());
}

bool PaletteTree::iconsView() const
{
	return mIconsView;
}

void PaletteTree::setIconsView(bool iconsView)
{
	mIconsView = iconsView;
}

void PaletteTree::loadEditors(EditorManagerInterface *editorManagerProxy)
{
	foreach (Id const editor, editorManagerProxy->editors()) {
		foreach (Id const diagram, editorManagerProxy->diagrams(editor)) {
			addEditorElements(editorManagerProxy, editor, diagram);
		}
	}
	const int index = mSettings->value("CurrentIndex", 0).toInt();
	mSettings->clear();
	mSettings->setValue("CurrentIndex", index >= 0 ? index : 0);
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
	loadPalette(!mIconsView, mItemsCountInARow, mEditorManagerProxy);
	SettingsManager::setValue("PaletteRepresentation", mIconsView);
	SettingsManager::setValue("PaletteIconsInARowCount", mItemsCountInARow);
	emit paletteParametersChanged();
}

void PaletteTree::loadPalette(bool isIconsView, int itemsCount, EditorManagerInterface *editorManagerProxy)
{
	if (mEditorManagerProxy) {
		recreateTrees();
	}
	mIconsView = isIconsView;
	mEditorManagerProxy = editorManagerProxy;
	mItemsCountInARow = itemsCount;
	loadEditors(editorManagerProxy);
	initDone();
	setComboBoxIndex();
}
