#include "switchGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;

SwitchGenerator::SwitchGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, const QString &part
		, const QStringList &values
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, QString("switch/%1.t").arg(part), {
			Binding::createConverting("@@EXPRESSION@@", "Expression"
					, customizer.factory()->floatPropertyConverter(id, "Expression"))
			, Binding::createConverting("@@CONDITION@@", "Expression"
					, customizer.factory()->switchConditionsMerger(values))
			}, parent)
{
}
