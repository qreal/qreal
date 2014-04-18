#include "drawBlockGenerator.h"

#include "generatorBase/generatorCustomizer.h"
#include "src/parts/images.h"
#include "src/parts/variables.h"

using namespace qReal::robots::generators::simple;

DrawBlockGenerator::DrawBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawBitmap.t"
			, QList<Binding *>() << Binding::createStatic("@@IMAGE_NAME@@", imageName(customizer))
			, parent)
{
}

QString DrawBlockGenerator::generate()
{
	int const width = mCustomizer.factory()->images()->displayWidth();
	int const height = mCustomizer.factory()->images()->displayHeight();

	QImage image(width, height, QImage::Format_Mono);
	QPainter painter(&image);
	painter.eraseRect(0, 0, width, height);
	drawBmp(&painter);

	QString const name = imageName(mCustomizer);
	mCustomizer.factory()->images()->registerBmpFile(name, image);
	mCustomizer.factory()->variables()->appendManualDeclaration(readTemplate("drawing/variablesDeclaration.t"));

	return BindingGenerator::generate();
}

QString DrawBlockGenerator::imageName(GeneratorCustomizer &customizer) const
{
	return "Image_" + QString::number(customizer.factory()->images()->bmpFilesCount());
}
