#include "drawLineBlockGenerator.h"

using namespace nxt::simple;

DrawLineBlockGenerator::DrawLineBlockGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

void DrawLineBlockGenerator::drawBmp(QPainter *painter)
{
	const int x1 = mRepo.property(mId, "X1CoordinateLine").toInt();
	const int y1 = mRepo.property(mId, "Y1CoordinateLine").toInt();
	const int x2 = mRepo.property(mId, "X2CoordinateLine").toInt();
	const int y2 = mRepo.property(mId, "Y2CoordinateLine").toInt();

	painter->drawLine(x1, y1, x2, y2);
}
