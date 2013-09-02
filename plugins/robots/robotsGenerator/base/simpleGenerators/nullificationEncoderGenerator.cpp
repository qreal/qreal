#include "nullificationEncoderGenerator.h"

using namespace qReal::robots::generators::simple;

NullificationEncoderGenerator::NullificationEncoderGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "nullifyEncoder.t", QList<Binding *>(), parent)
{
}
