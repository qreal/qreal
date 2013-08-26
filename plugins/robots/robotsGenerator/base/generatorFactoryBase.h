#pragma once

#include <ids.h>
#include <logicalModelAssistInterface.h>

namespace qReal {
namespace robots {
namespace generators {

class GeneratorFactoryBase
{
public:
	explicit GeneratorFactoryBase(LogicalModelAssistInterface const &model);


private:
	LogicalModelAssistInterface const &mModel;
};

}
}
}
