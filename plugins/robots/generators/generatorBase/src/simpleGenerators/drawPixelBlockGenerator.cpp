#include "drawPixelBlockGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

DrawPixelBlockGenerator::DrawPixelBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
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
