/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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

#include "hintReporter.h"

#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtGui/QMouseEvent>

#include <qrutils/graphicsUtils/animatedEffects.h>

using namespace qReal;
using namespace gui;
using namespace graphicsUtils;

HintReporter::HintReporter(QWidget *parent, const QString &message, int lifeTime)
	: QLabel(parent)
	, mDuration(lifeTime / 2)  // lifeTime = showing + fading out
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	setAutoFillBackground(false);
	setStyleSheet("background-color: #ffffff; border: 1px solid; border-radius: 7px; border-color: #303030;");
	setMinimumSize(300, 50);
	move(parent->width() - this->width(), 0);
	setWordWrap(true);
	show();
	raise();

	setText(message);

	QTimer::singleShot(mDuration, this, SLOT(disappear()));
	QTimer::singleShot(lifeTime, this, SLOT(deleteLater()));
}

void HintReporter::disappear()
{
	AnimatedEffects::disappear(this, mDuration);
}

void HintReporter::mouseReleaseEvent(QMouseEvent *)
{
	deleteLater();
}
