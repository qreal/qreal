#include "nxtGeneratorBase/parts/images.h"

#include <QtCore/QStringList>

using namespace nxt::parts;

Images::Images(QString const &pathToTemplates)
	: generatorBase::TemplateParametrizedEntity(pathToTemplates)
{
	reinit();
}

int Images::displayWidth() const
{
	return 100;
}

int Images::displayHeight() const
{
	return 64;
}

void Images::registerBmpFile(QString const &name, QImage const &image)
{
	mBmpFiles[name] = image;
}

QString Images::generate() const
{
	QStringList result;
	foreach (QString const &file, mBmpFiles.keys()) {
		result << readTemplate("drawing/bmpDeclaration.t").replace("@@FILE@@", file);
	}

	return result.join('\n');
}

// TODO: consider moving it into nxt-gen
QString Images::generateBmpFilesStringForMake() const
{
	QString result = "BMP_SOURCES :=";
	foreach (QString const &file, mBmpFiles.keys()) {
		result += QString("\\\n%1.bmp").arg(file);
	}

	return result;
}

QMap<QString, QImage> &Images::bmpFiles()
{
	return mBmpFiles;
}

int Images::bmpFilesCount() const
{
	return mBmpFiles.count();
}

void Images::reinit()
{
	mBmpFiles.clear();
}
