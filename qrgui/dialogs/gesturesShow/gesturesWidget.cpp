#include "gesturesWidget.h"
#include "ui_gesturesWidget.h"

const int minBoarder = -1000;

GesturesWidget::GesturesWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GesturesWidget)
{
	ui->setupUi(this);
	mGestureScene = new QGraphicsScene(ui->graphicsView);
	ui->graphicsView->setScene(mGestureScene);
	connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
			this, SIGNAL(currentElementChanged()));
//	mTimer = new QTimer(this);
//	QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(drawGesture()));
//	mTimer->start(5);
}

GesturesWidget::~GesturesWidget()
{
//	delete mTimer;
	delete ui;
}

//сделать, чтобы работало от листа листа точек, эта штука лежит в тулзах в PaintManager::drawPath
void GesturesWidget::draw(PathVector const &paths)
{
	mGestureScene->clear();

	foreach (PointVector path, paths)
	{
		QPointF previousPoint(minBoarder, minBoarder);

		QPen pen(Qt::blue);
		pen.setWidth(3);

		if (path.isEmpty())
			return;
		foreach (QPointF currentPoint, path)
		{
			if (previousPoint.x() != minBoarder && previousPoint.y() != minBoarder) {
				mGestureScene->addLine(QLineF(previousPoint, currentPoint), pen);
			}
			else
				mGestureScene->addLine(QLineF(currentPoint, currentPoint), pen);
			previousPoint = currentPoint;
		}
	}
	/*
	mCurrentPointNumber = 0;
	mGestureScene->clear();
	mPath = path;
	*/
}

/*void GesturesWidget::drawGesture()
{
	if (mPath.isEmpty())
		return;
	int verticeIndex = (mCurrentPointNumber / pointsAtSegment) % mPath.size();
	int segmentNumber = mCurrentPointNumber % pointsAtSegment + 1;
	if (verticeIndex == mPath.size() - 1)
	{
		mGestureScene->clear();
		mCurrentPointNumber = 0;
		return;
	}

	QPoint lastPaintedPoint = mPath.at(verticeIndex);
	QPoint nextPoint = mPath.at(verticeIndex + 1);
	QPoint currentPoint(coord(lastPaintedPoint.x(), nextPoint.x(), segmentNumber)
			, coord(lastPaintedPoint.y(), nextPoint.y(), segmentNumber));

	QPen pen(Qt::blue);
	pen.setWidth(3);

	if (mCurrentPointNumber == 0)
		mGestureScene->addEllipse(QRect(currentPoint, currentPoint).adjusted(-3, -3, 2, 2), pen);

	mGestureScene->addLine(QLine(lastPaintedPoint, currentPoint), pen);

	mCurrentPointNumber++;
}*/


int GesturesWidget::coord(int previous, int next, int part)
{
	return previous + (next - previous) * part / pointsAtSegment;
}

QString GesturesWidget::currentElement()
{
	return ui->listWidget->currentItem()->text();
}

void GesturesWidget::setElements(const QList<QString> &elements)
{
	ui->listWidget->clear();
	ui->listWidget->addItems(elements);
}

