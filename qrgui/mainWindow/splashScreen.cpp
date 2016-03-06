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

#include <QtCore/QFile>
#include <QtWidgets/QApplication>
#include <QtWidgets/QProgressBar>

#include <qrkernel/platformInfo.h>

using namespace qReal;

const QString defaultImage = ":/mainWindow/images/kroki3.PNG";
const Qt::WindowFlags windowFlags = Qt::SplashScreen | Qt::WindowStaysOnTopHint;

SplashScreen::SplashScreen(bool isVisible)
	: QSplashScreen()
	, mProgressBar(new QProgressBar(this))
{
	const QString customScreen = PlatformInfo::applicationDirPath() + "/splashscreen.png";
	const QString pixmapFilePath = QFile::exists(customScreen) ? customScreen : defaultImage;

	setPixmap(QPixmap(pixmapFilePath).scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	setWindowFlags(windowFlags());

	mProgressBar->move(20, height() - 30);
	mProgressBar->setFixedSize(width() - 40, 15);
	mProgressBar->setRange(0, 100);

	setVisible(isVisible);
	QApplication::processEvents();
}

void SplashScreen::setProgress(int progress)
{
	mProgressBar->setValue(progress);
}
