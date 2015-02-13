#include "gesturesWidget.h"

#include "ui_gesturesWidget.h"
#include "gesturePainter.h"

using namespace qReal::gestures;

const int pointsAtSegment = 5;

GesturesWidget::GesturesWidget(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::GesturesWidget)
{
	mUi->setupUi(this);
	connect(mUi->listWidget, &QListWidget::currentItemChanged, this, &GesturesWidget::currentElementChanged);
	mUi->gesturePixmap->setStyleSheet("background-color: white; border: 1px solid black");
}

GesturesWidget::~GesturesWidget()
{
	delete mUi;
}

void GesturesWidget::draw(const QString &paths)
{
	const int frame = 10;
	const int gestureAreaSize = qMin(mUi->gesturePixmap->width(), mUi->gesturePixmap->size().height()) - frame;
	const QSize size(gestureAreaSize, gestureAreaSize);

	GesturePainter painter(paths, Qt::white, Qt::blue, gestureAreaSize);
	const QPixmap gestureIcon = painter.pixmap(size, QIcon::Mode::Normal, QIcon::State::Off);

	mUi->gesturePixmap->setPixmap(gestureIcon.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

int GesturesWidget::coord(int previous, int next, int part)
{
	return previous + (next - previous) * part / pointsAtSegment;
}

qReal::Id GesturesWidget::currentElement() const
{
	return mUi->listWidget->currentItem()->data(Qt::UserRole).value<qReal::Id>();
}

void GesturesWidget::setElements(QList<QPair<QString, qReal::Id> > const &elements)
{
	mUi->listWidget->clear();
	QListIterator<QPair<QString, qReal::Id> > iterator(elements);
	while (iterator.hasNext()) {
		QPair<QString, qReal::Id> const element(iterator.next());
		QListWidgetItem *item = new QListWidgetItem(element.first);
		item->setData(Qt::UserRole, QVariant::fromValue(element.second));
		mUi->listWidget->addItem(item);
	}
}
