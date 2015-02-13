#include "drawRectBlockGenerator.h"

using namespace nxt::simple;

DrawRectBlockGenerator::DrawRectBlockGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

void DrawRectBlockGenerator::drawBmp(QPainter *painter)
{
	const int x = mRepo.property(mId, "XCoordinateRect").toInt();
	const int y = mRepo.property(mId, "YCoordinateRect").toInt();
	const int width = mRepo.property(mId, "WidthRect").toInt();
	const int height = mRepo.property(mId, "HeightRect").toInt();

	painter->drawRect(x, y, width, height);
}
