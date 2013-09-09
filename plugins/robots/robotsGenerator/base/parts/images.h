#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtGui/QImage>

#include "../templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

// TODO: rewrite it with generating code that generates buffer

/// Class responsible for everything with images generation
class Images : public TemplateParametrizedEntity
{
public:
	explicit Images(QString const &pathToTemplates);

	virtual int displayWidth() const;
	virtual int displayHeight() const;

	void reinit();

	QString generate() const;
	QString generateBmpFilesStringForMake() const;

	QMap<QString, QImage> &bmpFiles();
	int bmpFilesCount() const;

	void registerBmpFile(QString const &name, QImage const &image);

private:
	QMap<QString, QImage> mBmpFiles;
};

}
}
}
}
