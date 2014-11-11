#pragma once

#include "drawBlockGenerator.h"

namespace nxt {
namespace simple {

/// Generator for 'DrawLine' block in NXT kit.
class DrawLineBlockGenerator : public DrawBlockGenerator
{
public:
	DrawLineBlockGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
