#include <QtGui/QApplication>
#include "splashScreen.h"

using namespace qReal;

const QString SplashScreen::pixmapFilePath = ":/icons/kroki3.PNG";
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
