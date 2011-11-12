#include <QtCore/QUuid>
#include <QtGui/QLabel>
#include <QtGui/QMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QScrollArea>
#include <QtAlgorithms>

#include "paletteToolbox.h"
#include "../../qrkernel/definitions.h"
using namespace qReal;
using namespace qReal::gui;

EditorManager * PaletteToolbox::mEditMan = NULL;

PaletteToolbox::DraggableElement::DraggableElement(Id const &id, QString const &name
		, QString const &description, QIcon const &icon, QWidget *parent
		)
		: QWidget(parent)
		, mId(id)
		, mIcon(icon)
		, mText(name)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(4, 4, 4, 4);

	QLabel *pic = new QLabel(this);
	pic->setFixedSize(24, 24); // the frame
	pic->setPixmap(mIcon.pixmap(22, 22));
	layout->addWidget(pic);

	QLabel *text = new QLabel(this);
	text->setText(mText);
	layout->addWidget(text);

	setLayout(layout);

	QString modifiedDescription = description;
	if (!modifiedDescription.isEmpty()){
		modifiedDescription.insert(0, "<body>");//turns alignment on
		setToolTip(modifiedDescription);
	}
}

PaletteToolbox::PaletteToolbox(QWidget *parent)
	: QWidget(parent), mCurrentTab(0)
{
	createPalette();
}

void PaletteToolbox::createPalette()
{
	mLayout = new QVBoxLayout;
	mLayout->setSpacing(6);
	mLayout->setMargin(0);

	mComboBox = new QComboBox;
	mLayout->addWidget(mComboBox);

	mScrollArea = new QScrollArea;
	mLayout->addWidget(mScrollArea);

	setLayout(mLayout);
}

PaletteToolbox::~PaletteToolbox()
{
	deletePalette();
}

void PaletteToolbox::deletePalette()
{
	mScrollArea->takeWidget();
	delete mScrollArea;
	delete mComboBox;
	delete mLayout;

	for (int i = 0; i < mTabs.count(); i++)
		delete mTabs[i];
	mTabs.clear();
	mTabNames.clear();
	mCategories.clear();
}

void PaletteToolbox::setActiveEditor(Id id)
{
	setActiveEditor(mCategories.value(id, 0));
}

void PaletteToolbox::setActiveEditor(int const comboIndex)
{
	mCurrentTab = comboIndex;
	if (mTabs.size() > 0) {
		mScrollArea->takeWidget(); // Save current editor from extermination.
		mScrollArea->setWidget(mTabs[comboIndex]);
	}
}

void PaletteToolbox::addDiagramType(Id const &id, QString const &name)
{
	QWidget *tab = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(tab);

	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	tab->setLayout(layout);

	mTabs.append(tab);
	mTabNames.append(name);

	Q_ASSERT(id.idSize() == 2); // it should be diagram
	mCategories[id] = mTabs.size() - 1;

	mComboBox->addItem(name);

	Q_ASSERT(mTabNames.size() == mTabs.size());
}

void PaletteToolbox::addItemType(Id const &id, QString const &name, QString const &description,  QIcon const &icon)
{
	Id category(id.editor(), id.diagram());
	QWidget *tab = mTabs[mCategories[category]];
	Q_ASSERT(tab);

	DraggableElement *element = new DraggableElement(id, name, description, icon, this);
	tab->layout()->addWidget(element);
}

void  PaletteToolbox::addSortedItemTypes(EditorManager &editman, const Id &diagram){
	mEditMan = &editman;
	IdList list = editman.elements(diagram);
	qSort(list.begin(), list.end(), idLessThan);
	foreach (const Id element, list)
		addItemType(element, editman.friendlyName(element)
				, editman.description(element), editman.icon(element));

}
bool PaletteToolbox::idLessThan(const Id &s1, const Id &s2){
	return mEditMan->friendlyName(s1).toLower() <
		mEditMan->friendlyName(s2).toLower();
}

void PaletteToolbox::deleteDiagramType(const Id &id)
{
	if (mCategories.contains(id)) {
		mComboBox->removeItem(mCategories[id]);
		mTabNames.remove(mCategories[id]);
		mTabs.remove(mCategories[id]);
		mCategories.remove(id);
	}
}

void PaletteToolbox::initDone()
{
	connect(mComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveEditor(int)));
	setActiveEditor(0);
}

QComboBox* PaletteToolbox::getComboBox()
{
	return mComboBox;
}

QVector<QString> PaletteToolbox::getTabNames()
{
	return mTabNames;
}

void PaletteToolbox::dragEnterEvent(QDragEnterEvent * /*event*/)
{
}

void PaletteToolbox::dropEvent(QDropEvent * /*event*/)
{
}

void PaletteToolbox::mousePressEvent(QMouseEvent *event)
{
	QWidget *atMouse = childAt(event->pos());
	if (!atMouse || atMouse == this)
		return;

	DraggableElement *child = dynamic_cast<DraggableElement *>(atMouse->parent());
	if (!child)
		child = dynamic_cast<DraggableElement *>(atMouse);
	if (!child)
		return;

	Q_ASSERT(child->id().idSize() == 3); // it should be element type

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

	if (!p.isNull())
		drag->setPixmap(child->icon().pixmap(96, 96));

	if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction)
		child->close();
	else
		child->show();
}

Id PaletteToolbox::currentTab()
{
	return mCategories.key(mCurrentTab);
}

void PaletteToolbox::setComboBox(Id id)
{
	mComboBox->setCurrentIndex(mCategories.value(id, -1));
}

void PaletteToolbox::recreateTabs()
{
	deletePalette();
	createPalette();
}
