#include "circleWidget.h"

#include <QtCore/QVariant>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QPainter>

using namespace qReal;

CircleWidget::CircleWidget(QSize const &size, const QString &icon, QWidget *parent)
	: QWidget(parent)
{
	QSize const iconSize = size - QSize(25, 25);

	QPixmap const pixmap(icon);
	const int targetHeight = iconSize.height();
	const int targetWidth = iconSize.width();

	const int pictureHeight = pixmap.height();
	const int pictureWidth = pixmap.width();

	// Take picture aspect into account
	// TODO: move it into shared place and reuse in SdfIconEngineV2
	QRect const iconRect(QPoint(), iconSize);
	QRect resultingRect = iconRect;
	if (targetHeight * pictureWidth < pictureHeight * targetWidth) {
		resultingRect.setLeft(iconRect.left()
				+ (targetWidth - targetHeight * pictureWidth / pictureHeight) / 2);
		resultingRect.setRight(iconRect.right()
				- (targetWidth - targetHeight * pictureWidth / pictureHeight) / 2);
	} else if (targetHeight * pictureWidth > pictureHeight * targetWidth) {
		resultingRect.setTop(iconRect.top() + (targetHeight
				- targetWidth * pictureHeight / pictureWidth) / 2);
		resultingRect.setBottom(iconRect.bottom()
				- (targetHeight - targetWidth * pictureHeight / pictureWidth) / 2);
	}

	QLabel * const iconLabel = new QLabel;
	iconLabel->setFixedSize(resultingRect.size());
	iconLabel->setScaledContents(true);
	iconLabel->setPixmap(pixmap);

	QVBoxLayout * const circleLayout = new QVBoxLayout;
	circleLayout->setContentsMargins(0, 0, 0, 0);
	circleLayout->setSpacing(0);
	circleLayout->setMargin(0);
	circleLayout->addWidget(iconLabel, 0, Qt::AlignCenter);

	setFixedSize(size);
	setLayout(circleLayout);
}

void CircleWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QColor const backgroundColor = Qt::white;
	QPen borderPen(property("enabled").toBool() ? palette().foreground().color() : backgroundColor);
	borderPen.setWidth(3);
	QBrush const backgroundBrush(backgroundColor);
	painter.setBrush(backgroundBrush);
	painter.setPen(borderPen);
	painter.drawEllipse(QRect(QPoint(2, 2), size() - QSize(4, 4)));
}
