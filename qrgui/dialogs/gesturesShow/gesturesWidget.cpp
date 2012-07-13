#include "gesturesWidget.h"
#include "ui_gesturesWidget.h"

int const minBoarder = -1000;
QColor const gestColor(Qt::blue);
short const gestWidth(3);

GesturesWidget::GesturesWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GesturesWidget)
{
	ui->setupUi(this);
	mGestureScene = new QGraphicsScene(ui->graphicsView);
	ui->graphicsView->setScene(mGestureScene);
	connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
			this, SIGNAL(currentElementChanged()));
}

GesturesWidget::~GesturesWidget()
{
	delete ui;
}

void GesturesWidget::draw(PathVector const &paths)
{
	mGestureScene->clear();

	foreach (PointVector const &path, paths)
	{
		QPointF previousPoint(minBoarder, minBoarder);

		QPen pen(gestColor);
		pen.setWidth(gestWidth);

		if (path.isEmpty()) {
			return;
		}
		foreach (QPointF const &currentPoint, path)
		{
			if (previousPoint.x() != minBoarder && previousPoint.y() != minBoarder) {
				mGestureScene->addLine(QLineF(previousPoint, currentPoint), pen);
			}
			else {
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

QString GesturesWidget::currentElement()
{
	return ui->listWidget->currentItem()->text();
}

void GesturesWidget::setElements(const QList<QString> &elements)
{
	ui->listWidget->clear();
	ui->listWidget->addItems(elements);
}

