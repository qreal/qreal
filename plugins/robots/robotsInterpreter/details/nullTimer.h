#pragma once

#include <QtCore/QTimer>

#include "abstractTimer.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{

class NullTimer : public AbstractTimer
{
public:
	virtual void start(int ms);
};

}
}
}
}