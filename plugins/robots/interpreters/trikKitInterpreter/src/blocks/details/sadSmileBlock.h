#pragma once

#include "smileBlockBase.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SadSmileBlock : public SmileBlockBase
{
public:
	explicit SadSmileBlock(robotModel::parts::TrikDisplay &display);
};

}
}
}
