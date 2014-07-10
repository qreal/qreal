#pragma once

#include "drawBlockGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'DrawLine' block
class DrawLineBlockGenerator : public DrawBlockGenerator
{
public:
	DrawLineBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
