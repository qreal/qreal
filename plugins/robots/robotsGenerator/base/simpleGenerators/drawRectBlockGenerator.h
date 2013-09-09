#pragma once

#include "drawBlockGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class DrawRectBlockGenerator : public DrawBlockGenerator
{
public:
	DrawRectBlockGenerator(qrRepo::RepoApi const &repo
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
