#pragma once

#include "drawBlockGenerator.h"

namespace nxt {
namespace simple {

/// Generator for 'DrawRect' block in NXT kit.
class DrawRectBlockGenerator : public DrawBlockGenerator
{
public:
	DrawRectBlockGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
