#pragma once
#include <QtGui/QProgressBar>
#include <QSplashScreen>

namespace qReal {

class SplashScreen : public QSplashScreen
{
	Q_OBJECT

public:
	explicit SplashScreen(bool isVisible);
	~SplashScreen();

	void setProgress(int progress);

	static const QString pixmapFilePath;
	static const Qt::WindowFlags windowFlags;

private:
	QProgressBar *mProgressBar;
};

}
