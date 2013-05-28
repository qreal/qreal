#include "imageGenerator.h"

ImageGenerator::ImageGenerator()
{
	reinit();
}

void ImageGenerator::addBmpFileName(QString name)
{
	mBmpFileNames.append(name);
}

QString ImageGenerator::generateBmpFilesStringForC() const
{
	QString result = "";
	foreach(QString const &file, mBmpFileNames) {
		result += QString("EXTERNAL_BMP_DATA(%1);\n").arg(file);
	}

	return result;
}

QString ImageGenerator::generateBmpFilesStringForMake() const
{
	QString result = "BMP_SOURCES :=";

	foreach(QString const &file, mBmpFileNames) {
		result += QString("\\\n%1.bmp").arg(file);
	}

	return result;
}

void ImageGenerator::increaseBmpCounter()
{
	mBmpFilesCounter++;
}

int ImageGenerator::bmpFilesNumber() const
{
	return mBmpFilesCounter;
}

void ImageGenerator::reinit()
{
	mBmpFilesCounter = 0;
	mBmpFileNames.clear();
}
