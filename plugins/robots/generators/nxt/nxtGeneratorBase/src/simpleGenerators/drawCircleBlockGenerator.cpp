#include "drawCircleBlockGenerator.h"

using namespace nxt::simple;

DrawCircleBlockGenerator::DrawCircleBlockGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

void DrawCircleBlockGenerator::drawBmp(QPainter *painter)
{
	int const x = mRepo.property(mId, "XCoordinateCircle").toInt();
	int const y = mRepo.property(mId, "YCoordinateCircle").toInt();
	int const radius = mRepo.property(mId, "CircleRadius").toInt();

	painter->drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}
