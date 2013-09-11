#include "drawCircleBlockGenerator.h"

using namespace qReal::robots::generators::simple;

DrawCircleBlockGenerator::DrawCircleBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
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
