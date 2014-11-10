#include "waitForEncoderBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

WaitForEncoderBlockGenerator::WaitForEncoderBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/encoder.t",
			{ Binding::createConverting("@@PORT@@", "Port", customizer.factory()->portNameConverter())
			, Binding::createConverting("@@TACHO_LIMIT@@", "TachoLimit"
					, customizer.factory()->intPropertyConverter(id, "TachoLimit"))
			, Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter()) }
			, parent)
{
}
