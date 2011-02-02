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

PaletteToolbox::DraggableElement::DraggableElement(NewType const &type, QString const &name, QString const &description,
	QIcon const &icon, QWidget *parent)
: QWidget(parent), mType(type), mIcon(icon), mText(name)
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

	QString a = description;
		if (a.compare("") != 0){
		a.insert(0, "<body>");//turns alignment on
		setToolTip(a);
	}

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
	if (mTabs.size() > 0) {
		mScrollArea->takeWidget(); // Save current editor from extermination.
		mScrollArea->setWidget(mTabs[comboIndex]);
	}
}

void PaletteToolbox::addDiagramType(NewType const &type, QString const &name)
{
	QWidget *tab = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(tab);

	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	tab->setLayout(layout);

	mTabs.append(tab);
	mTabNames.append(name);

        Q_ASSERT(type.typeSize() == 2); // it should be diagram
        mCategories[type] = mTabs.size() - 1;

	mComboBox->addItem(name);

	Q_ASSERT(mTabNames.size() == mTabs.size());
}

void PaletteToolbox::addItemType(NewType const &type, QString const &name, QString const &description,  QIcon const &icon)
{
        NewType category(type.editor(), type.diagram());
	QWidget *tab = mTabs[mCategories[category]];
	Q_ASSERT(tab);

        DraggableElement *element = new DraggableElement(type, name, description, icon, this);
	tab->layout()->addWidget(element);
}

void PaletteToolbox::deleteDiagramType(const NewType &type)
{
        if (mCategories.contains(type)) {
                mComboBox->removeItem(mCategories[type]);
                mTabNames.remove(mCategories[type]);
                mTabs.remove(mCategories[type]);
                mCategories.remove(type);
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

        Q_ASSERT(child->type().typeSize() == 3); // it should be element type

	// new element's ID is being generated here
	// may this epic event should take place in some more appropriate place

        NewType elementType(child->type());
        Id elementId(QUuid::createUuid().toString());

	QByteArray itemData;

	QDataStream stream(&itemData, QIODevice::WriteOnly);
        stream << elementType.toString();  // uuid
        stream << elementId.toString();
	stream << ROOT_ID.toString();  // pathToItem
	stream << QString("(" + child->text() + ")");
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
