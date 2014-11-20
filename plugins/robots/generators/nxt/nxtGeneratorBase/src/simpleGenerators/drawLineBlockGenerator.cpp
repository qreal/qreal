#include "drawLineBlockGenerator.h"

using namespace nxt::simple;

DrawLineBlockGenerator::DrawLineBlockGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

void DrawLineBlockGenerator::drawBmp(QPainter *painter)
{
	int const x1 = mRepo.property(mId, "X1CoordinateLine").toInt();
	int const y1 = mRepo.property(mId, "Y1CoordinateLine").toInt();
	int const x2 = mRepo.property(mId, "X2CoordinateLine").toInt();
	int const y2 = mRepo.property(mId, "Y2CoordinateLine").toInt();

	painter->drawLine(x1, y1, x2, y2);
}
