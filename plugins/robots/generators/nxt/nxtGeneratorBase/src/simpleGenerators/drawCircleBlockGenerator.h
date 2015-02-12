#pragma once

#include "drawBlockGenerator.h"

namespace nxt {
namespace simple {

/// Generator for 'DrawCircle' block in NXT kit.
class DrawCircleBlockGenerator : public DrawBlockGenerator
{
public:
	DrawCircleBlockGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
