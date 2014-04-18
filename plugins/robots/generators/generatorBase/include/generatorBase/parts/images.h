#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtGui/QImage>

#include "generatorBase/robotsGeneratorDeclSpec.h"
#include "generatorBase/templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

// TODO: rewrite it with generating code that generates buffer
// TODO: move this class into NxtOSEK-only generator

/// Class responsible for everything with images generation
class ROBOTS_GENERATOR_EXPORT Images : public TemplateParametrizedEntity
{
public:
	explicit Images(QString const &pathToTemplates);

	/// Returns target robot display width in pixels
	virtual int displayWidth() const;

	/// Returns target robot display height in pixels
	virtual int displayHeight() const;

	/// Prepares this instance for new usage
	void reinit();

	/// Generates code for declaration resulting BMPs inside the program
	QString generate() const;

	// TODO: move it into osek
	/// Generates code for declaration resulting BMPs inside the makefiles
	QString generateBmpFilesStringForMake() const;

	/// Returns a resulting set of BMPs (paths mapped into the images themselves)
	QMap<QString, QImage> &bmpFiles();

	/// Returns the count of registered in the moment of query BMPs
	int bmpFilesCount() const;

	/// Adds given imageto the resulting BMP set
	void registerBmpFile(QString const &name, QImage const &image);

private:
	QMap<QString, QImage> mBmpFiles;
};

}
}
}
}
