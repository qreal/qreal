#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "simpleGenerators/abstractSimpleGenerator.h"
#include "simpleGenerators/binding.h"

#include "parts/variables.h"
#include "parts/subprograms.h"
#include "parts/engines.h"
#include "parts/sensors.h"
#include "parts/functions.h"
#include "parts/images.h"

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
	virtual parts::Engines *engines();
	virtual parts::Sensors *sensors();
	virtual parts::Functions *functions();
	virtual parts::Images *images();

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

	virtual simple::AbstractSimpleGenerator *finalNodeGenerator(Id const &id
			, GeneratorCustomizer &customizer, bool fromMainGenerator);

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

	virtual QList<parts::InitTerminateCodeGenerator *> initTerminateGenerators();

	QString initCode();
	QString terminateCode();
	QString isrHooksCode();

protected:
	virtual void initVariables();
	virtual void initSubprograms();
	virtual void initEngines();
	virtual void initSensors();
	virtual void initFunctions();
	virtual void initImages();

private:
	qrRepo::RepoApi const &mRepo;
	ErrorReporterInterface &mErrorReporter;
	parts::Variables *mVariables;
	parts::Subprograms *mSubprograms;
	parts::Engines *mEngines;
	parts::Sensors *mSensors;
	parts::Functions *mFunctions;
	parts::Images *mImages;
};

}
}
}
