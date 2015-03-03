#pragma once

#include <qrkernel/ids.h>
#include "generatorFactoryBase.h"
#include "robotsGeneratorDeclSpec.h"

namespace generatorBase {

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
	virtual ~GeneratorCustomizer() {}

	/// Implementation must tell if block with given id has initial node semantics
	virtual bool isInitialNode(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has final node semantics
	virtual bool isFinalNode(const qReal::Id &block) const;

	/// Implementation must tell if block with given id calls some new routine
	virtual bool isSubprogramCall(const qReal::Id &block) const;

	/// Returns semantics type of the given block
	enums::semantics::Semantics semanticsOf(const qReal::Id &block) const;

	/// Initializes everything that couldn`t be initialized in constructor
	/// (for example, everything about pure virtual methods)
	void initialize();

	/// Implementation must return a pointer to a concrete factory for a
	/// concrete generator
	virtual GeneratorFactoryBase *factory() = 0;

protected:
	GeneratorCustomizer();

	/// Implementation must tell if block with given id has if-block semantics
	virtual bool isConditional(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has loop semantics
	virtual bool isLoop(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has switch semantics
	virtual bool isSwitch(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has fork semantics
	virtual bool isFork(const qReal::Id &block) const;

private:
	const qReal::Id mDefaultInitialBlockType;
	const qReal::Id mDefaultFinalBlockType;
	const qReal::Id mDefaultConditionalBlockType;
	const qReal::Id mDefaultLoopBlockType;
	const qReal::Id mDefaultSwitchBlockType;
	const qReal::Id mDefaultForkBlockType;
	const qReal::Id mDefaultSubprogramCallBlockType;
};

}
