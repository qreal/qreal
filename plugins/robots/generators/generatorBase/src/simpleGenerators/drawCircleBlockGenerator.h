#pragma once

#include "drawBlockGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'DrawCircle' block
class DrawCircleBlockGenerator : public DrawBlockGenerator
{
public:
	DrawCircleBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
