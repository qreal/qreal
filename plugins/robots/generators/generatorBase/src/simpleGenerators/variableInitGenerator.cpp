#include "variableInitGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

VariableInitGenerator::VariableInitGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "variableInit.t", QList<Binding *>()
			<< Binding::createDirect("@@VARIABLE@@", "variable")
			<< Binding::createConverting("@@VALUE@@", "value", customizer.factory()->floatPropertyConverter(id))
			, parent)
{
}
