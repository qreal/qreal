#include "drawPixelBlockGenerator.h"

using namespace nxt::simple;

DrawPixelBlockGenerator::DrawPixelBlockGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

void DrawPixelBlockGenerator::drawBmp(QPainter *painter)
{
	const int x = mRepo.property(mId, "XCoordinatePix").toInt();
	const int y = mRepo.property(mId, "YCoordinatePix").toInt();

	painter->drawPoint(x, y);
}
