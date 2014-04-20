#pragma once

#include "drawBlockGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'DrawPixel' block
class DrawPixelBlockGenerator : public DrawBlockGenerator
{
public:
	DrawPixelBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
