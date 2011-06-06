#pragma once

#include <QtGui/QPainter>

#include "worldModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class WorldDrawer
{
public:
	WorldDrawer(WorldModel const * const worldModel);
	void drawWorld(QPainter &painter);

private:
	WorldModel const * const mWorldModel;
};

}
}
}
}
}
