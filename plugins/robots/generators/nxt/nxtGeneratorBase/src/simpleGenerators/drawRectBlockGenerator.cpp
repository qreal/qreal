#include "drawRectBlockGenerator.h"

using namespace nxt::simple;

DrawRectBlockGenerator::DrawRectBlockGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

void DrawRectBlockGenerator::drawBmp(QPainter *painter)
{
	int const x = mRepo.property(mId, "XCoordinateRect").toInt();
	int const y = mRepo.property(mId, "YCoordinateRect").toInt();
	int const width = mRepo.property(mId, "WidthRect").toInt();
	int const height = mRepo.property(mId, "HeightRect").toInt();

	painter->drawRect(x, y, width, height);
}
