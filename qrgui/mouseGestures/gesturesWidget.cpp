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

	if (mUi->listWidget->count() > 0) {
		mUi->listWidget->setCurrentRow(0);
	}
}
