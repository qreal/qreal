#include "gesturesWidget.h"
#include "ui_gesturesWidget.h"

using namespace qReal::gestures;

GesturesWidget::GesturesWidget(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::GesturesWidget)
{
	mUi->setupUi(this);
	mGestureScene = new QGraphicsScene(mUi->graphicsView);
	gestColor = Qt::blue;
	mUi->graphicsView->setScene(mGestureScene);
	connect(mUi->listWidget, SIGNAL(currentItemChanged(QListWidgetItem *,QListWidgetItem *))
			, this, SIGNAL(currentElementChanged()));
}

GesturesWidget::~GesturesWidget()
{
	delete mUi;
}

void GesturesWidget::draw(PathVector const &paths)
{
	mGestureScene->clear();

	foreach (PointVector const &path, paths) {
		QPointF previousPoint(minBoarder, minBoarder);

		QPen pen(gestColor);
		pen.setWidth(gestWidth);

		if (path.isEmpty()) {
			return;
		}
		foreach (QPointF const &currentPoint, path) {
			if (previousPoint.x() != minBoarder && previousPoint.y() != minBoarder) {
				mGestureScene->addLine(QLineF(previousPoint, currentPoint), pen);
			} else {
				mGestureScene->addLine(QLineF(currentPoint, currentPoint), pen);
			}
			previousPoint = currentPoint;
		}
	}
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
