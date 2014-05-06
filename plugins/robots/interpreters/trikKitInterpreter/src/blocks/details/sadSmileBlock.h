#pragma once

#include "smileBlockBase.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SadSmileBlock : public SmileBlockBase
{
public:
	explicit SadSmileBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
};

}
}
}
