#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Implementation of AbstractElementGenerator for Subprogram.
class SubprogramGenerator : public AbstractElementGenerator
{
public:
	SubprogramGenerator(NxtOSEKRobotGenerator *generator
			, qReal::Id const &elementId);
};

}
}
