#include "splashScreen.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

using namespace qReal;

QString const SplashScreen::defaultPixmapFilePath = ":/icons/kroki3.PNG";
const Qt::WindowFlags SplashScreen::windowFlags = Qt::SplashScreen | Qt::WindowStaysOnTopHint;

SplashScreen::SplashScreen(bool isVisible)
	: QSplashScreen(QPixmap(), windowFlags)
	, mProgressBar(new QProgressBar(this))
{
	QString const customPixmapPath = QApplication::applicationDirPath() + "/splashscreen.png";
	QString const pixmapPath = QDir().exists(customPixmapPath)
				? customPixmapPath
				: defaultPixmapFilePath;

	setPixmap(QPixmap(pixmapPath).scaled(600, 300));

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
