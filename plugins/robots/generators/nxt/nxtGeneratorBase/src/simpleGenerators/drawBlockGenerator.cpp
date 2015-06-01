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

#include "drawBlockGenerator.h"

#include <generatorBase/parts/variables.h>
#include "src/nxtGeneratorCustomizer.h"
#include "nxtGeneratorBase/parts/images.h"

using namespace nxt::simple;

DrawBlockGenerator::DrawBlockGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawBitmap.t"
			, { generatorBase::simple::Binding::createStatic("@@IMAGE_NAME@@", imageName(customizer)) }
			, parent)
{
}

QString DrawBlockGenerator::generate()
{
	parts::Images &images = static_cast<NxtGeneratorFactory *>(mCustomizer.factory())->images();
	const int width =  images.displayWidth();
	const int height = images.displayHeight();

	QImage image(width, height, QImage::Format_Mono);
	QPainter painter(&image);
	painter.eraseRect(0, 0, width, height);
	drawBmp(&painter);

	const QString name = imageName(mCustomizer);
	images.registerBmpFile(name, image);
	mCustomizer.factory()->variables()->appendManualDeclaration(readTemplate("drawing/variablesDeclaration.t"));

	return BindingGenerator::generate();
}

QString DrawBlockGenerator::imageName(generatorBase::GeneratorCustomizer &customizer) const
{
	const int imagesCount = static_cast<NxtGeneratorFactory *>(customizer.factory())->images().bmpFilesCount();
	return "Image_" + QString::number(imagesCount);
}
