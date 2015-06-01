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

#include "splashScreen.h"

#include <QtWidgets/QApplication>

using namespace qReal;

const QString SplashScreen::pixmapFilePath = ":/mainWindow/images/kroki3.PNG";
const Qt::WindowFlags SplashScreen::windowFlags = Qt::SplashScreen | Qt::WindowStaysOnTopHint;

SplashScreen::SplashScreen(bool isVisible)
	: QSplashScreen(QPixmap(pixmapFilePath), windowFlags)
	, mProgressBar(new QProgressBar(this))
{
	mProgressBar->move(20, 270);
	mProgressBar->setFixedSize(600, 15);
	mProgressBar->setRange(0, 100);

	setVisible(isVisible);
	QApplication::processEvents();
}

void SplashScreen::setProgress(int progress)
{
	mProgressBar->setValue(progress);
}
