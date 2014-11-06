#include "drawPixelBlockGenerator.h"

using namespace nxt::simple;

DrawPixelBlockGenerator::DrawPixelBlockGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

void DrawPixelBlockGenerator::drawBmp(QPainter *painter)
{
	int const x = mRepo.property(mId, "XCoordinatePix").toInt();
	int const y = mRepo.property(mId, "YCoordinatePix").toInt();

	painter->drawPoint(x, y);
}
