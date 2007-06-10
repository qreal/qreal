#include <QtGui>

#include "palettetoolbox.h"
#include "realrepoinfo.h"

PaletteToolbox::DraggableElement::DraggableElement(int classid, QWidget *parent/*0*/)
	: QWidget(parent)
{
	RealRepoInfo info;

	m_id = classid;
	m_text = info.objectDesc(classid);
	m_icon = info.objectIcon(classid);

	QHBoxLayout *layout = new QHBoxLayout(this);
	QLabel *icon = new QLabel(this);
	icon->setFixedSize(24,24);
	icon->setPixmap(m_icon.pixmap(24,24));
	layout->addWidget(icon);

	QLabel *text = new QLabel(this);
	text->setText(m_text);
	layout->addWidget(text);

	setLayout(layout);
}

PaletteToolbox::PaletteToolbox(QWidget *parent)
	: QToolBox(parent)
{
	RealRepoInfo info;
	//  setAcceptDrops(true);
	QStringList categories = info.getObjectCategories();
	for (int i = 0; i < categories.size(); i++) {
		QWidget *tab = new QWidget(this);
		QVBoxLayout *layout = new QVBoxLayout(this);
		addItem(tab, categories[i]);

		foreach(int classid, info.getObjects(i)) {
			DraggableElement *element = new DraggableElement(classid, this);
			layout->addWidget(element);
		}

		tab->setLayout(layout);
	}
}

void PaletteToolbox::dragEnterEvent(QDragEnterEvent *event)
{
}

void PaletteToolbox::dropEvent(QDropEvent *event)
{
}

void PaletteToolbox::mousePressEvent(QMouseEvent *event)
{
	QWidget *atMouse = childAt(event->pos());
	if ( ! atMouse || atMouse == this )
		return;

	DraggableElement *child = dynamic_cast<DraggableElement *>(atMouse->parent());
	if (!child)
		child = dynamic_cast<DraggableElement *>(atMouse);
	if (!child)
		return;

	QByteArray itemData;

	QDataStream stream(&itemData, QIODevice::WriteOnly);
	stream << -1;				// uuid
	stream << child->id();		// type
	stream << "New Element";

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-real-uml-data", itemData);
	// mimeData->setText(child->text());

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);

	QPixmap p = child->icon().pixmap(96,96);

	if ( ! p.isNull() )
		drag->setPixmap(child->icon().pixmap(96,96));

	if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction)
		child->close();
	else 
		child->show();
}
