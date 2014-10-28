#include "switchGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;

SwitchGenerator::SwitchGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QString const &part
		, QStringList const &values
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, QString("switch/%1.t").arg(part), {
			Binding::createConverting("@@EXPRESSION@@", "Expression"
					, customizer.factory()->floatPropertyConverter(id))
			, Binding::createConverting("@@CONDITION@@", "Expression"
					, customizer.factory()->switchConditionsMerger(values))
			}, parent)
{
}
