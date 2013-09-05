#pragma once

#include <qrkernel/ids.h>
#include "generatorFactoryBase.h"

namespace qReal {
namespace robots {
namespace generators {

namespace enums {
namespace semantics {

enum Semantics {
	regularBlock = 0
	, condidionalBlock
	, loopBlock
	, switchBlock
	, forkBlock
};

}
}

class GeneratorCustomizer
{
public:
	// TODO: make it protected
	GeneratorCustomizer();

	virtual bool isInitialNode(Id const &block) const;
	virtual bool isFinalNode(Id const &block) const;

	enums::semantics::Semantics semanticsOf(Id const &block) const;

	void initialize();

	virtual GeneratorFactoryBase *factory() = 0;

protected:
	virtual bool isConditional(Id const &block) const;
	virtual bool isLoop(Id const &block) const;
	virtual bool isSwitch(Id const &block) const;
	virtual bool isFork(Id const &block) const;

private:
	Id const mDefaultInitialBlockType;
	Id const mDefaultFinalBlockType;
	Id const mDefaultConditionalBlockType;
	Id const mDefaultLoopBlockType;
	Id const mDefaultForkBlockType;
};

}
}
}
