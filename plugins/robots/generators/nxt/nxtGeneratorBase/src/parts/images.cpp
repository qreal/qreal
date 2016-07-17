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

#include "nxtGeneratorBase/parts/images.h"

#include <QtCore/QStringList>

using namespace nxt::parts;

Images::Images(const QStringList &pathsToTemplates)
	: generatorBase::TemplateParametrizedEntity(pathsToTemplates)
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

void Images::registerBmpFile(const QString &name, const QImage &image)
{
	mBmpFiles[name] = image;
}

QString Images::generate() const
{
	QStringList result;
	for (const QString &file : mBmpFiles.keys()) {
		result << readTemplate("drawing/bmpDeclaration.t").replace("@@FILE@@", file);
	}

	return result.join('\n');
}

// TODO: consider moving it into nxt-gen
QString Images::generateBmpFilesStringForMake() const
{
	QString result = "BMP_SOURCES :=";
	for (const QString &file : mBmpFiles.keys()) {
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
