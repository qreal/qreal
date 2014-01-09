#pragma once

#include <qrkernel/ids.h>
#include "generatorFactoryBase.h"
#include "robotsGeneratorDeclSpec.h"

namespace qReal {
namespace robots {
namespace generators {

namespace enums {
namespace semantics {

enum Semantics {
	regularBlock = 0
	, finalBlock
	, conditionalBlock
	, loopBlock
	, switchBlock
	, forkBlock
};

}
}

/// This class must be inherited in each concrete generator. Implementation
/// must customize such aspects like blocks-semantics mapping and factory
/// for producing different generator parts.
class ROBOTS_GENERATOR_EXPORT GeneratorCustomizer
{
public:
	// TODO: make it protected
	GeneratorCustomizer();

	/// Implementation must tell if block with given id has initial node semantics
	virtual bool isInitialNode(Id const &block) const;

	/// Implementation must tell if block with given id has final node semantics
	virtual bool isFinalNode(Id const &block) const;

	/// Returns semantics type of the given block
	enums::semantics::Semantics semanticsOf(Id const &block) const;

	/// Initializes everything that couldn`t be initialized in constructor
	/// (for example, everything about pure virtual methods)
	void initialize();

	/// Implementation must return a pointer to a concrete factory for a
	/// concrete generator
	virtual GeneratorFactoryBase *factory() = 0;

protected:
	/// Implementation must tell if block with given id has if-block semantics
	virtual bool isConditional(Id const &block) const;

	/// Implementation must tell if block with given id has loop semantics
	virtual bool isLoop(Id const &block) const;

	/// Implementation must tell if block with given id has switch semantics
	virtual bool isSwitch(Id const &block) const;

	/// Implementation must tell if block with given id has fork semantics
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
