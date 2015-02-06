#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtGui/QImage>

#include <generatorBase/robotsGeneratorDeclSpec.h>
#include <generatorBase/templateParametrizedEntity.h>

#include "nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h"

namespace nxt {
namespace parts {

// TODO: rewrite it with generating code that generates buffer

/// Class responsible for everything with images generation
class ROBOTS_NXT_GENERATOR_BASE_EXPORT Images : public generatorBase::TemplateParametrizedEntity
{
public:
	explicit Images(const QString &pathToTemplates);

	/// Returns target robot display width in pixels
	int displayWidth() const;

	/// Returns target robot display height in pixels
	int displayHeight() const;

	/// Prepares this instance for new usage
	void reinit();

	/// Generates code for declaration resulting BMPs inside the program
	QString generate() const;

	/// Generates code for declaration resulting BMPs inside the makefiles
	QString generateBmpFilesStringForMake() const;

	/// Returns a resulting set of BMPs (paths mapped into the images themselves)
	QMap<QString, QImage> &bmpFiles();

	/// Returns the count of registered in the moment of query BMPs
	int bmpFilesCount() const;

	/// Adds given imageto the resulting BMP set
	void registerBmpFile(const QString &name, QImage const &image);

private:
	QMap<QString, QImage> mBmpFiles;
};

}
}
