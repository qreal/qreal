#include "drawCircleBlockGenerator.h"

using namespace nxt::simple;

DrawCircleBlockGenerator::DrawCircleBlockGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

void DrawCircleBlockGenerator::drawBmp(QPainter *painter)
{
	const int x = mRepo.property(mId, "XCoordinateCircle").toInt();
	const int y = mRepo.property(mId, "YCoordinateCircle").toInt();
	const int radius = mRepo.property(mId, "CircleRadius").toInt();

	painter->drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}
