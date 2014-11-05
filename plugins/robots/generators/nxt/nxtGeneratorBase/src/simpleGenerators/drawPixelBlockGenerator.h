#pragma once

#include "drawBlockGenerator.h"

namespace nxt {
namespace simple {

/// Generator for 'DrawPixel' block in NXT kit.
class DrawPixelBlockGenerator : public DrawBlockGenerator
{
public:
	DrawPixelBlockGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
