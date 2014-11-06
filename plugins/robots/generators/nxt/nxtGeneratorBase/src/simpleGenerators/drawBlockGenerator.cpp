#include "drawBlockGenerator.h"

#include <generatorBase/parts/variables.h>
#include "src/nxtGeneratorCustomizer.h"
#include "nxtGeneratorBase/parts/images.h"

using namespace nxt::simple;

DrawBlockGenerator::DrawBlockGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawBitmap.t"
			, { generatorBase::simple::Binding::createStatic("@@IMAGE_NAME@@", imageName(customizer)) }
			, parent)
{
}

QString DrawBlockGenerator::generate()
{
	parts::Images &images = static_cast<NxtGeneratorFactory *>(mCustomizer.factory())->images();
	int const width =  images.displayWidth();
	int const height = images.displayHeight();

	QImage image(width, height, QImage::Format_Mono);
	QPainter painter(&image);
	painter.eraseRect(0, 0, width, height);
	drawBmp(&painter);

	QString const name = imageName(mCustomizer);
	images.registerBmpFile(name, image);
	mCustomizer.factory()->variables()->appendManualDeclaration(readTemplate("drawing/variablesDeclaration.t"));

	return BindingGenerator::generate();
}

QString DrawBlockGenerator::imageName(generatorBase::GeneratorCustomizer &customizer) const
{
	int const imagesCount = static_cast<NxtGeneratorFactory *>(customizer.factory())->images().bmpFilesCount();
	return "Image_" + QString::number(imagesCount);
}
