#pragma once

#include "drawBlockGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'DrawRect' block
class DrawRectBlockGenerator : public DrawBlockGenerator
{
public:
	DrawRectBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
