#pragma once

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

int const displayWidth = 100;
int const displayHeight = 64;

class NxtOSEKRobotGenerator;

class DrawBlockGenerator : public BindingGenerator
{
public:
	DrawBlockGenerator();
};

}
}
}
}
