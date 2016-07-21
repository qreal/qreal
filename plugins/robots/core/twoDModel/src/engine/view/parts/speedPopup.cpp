/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

#include "speedPopup.h"

#include <QtCore/QTimer>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

using namespace twoDModel::view;

const int timeToLive = 3000;
const QString pattern = QObject::tr("Speed:&nbsp;&nbsp;<b>%1%</b>");

SpeedPopup::SpeedPopup(QWidget *parent)
	: QWidget(parent)
	, mText(new QLabel(this))
	, mResetButton(new QPushButton(tr("Reset to default"), this))
	, mHideTimer(new QTimer(this))
{
	setAutoFillBackground(true);
	QVBoxLayout * const layout = new QVBoxLayout(this);
	mText->setAlignment(Qt::AlignCenter);
	connect(mResetButton, &QAbstractButton::clicked, this, &SpeedPopup::resetToDefault);
	layout->addWidget(mText);
	layout->addWidget(mResetButton);
	mHideTimer->setInterval(2000);
	mHideTimer->setSingleShot(true);
	connect(mHideTimer, &QTimer::timeout, this, &SpeedPopup::hide);
	updateDueToLayout();
	hide();
}

void SpeedPopup::setSpeed(int speed)
{
	mText->setText(pattern.arg(QString::number(speed)));
	mResetButton->setEnabled(speed != 100);
	updateDueToLayout();
	show();
	mHideTimer->stop();
	mHideTimer->start();
}

void SpeedPopup::updateDueToLayout()
{
	setFixedSize(layout()->sizeHint());
}
