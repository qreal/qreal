/** @file palettetoolbox.cpp
 * 	@brief Класс палитры элементов
 * */
#include <QtGui>

#include "palettetoolbox.h"
#include "realrepoinfo.h"
#include <QMessageBox>

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
	: QWidget(parent)
{
	RealRepoInfo info;

	mLayout = new QVBoxLayout;
	mLayout->setSpacing(6);
	mLayout->setMargin(0);

	mComboBox = new QComboBox;
	mLayout->addWidget(mComboBox);

	mScrollArea = new QScrollArea;
	mLayout->addWidget(mScrollArea);

	setLayout(mLayout);

	QStringList categories = info.getObjectCategories();
	mTabs.resize(categories.size());
	mTabNames.resize(categories.size());
	mShownTabs.resize(categories.size());

	QSettings settings("Tercom", "QReal");

	unsigned i = 0;
	foreach (QString category, categories) {
		QWidget *tab = new QWidget;
		QVBoxLayout *layout = new QVBoxLayout(tab);

		layout->setSpacing(0);
		layout->setContentsMargins(0, 0, 0, 0);

		foreach(TypeIdType classid, info.getObjects(category)) {
			DraggableElement *element = new DraggableElement(classid);
			layout->addWidget(element);
		}

		tab->setLayout(layout);

		Q_ASSERT(!category.isEmpty());

		mTabs[i] = tab;
		mTabNames[i] = category;
		mShownTabs[i] = !settings.contains(category)
			|| settings.value(category).toString() == "Show";

		++i;
	}

	if (i == 0)
	{
		qDebug() << "OMG WTF?" << categories;
	}
	mComboBox->setUpdatesEnabled(false);
	setEditors(mShownTabs);
	connect(mComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveEditor(int)));

	// This sequence changes current editor, so mComboBox will issue signal
	mComboBox->setCurrentIndex(-1);
	mComboBox->setCurrentIndex(0);
	mComboBox->setUpdatesEnabled(true);
}

PaletteToolbox::~PaletteToolbox()
{
	int i;

	QSettings settings("Tercom", "QReal");
	for (i = 0; i < mTabNames.count(); ++i)
		if (mShownTabs[i])
			settings.setValue(mTabNames[i], "Show");
		else
			settings.setValue(mTabNames[i], "Hide");

	mScrollArea->takeWidget();
	delete mScrollArea;
	delete mComboBox;
	delete mLayout;

	for (i = 0; i < mTabs.count(); i++)
		delete mTabs[i];
}

void PaletteToolbox::setActiveEditor(int comboIndex)
{
	mScrollArea->takeWidget(); // Save current editor from extermination.
	if (comboIndex == -1) return;
	mScrollArea->setWidget(mTabs[mComboToRepo[comboIndex]]);
}

void PaletteToolbox::setEditors(QVector<bool> const &editors)
{
	int oldComboIndex, oldIndex, curComboIndex = -1;
	int newComboIndex = -1;
	bool old_updatesEnabled;

	old_updatesEnabled = mComboBox->updatesEnabled();
	mComboBox->setUpdatesEnabled(false);
	oldComboIndex = mComboBox->currentIndex();
	oldIndex = (oldComboIndex==-1)?-1:mComboToRepo[oldComboIndex];

	mComboBox->clear();
	mComboToRepo.clear();
	mRepoToCombo.clear();

	// Rebuld vectors
	for (int i = 0; i < editors.size(); ++i)
	{
		if (editors[i])
		{
			curComboIndex++;
			mComboToRepo << i;
			mRepoToCombo << curComboIndex;
		}
		else
			mRepoToCombo << -1;
		mShownTabs[i] = editors[i];
	}

	// Fill Combobox
	for (int i = 0; i < editors.size(); ++i)
		if (editors[i])
			mComboBox->addItem(mTabNames[i]);

	if (oldIndex != -1)
		newComboIndex = mRepoToCombo[oldIndex];

	// Setting empty editor reduces usability, so avoid it
	if (newComboIndex == -1)
		newComboIndex = 0;
	mComboBox->setCurrentIndex(newComboIndex);
	mComboBox->setUpdatesEnabled(old_updatesEnabled);
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
        stream << child->id();  		// type
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
