#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

namespace robots {
namespace generator {

/// Class responsible for everything with images generation
class ImageGenerator
{
public:
	ImageGenerator();

	void addBmpFileName(QString const &name);
	QString generateBmpFilesStringForC() const;
	QString generateBmpFilesStringForMake() const;
	void increaseBmpCounter();
	int bmpFilesCount() const;
	void reinit();

private:
	QList<QString> mBmpFileNames;
	int mBmpFilesCounter;
};

}
}
