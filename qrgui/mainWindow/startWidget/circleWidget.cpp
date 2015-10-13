/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "circleWidget.h"

#include <QtCore/QVariant>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QPainter>

using namespace qReal;

CircleWidget::CircleWidget(const QSize &size, const QString &icon, QWidget *parent)
	: QWidget(parent)
{
	const QSize iconSize = size - QSize(25, 25);

	const QPixmap pixmap(icon);
	const int targetHeight = iconSize.height();
	const int targetWidth = iconSize.width();

	const int pictureHeight = pixmap.height();
	const int pictureWidth = pixmap.width();

	// Take picture aspect into account
	// TODO: move it into shared place and reuse in SdfIconEngineV2
	const QRect iconRect(QPoint(), iconSize);
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

	const QColor backgroundColor = Qt::white;
	QPen borderPen(property("enabled").toBool() ? palette().foreground().color() : backgroundColor);
	borderPen.setWidth(3);
	const QBrush backgroundBrush(backgroundColor);
	painter.setBrush(backgroundBrush);
	painter.setPen(borderPen);
	painter.drawEllipse(QRect(QPoint(2, 2), size() - QSize(4, 4)));
}
