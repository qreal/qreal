#pragma once
#include <QtGui/QProgressBar>
#include <QtGui/QSplashScreen>

namespace qReal {

class SplashScreen : public QSplashScreen
{
	Q_OBJECT

public:
	explicit SplashScreen(bool isVisible = true);
	void setProgress(int progress);

	static const QString pixmapFilePath;
	static const Qt::WindowFlags windowFlags;

private:
	QProgressBar *mProgressBar;
};

}
