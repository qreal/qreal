#include "nxtOsekGeneratorCustomizer.h"

using namespace qReal::robots::generators::nxtOsek;

NxtOsekGeneratorCustomizer::NxtOsekGeneratorCustomizer(LogicalModelAssistInterface const &model)
	: mFactory(new NxtOsekGeneratorFactory(model))
{
}

qReal::robots::generators::GeneratorFactoryBase *NxtOsekGeneratorCustomizer::factory()
{
	return mFactory;
}

