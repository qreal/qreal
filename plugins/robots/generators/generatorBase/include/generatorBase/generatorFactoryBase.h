#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

#include "robotsGeneratorDeclSpec.h"

#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/parts/deviceVariables.h"

namespace generatorBase {

namespace parts {
class Variables;
class Subprograms;
class Threads;
class Engines;
class Sensors;
class Functions;
class Images;
class InitTerminateCodeGenerator;
}

class GeneratorCustomizer;

/// This class must be inherited in each concrete generator. Implementation
/// must specify every generator part (starting from simple block generators
/// and converters to init/terminate code generators and path to templates)
class ROBOTS_GENERATOR_EXPORT GeneratorFactoryBase : public QObject
{
public:
	GeneratorFactoryBase(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager);

	virtual ~GeneratorFactoryBase();

	void initialize();

	/// Sets an id of the root diagram for obtaining sensors configuration.
	void setMainDiagramId(qReal::Id const &diagramId);

	// ------------------------------- Parts -----------------------------------

	/// Immutable version of variables()
	virtual parts::Variables *variablesInfo() const;

	/// Returns a pointer to an entity processing everything about variables
	virtual parts::Variables *variables();

	/// Returns a pointer to an entity processing everything about subprograms
	virtual parts::Subprograms *subprograms();

	/// Returns a pointer to a global threads storage
	parts::Threads &threads();

	/// Returns a pointer to an entity processing everything about engines usage
	virtual parts::Engines *engines();

	/// Returns a pointer to an entity processing everything about sensors
	virtual parts::Sensors *sensors();

	/// Returns a pointer to an entity processing everything about functions
	/// with property 'Initialization' set to 'true'
	virtual parts::Functions *functions();

	/// Returns a pointer to an entity processing everything about images
	virtual parts::Images *images();

	/// Returns a pointer to an entity processing everything about sensor/device variables.
	virtual parts::DeviceVariables *deviceVariables() const;

	// ----------------------------- Generators --------------------------------

	/// Returns a pointer to a code generator for blocks with if semantics
	virtual simple::AbstractSimpleGenerator *ifGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer
			, bool elseIsEmpty
			, bool needInverting);

	/// Returns a pointer to a code generator for infinite loops
	virtual simple::AbstractSimpleGenerator *infiniteLoopGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for 'while-do' and 'do-while' loops
	virtual simple::AbstractSimpleGenerator *whileLoopGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer
			, bool doWhileForm
			, bool needInverting);

	/// Returns a pointer to a code generator for loops in 'for' form
	virtual simple::AbstractSimpleGenerator *forLoopGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a threads instantiation generator
	virtual simple::AbstractSimpleGenerator *forkCallGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer, qReal::IdList const &threads);

	/// Returns a pointer to a code generator for blocks with regular semantics
	virtual simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for 'break' instruction
	virtual simple::AbstractSimpleGenerator *breakGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for 'continue' instruction
	virtual simple::AbstractSimpleGenerator *continueGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for goto label declaration
	virtual simple::AbstractSimpleGenerator *labelGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for 'goto' instruction
	virtual simple::AbstractSimpleGenerator *gotoSimpleGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for blocks with final-blocks semantics
	virtual simple::AbstractSimpleGenerator *finalNodeGenerator(qReal::Id const &id
			, GeneratorCustomizer &customizer, bool fromMainGenerator);

	/// Implementation must return a path to a folder containing templates for
	/// customizing concrete generator
	virtual QString pathToTemplates() const = 0;

	// ----------------------------- Converters --------------------------------

	/// Produces converter for expressions which should have int type
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *intPropertyConverter() const;

	/// Produces converter for expressions which should have float type
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *floatPropertyConverter() const;

	/// Produces converter for expressions which should have bool type
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *boolPropertyConverter(bool needInverting) const;

	/// Produces converter for expressions which should have string type
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *stringPropertyConverter() const;

	/// Produces a converter that returns an expression that obtain values of system variables
	/// getting its name or the given string othrewise. Transfers ownership.
	virtual simple::Binding::ConverterInterface *systemVariableNameConverter() const;

	/// Produces converter for transformation a string into valid c++-style identifier
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *nameNormalizerConverter() const;

	/// Produces converter for replacing different function invocations with
	/// generator-dependent code without taking ownership on it
	virtual simple::Binding::ConverterInterface *functionInvocationConverter() const;

	/// Produces converter for transformation function block code into
	/// generator-dependent code without taking ownership on it
	virtual simple::Binding::ConverterInterface *functionBlockConverter() const;

	/// Produces converter for transformation repo property of the type 'Sign' to
	/// generator-dependent infix inequality sign without taking ownership on it
	virtual simple::Binding::ConverterInterface *inequalitySignConverter() const;

	/// Produces multiconverter for ports specification in engine blocks
	/// without taking ownership on it
	virtual simple::Binding::MultiConverterInterface *enginesConverter() const;

	/// Produces converter for sensor port specification without taking ownership on it
	virtual simple::Binding::ConverterInterface *inputPortConverter() const;

	/// Produces converter for engine port specification without taking ownership on it
	virtual simple::Binding::ConverterInterface *outputPortConverter() const;

	/// Produces converter for color specification without taking ownership on it
	virtual simple::Binding::ConverterInterface *colorConverter() const;

	/// Produces converter for engine break mode specification without taking ownership on it
	virtual simple::Binding::ConverterInterface *breakModeConverter() const;

	/// Produces converter for variable type specification without taking ownership on it
	virtual simple::Binding::ConverterInterface *typeConverter() const;

	// ------------------------- Init-terminate code ---------------------------

	/// Returns a list of all init-terminate code generators used in system
	virtual QList<parts::InitTerminateCodeGenerator *> initTerminateGenerators();

	/// Returns generated initialization code. Calling this method has sense
	/// only after generation process was successfully finished
	QString initCode();

	/// Returns generated termination code. Calling this method has sense
	/// only after generation process was successfully finished
	QString terminateCode();

	/// Returns generated ISR-hooks code. Calling this method has sense
	/// only after generation process was successfully finished
	QString isrHooksCode();

protected:
	/// Implementation must prepare variables controller
	virtual void initVariables();

	/// Implementation must prepare supbrograms controller
	virtual void initSubprograms();

	/// Implementation must prepare engines usage controller
	virtual void initEngines();

	/// Implementation must prepare sensors usage controller
	virtual void initSensors();

	/// Implementation must prepare function initialization code controller
	virtual void initFunctions();

	/// Implementation must prepare images controller
	virtual void initImages();

	/// Implementation must prepare device variables controller
	virtual void initDeviceVariables();

	/// @todo Why "init*" methods and corresponding "*" methods are both virtual? It is not clear what we supposed
	/// to override in concrete generators?

	/// Returns sensors configuration for the given robot model.
	QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> currentConfiguration() const;

	qrRepo::RepoApi const &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	interpreterBase::robotModel::RobotModelManagerInterface const &mRobotModelManager;
	qReal::Id mDiagram;
	parts::Variables *mVariables;
	parts::Subprograms *mSubprograms;
	parts::Threads *mThreads;
	parts::Engines *mEngines;
	parts::Sensors *mSensors;
	parts::Functions *mFunctions;
	parts::Images *mImages;
	parts::DeviceVariables *mDeviceVariables;  // Has ownership.
};

}
