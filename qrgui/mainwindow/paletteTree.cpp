#include <QtCore/QUuid>
#include <QtGui/QMouseEvent>
#include "../../qrkernel/definitions.h"
#include "paletteTree.h"
#include <QtCore/QtAlgorithms>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

using namespace qReal;
using namespace gui;

EditorManager * PaletteTree::mEditorManager = NULL;

PaletteTree::DraggableElement::DraggableElement(const Id &id, const QString &name
		, const QString &description, const QIcon &icon, bool iconsOnly, QWidget *parent)
	: QWidget(parent)
	, mId(id)
	, mIcon(icon)
	, mText(name)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(4, 4, 4, 4);

	int size = iconsOnly ? 50 : 30;
	QLabel *pic = new QLabel(this);
	pic->setFixedSize(size, size);  // the frame
	pic->setPixmap(mIcon.pixmap(size - 2, size - 2));
	layout->addWidget(pic);

	if (!iconsOnly) {
		QLabel *text = new QLabel(this);
		text->setText(mText);
		layout->addWidget(text);
	}

	setLayout(layout);

	QString modifiedDescription = description;
	if (!modifiedDescription.isEmpty()) {
		modifiedDescription.insert(0, "<body>");  //turns alignment on
		setToolTip(modifiedDescription);
	}
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
	tree->setItemWidget(item,0,element);
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
	return mEditorManager->friendlyName(s1).toLower() <
			mEditorManager->friendlyName(s2).toLower();
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
	mCurrentEditor = index;
	mTree->hide();
	mTree = mEditorsTrees[index];
	mTree->show();
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

void PaletteTree::addItemsRow(IdList &tmpIdList, QTreeWidget *editorTree, QTreeWidgetItem *item)
{
	if (mItemsCountInARow == 1 || !mIconsView) {
		foreach (const Id &element, tmpIdList) {
			addItemType(element, mEditorManager->friendlyName(element)
					, mEditorManager->description(element)
					, mEditorManager->icon(element), editorTree, item);
		}
		return;
	}
	for (IdList::ConstIterator it = tmpIdList.begin(); it != tmpIdList.end();) {
		QWidget *field = new QWidget;
		QHBoxLayout *layout = new QHBoxLayout;
		int count = mItemsCountInARow;
		for (; it != tmpIdList.end() && count-- > 0; ++it) {
			DraggableElement *element = new DraggableElement(*it
					, mEditorManager->friendlyName(*it)
					, mEditorManager->description(*it)
					, mEditorManager->icon(*it)
					, true);
			element->setToolTip(mEditorManager->friendlyName(*it));
			layout->addWidget(element);
			if (count > 0) {
				layout->addStretch(50);
			}
		}
		field->setLayout(layout);
		QTreeWidgetItem *leaf = new QTreeWidgetItem;
		item->addChild(leaf);
		editorTree->setItemWidget(leaf, 0, field);
	}
}

void PaletteTree::addEditorElements(EditorManager &editorManager, const Id &editor, const Id &diagram)
{
	mEditorManager = &editorManager;
	mEditorsNames.push_back(mEditorManager->friendlyName(diagram));

	mComboBox->addItem(mEditorManager->friendlyName(diagram));

	QTreeWidget *EditorTree = new QTreeWidget(this);
	EditorTree->setHeaderHidden(true);

	IdList list = mEditorManager->elements(diagram);
	qSort(list.begin(), list.end(), idLessThan);

	mCategories[diagram] = mEditorsTrees.size() - 1;

	if (!mEditorManager->paletteGroups(editor, diagram).empty()) {
		foreach (const QString &group, mEditorManager->paletteGroups(editor, diagram)) {
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(0, group);

			IdList tmpIdList;

			foreach (const QString &elementName, mEditorManager->paletteGroupList(editor, diagram, group)) {
				foreach (const Id &element, list) {
					if (mEditorManager->friendlyName(element) == elementName) {
						tmpIdList.append (element);
						break;
					}
				}
			}
			qSort(tmpIdList.begin(), tmpIdList.end(), idLessThan);

			addItemsRow(tmpIdList, EditorTree, item);

			EditorTree->addTopLevelItem(item);

			if (mSettings->value("Diagram" + mEditorsTrees.count() + group, 0).toBool()) {
				EditorTree->expandItem(item);
				mSettings->remove("Diagram" + mEditorsTrees.count() + group);
			}
		}
	} else {
		foreach (const Id &element, list) {
			addTopItemType(element, mEditorManager->friendlyName(element)
					, mEditorManager->description(element)
					, mEditorManager->icon(element), EditorTree);
		}
	}
	EditorTree->hide();

	mEditorsTrees.push_back(EditorTree);

	mLayout->addWidget(EditorTree);
}

void PaletteTree::initDone()
{
	connect(mComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveEditor(int)));
	setActiveEditor(mSettings->value("CurrentIndex", 0).toInt());
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

	QLabel *header = new QLabel("Palette");
	header->setAlignment(Qt::AlignHCenter);
	mLayout->addWidget(header);

	mComboBox = new QComboBox;
	mComboBox->setGeometry(0,0,300,50);
	mLayout->addWidget(mComboBox);

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(0);

	mExpandAll  = new QPushButton(this);
	mExpandAll->setGeometry(0,0,140,50);
	mExpandAll->setText("Expand All");
	connect(mExpandAll,SIGNAL(clicked()),this,SLOT(expand()));
	hLayout->addWidget(mExpandAll);

	mCollapseAll  = new QPushButton(this);
	mCollapseAll->setGeometry(0,0,140,50);
	mCollapseAll->setText("Collapse All");
	connect(mCollapseAll,SIGNAL(clicked()),this,SLOT(collapse()));
	hLayout->addWidget(mCollapseAll);
	mLayout->addLayout(hLayout,0);

	mTree = new QTreeWidget(this);
	mTree->setHeaderHidden(true);
	mLayout->addWidget(mTree);
	mSettings = new QSettings("Qreal", "PaletteItems");
}

void PaletteTree::deletePaletteTree()
{
	for (int i = 0; i < mEditorsTrees.count(); i++) {
		const QTreeWidget *editorTree = mEditorsTrees[i];
		for (int j = 0; j < editorTree->topLevelItemCount(); j++) {
			const QTreeWidgetItem *topItem = editorTree->topLevelItem(j);
			if (topItem && topItem->isExpanded()) {
				mSettings->setValue("Diagram" + i + topItem->text(0), 1);
			}
		}
	}
	mSettings->setValue("CurrentIndex", mComboBox->currentIndex());
	mSettings->sync();
	delete mCollapseAll;
	delete mExpandAll;
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

bool PaletteTree::IconsView() const
{
	return mIconsView;
}

void PaletteTree::setIconsView(bool iconsView)
{
	mIconsView = iconsView;
}

void PaletteTree::loadEditors(EditorManager &editorManager)
{
	foreach (Id const editor, editorManager.editors()) {
		foreach (Id const diagram, editorManager.diagrams(editor)) {
			addEditorElements(editorManager, editor, diagram);
		}
	}
}

void PaletteTree::setItemsCountInARow(int count)
{
	mItemsCountInARow = count;
}

int PaletteTree::ItemsCountInARow() const
{
	return mItemsCountInARow;
}
