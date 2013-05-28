#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

class ImageGenerator
{
public:
	ImageGenerator();
	void addBmpFileName(QString name);
	QString generateBmpFilesStringForC() const;
	QString generateBmpFilesStringForMake() const;
	void increaseBmpCounter();
	int bmpFilesNumber() const;
	void reinit();

private:
	QList<QString> mBmpFileNames;
	int mBmpFilesCounter;
};

