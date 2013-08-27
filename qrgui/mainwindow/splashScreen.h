#pragma once

#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSplashScreen>

namespace qReal {

/**
* @brief Splash screen with progress bar
*/

class SplashScreen : public QSplashScreen
{
	Q_OBJECT

public:
	/// @param isVisible This flag allows to disable the splash screen
	/// without the need to include a large number of checks in MainWindow
	explicit SplashScreen(bool isVisible = true);
	void setProgress(int progress);

	static QString const pixmapFilePath;
	static const Qt::WindowFlags windowFlags;

private:
	QProgressBar *mProgressBar;
};

}
