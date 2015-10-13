/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	explicit Images(const QStringList &pathsToTemplates);

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
	void registerBmpFile(const QString &name, const QImage &image);

private:
	QMap<QString, QImage> mBmpFiles;
};

}
}
