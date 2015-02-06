#pragma once

#include "drawBlockGenerator.h"

namespace nxt {
namespace simple {

/// Generator for 'DrawPixel' block in NXT kit.
class DrawPixelBlockGenerator : public DrawBlockGenerator
{
public:
	DrawPixelBlockGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
