#include "commentElementGenerator.h"

using namespace qReal;
using namespace robots::generators::simple;

CommentElementGenerator::CommentElementGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent = 0)
	: AbstractSimpleGenerator(model, customizer, id, parent)
{
}
