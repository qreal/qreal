#include <QtCore/QUuid>
#include <QtGui/QLabel>
#include <QtGui/QMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QScrollArea>

#include "paletteToolbox.h"
#include "../kernel/definitions.h"

using namespace qReal;
using namespace qReal::gui;

PaletteToolbox::DraggableElement::DraggableElement(Id const &id, QString const &name,
	QIcon const &icon, QWidget *parent)
: QWidget(parent), mId(id), mIcon(icon), mText(name)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(4, 4, 4, 4);

	QLabel *pic = new QLabel(this);
	pic->setFixedSize(24, 24);
	pic->setPixmap(mIcon.pixmap(22, 22));  // 24 - "рамочка"
	layout->addWidget(pic);

	QLabel *text = new QLabel(this);
	text->setText(mText);
	layout->addWidget(text);

	setLayout(layout);
}

PaletteToolbox::PaletteToolbox(QWidget *parent)
	: QWidget(parent)
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
	mScrollArea->takeWidget();
	delete mScrollArea;
	delete mComboBox;
	delete mLayout;

	for (int i = 0; i < mTabs.count(); i++)
		delete mTabs[i];
}

void PaletteToolbox::setActiveEditor(int const comboIndex)
{
	mScrollArea->takeWidget(); // Save current editor from extermination.
	mScrollArea->setWidget(mTabs[comboIndex]);
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

	Q_ASSERT(id.idSize() == 2);  // Это должна быть диаграмма
	mCategories[id] = mTabs.size() - 1;

	mComboBox->addItem(name);

	Q_ASSERT(mTabNames.size() == mTabs.size());
}

void PaletteToolbox::addItemType(Id const &id, QString const &name, QIcon const &icon)
{
	Id category(id.editor(), id.diagram());
	QWidget *tab = mTabs[mCategories[category]];
	Q_ASSERT(tab);

	DraggableElement *element = new DraggableElement(id, name, icon, this);
	tab->layout()->addWidget(element);
}

void PaletteToolbox::initDone()
{
	connect(mComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveEditor(int)));
	setActiveEditor(0);
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

	Q_ASSERT(child->id().idSize() == 3);  // Это должен быть тип элемента

	// Id нового элемента генерится здесь. Возможно, не слишком подходящее место
	// для столь важного действия.
	Id elementId(child->id(), QUuid::createUuid().toString());

	QByteArray itemData;

	QDataStream stream(&itemData, QIODevice::WriteOnly);
	stream << elementId.toString();  // uuid
	stream << ROOT_ID.toString();  // pathToItem
	stream << QString("(anonymous " + child->text() + ")");
	stream << QPointF(0, 0);

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
