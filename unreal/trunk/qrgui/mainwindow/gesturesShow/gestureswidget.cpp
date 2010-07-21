#include "gestureswidget.h"
#include "ui_gestureswidget.h"

GesturesWidget::GesturesWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GesturesWidget)
{
	ui->setupUi(this);
	mGestureScene = new QGraphicsScene(ui->graphicsView);
	ui->graphicsView->setScene(mGestureScene);
	connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
			this, SIGNAL(currentElementChanged()));
	mTimer = new QTimer(this);
	QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(drawGesture()));
	mTimer->start(50);
}

GesturesWidget::~GesturesWidget()
{
	delete mTimer;
	delete ui;
}

void GesturesWidget::draw(QList<QPoint> const & path)
{
	mCurrentPointNumber = 0;
	mGestureScene->clear();
	mPath = path;

}

void GesturesWidget::drawGesture()
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
	QPoint currentPoint(coord(lastPaintedPoint.x(), nextPoint.x(), segmentNumber),
						coord(lastPaintedPoint.y(), nextPoint.y(), segmentNumber));
	mGestureScene->addLine(QLine(lastPaintedPoint, currentPoint), QPen(Qt::black));

	mCurrentPointNumber++;
}

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
