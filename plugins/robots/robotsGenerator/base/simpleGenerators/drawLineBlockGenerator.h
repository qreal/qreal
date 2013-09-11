#pragma once

#include "drawBlockGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class DrawLineBlockGenerator : public DrawBlockGenerator
{
public:
	DrawLineBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(QPainter *painter);
};

}
}
}
}
