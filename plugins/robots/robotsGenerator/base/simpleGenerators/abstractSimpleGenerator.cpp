#include "abstractSimpleGenerator.h"

using namespace qReal::robots::generators::simple;

AbstractSimpleGenerator::AbstractSimpleGenerator(LogicalModelAssistInterface const &model
		, QObject *parent)
	: QObject(parent)
	, mModel(model)
	, mId(id)
{
}

AbstractSimpleGenerator::~AbstractSimpleGenerator()
{
}
