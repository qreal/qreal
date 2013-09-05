#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "simpleGenerators/abstractSimpleGenerator.h"
#include "simpleGenerators/binding.h"

#include "parts/variables.h"
#include "parts/subprograms.h"

namespace qReal {
namespace robots {
namespace generators {

class GeneratorCustomizer;

class GeneratorFactoryBase : public QObject
{
public:
	GeneratorFactoryBase(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

	virtual ~GeneratorFactoryBase();

	void initialize();

	/// Immutable version of variables()
	virtual parts::Variables *variablesInfo() const;
	virtual parts::Variables *variables();
	virtual parts::Subprograms *subprograms();

	virtual simple::AbstractSimpleGenerator *ifGenerator(Id const &id
			, GeneratorCustomizer &customizer
			, bool elseIsEmpty
			, bool needInverting);

	virtual simple::AbstractSimpleGenerator *infiniteLoopGenerator(Id const &id
			, GeneratorCustomizer &customizer);

	virtual simple::AbstractSimpleGenerator *whileLoopGenerator(Id const &id
			, GeneratorCustomizer &customizer
			, bool doWhileForm
			, bool needInverting);

	virtual simple::AbstractSimpleGenerator *forLoopGenerator(Id const &id
			, GeneratorCustomizer &customizer);

	virtual simple::AbstractSimpleGenerator *simpleGenerator(Id const &id
			, GeneratorCustomizer &customizer);

	virtual simple::AbstractSimpleGenerator *breakGenerator(Id const &id
			, GeneratorCustomizer &customizer);
	virtual simple::AbstractSimpleGenerator *continueGenerator(Id const &id
			, GeneratorCustomizer &customizer);

	virtual QString pathToTemplates() const = 0;

	virtual simple::Binding::ConverterInterface *intPropertyConverter() const;
	virtual simple::Binding::ConverterInterface *floatPropertyConverter() const;
	virtual simple::Binding::ConverterInterface *boolPropertyConverter(bool needInverting) const;
	virtual simple::Binding::ConverterInterface *stringPropertyConverter() const;
	virtual simple::Binding::ConverterInterface *nameNormalizerConverter() const;
	virtual simple::Binding::ConverterInterface *functionInvocationConverter() const;
	virtual simple::Binding::ConverterInterface *functionBlockConverter() const;
	virtual simple::Binding::ConverterInterface *inequalitySignConverter() const;
	virtual simple::Binding::MultiConverterInterface *enginesConverter() const;
	virtual simple::Binding::ConverterInterface *inputPortConverter() const;
	virtual simple::Binding::ConverterInterface *outputPortConverter() const;
	virtual simple::Binding::ConverterInterface *colorConverter() const;
	virtual simple::Binding::ConverterInterface *breakModeConverter() const;
	virtual simple::Binding::ConverterInterface *typeConverter() const;

protected:
	virtual void initVariables();
	virtual void initSubprograms();

private:
	qrRepo::RepoApi const &mRepo;
	ErrorReporterInterface &mErrorReporter;
	parts::Variables *mVariables;
	parts::Subprograms *mSubprograms;
};

}
}
}
