#include <QtGui>

#include "palettetoolbox.h"

PaletteToolbox::DraggableElement::DraggableElement(QString id, QString name,
					QIcon icon, QWidget *parent/*0*/)
	: QWidget(parent)
{
	m_id = id;
	m_text = name;
	m_icon = icon;

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins ( 0,0,0,0 );
	layout->setSpacing( 0 );

	QLabel *pic = new QLabel(this);
	pic->setFrameStyle( QFrame::Box | QFrame::Sunken );
	pic->setFixedSize(16,16);
	pic->setPixmap(m_icon.pixmap(16,16));
	layout->addWidget(pic);

	QLabel *text = new QLabel(this);
	text->setFrameStyle( QFrame::Box | QFrame::Sunken );
	text->setText(m_text);
	layout->addWidget(text);

	setLayout(layout);
}

PaletteToolbox::PaletteToolbox(QWidget *parent)
	: QToolBox(parent)
{
}

void PaletteToolbox::addDiagramType( QString id, QString name )
{
	if (categories.contains(id)) {
		qDebug() << "Toolbox already has category " << id;
		return;
	}

	QWidget *tab = new QWidget( this );

	QVBoxLayout *layout = new QVBoxLayout(tab);
        layout->setSpacing(0);
        layout->setContentsMargins ( 0,0,0,0 );
	tab->setLayout(layout);

        categories[id] = addItem(tab, name);	
}

void PaletteToolbox::addItemType( QString id, QString name, QIcon icon )
{
	QString category = id.section('/',0,-2);
	QWidget *tab = widget(categories[category]);

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
	stream << child->id();			// type
	stream << QString("(anon element)");
	stream << QPointF(0,0);

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
