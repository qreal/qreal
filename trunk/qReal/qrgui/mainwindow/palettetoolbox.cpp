#include <QtGui>
#include <QtCore/QUuid>

#include "palettetoolbox.h"
#include "../kernel/definitions.h"

using namespace qReal;
using namespace qReal::gui;

PaletteToolbox::DraggableElement::DraggableElement(Id const &id, QString const &name,
	QIcon const &icon, QWidget *parent)
: QWidget(parent)
{
	mId = id;
	mText = name;
	mIcon = icon;

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	QLabel *pic = new QLabel(this);
	pic->setFrameStyle(QFrame::Box | QFrame::Sunken);
	pic->setFixedSize(24, 24);
	pic->setPixmap(mIcon.pixmap(24, 24));
	layout->addWidget(pic);

	QLabel *text = new QLabel(this);
	text->setFrameStyle(QFrame::Box | QFrame::Sunken);
	text->setText(mText);
	layout->addWidget(text);

	setLayout(layout);
}

PaletteToolbox::PaletteToolbox(QWidget *parent)
	: QToolBox(parent)
{
}

void PaletteToolbox::addDiagramType(Id const &id, QString const &name)
{
	Q_ASSERT(!mCategories.contains(id));

	QWidget *tab = new QWidget(this);

	QVBoxLayout *layout = new QVBoxLayout(tab);
	layout->setSpacing(0);
	layout->setContentsMargins (0, 0, 0, 0);
	tab->setLayout(layout);

	Q_ASSERT(id.idSize() == 2);  // Это должна быть диаграмма
	mCategories[id] = addItem(tab, name);
}

void PaletteToolbox::addItemType(Id const &id, QString const &name, QIcon const &icon)
{
	Id category(id.editor(), id.diagram());
	QWidget *tab = widget(mCategories[category]);
	Q_ASSERT(tab);

	DraggableElement *element = new DraggableElement(id, name, icon, this);
	tab->layout()->addWidget(element);
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
	stream << QString("(anon element)");
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
