/** @file palettetoolbox.cpp
 * 	@brief Класс палитры элементов
 * */
#include <QtGui>

#include "palettetoolbox.h"
#include "realrepoinfo.h"

PaletteToolbox::DraggableElement::DraggableElement(TypeIdType const &classid, QWidget *parent)
	: QWidget(parent)
{
	RealRepoInfo info;

	m_id = classid;
	m_text = info.objectDesc(classid);
	m_icon = info.objectIcon(classid);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(4, 4, 4, 4);
	QLabel *icon = new QLabel(this);
	icon->setFixedSize(16, 16);
	icon->setPixmap(m_icon.pixmap(16, 16));
	layout->addWidget(icon);

	QLabel *text = new QLabel(this);
	text->setText(m_text);
	layout->addWidget(text);

	setLayout(layout);
}

PaletteToolbox::PaletteToolbox(QWidget *parent)
	: QTabWidget(parent)
{
	RealRepoInfo info;

	setTabPosition(QTabWidget::West);
	setTabShape(QTabWidget::Triangular);
	//  setAcceptDrops(true);
	QStringList categories = info.getObjectCategories();
	mTabs.resize(categories.size());
	mTabNames.resize(categories.size());
	mShownTabs.resize(categories.size());

	QSettings settings("Tercom", "QReal");

	unsigned i = 0;
	foreach (QString category, categories) {
		QScrollArea *scroller = new QScrollArea(this);
		QWidget *tab = new QWidget(this);
		QVBoxLayout *layout = new QVBoxLayout(tab);

		layout->setSpacing(0);
		layout->setContentsMargins(0, 0, 0, 0);

		foreach(TypeIdType classid, info.getObjects(category)) {
			DraggableElement *element = new DraggableElement(classid, this);
			layout->addWidget(element);
		}

		tab->setLayout(layout);
		scroller->setWidget(tab);

		Q_ASSERT(!category.isEmpty());

		mTabs[i] = scroller;
		mTabNames[i] = category;
		mShownTabs[i] = !settings.contains(category)
			|| settings.value(category).toString() == "Show";

		addTab(scroller, category);
		++i;
	}
	setEditors(mShownTabs);
}

PaletteToolbox::~PaletteToolbox()
{
	QSettings settings("Tercom", "QReal");
	for (int i = 0; i < mTabNames.count(); ++i)
		if (mShownTabs[i])
			settings.setValue(mTabNames[i], "Show");
		else
			settings.setValue(mTabNames[i], "Hide");
}

void PaletteToolbox::setEditors(QVector<bool> const &editors)
{
	Q_ASSERT(editors.count() == mTabs.count());
	setUpdatesEnabled(false);
	for (int i = 0; i < editors.size(); ++i)
	{
		if (editors[i] && indexOf(mTabs[i]) == -1)
			addTab(mTabs[i], mTabNames[i]);
		if (!editors[i] && indexOf(mTabs[i]) != -1)
			removeTab(indexOf(mTabs[i]));
		mShownTabs[i] = editors[i];
	}
	setUpdatesEnabled(true);
}

QVector<bool> PaletteToolbox::getSelectedTabs() const
{
	return mShownTabs;
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
	stream << child->id();		// type
	stream << -1;				// old parent
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

void PaletteToolbox::wheelEvent(QWheelEvent *event)
{
	if (tabBar()->underMouse())
	{
		if (event->delta() < 0 && currentIndex() < count())
			setCurrentIndex(currentIndex() + 1);
		else if (event->delta() > 0 && currentIndex() > 0)
			setCurrentIndex(currentIndex() - 1);
		event->accept();
	} else
		event->ignore();
}

