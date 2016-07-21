/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>

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
class InitTerminateCodeGenerator;
}

namespace lua {
class LuaProcessor;
}

class GeneratorCustomizer;

/// This class must be inherited in each concrete generator. Implementation
/// must specify every generator part (starting from simple block generators
/// and converters to init/terminate code generators and path to templates)
class ROBOTS_GENERATOR_EXPORT GeneratorFactoryBase : public QObject
{
public:
	GeneratorFactoryBase(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, lua::LuaProcessor &luaProcessor);

	virtual ~GeneratorFactoryBase();

	void initialize();

	/// Sets an id of the root diagram for obtaining sensors configuration.
	void setMainDiagramId(const qReal::Id &diagramId);

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

	/// Returns a pointer to an entity processing everything about sensor/device variables.
	virtual parts::DeviceVariables *deviceVariables() const;

	// ----------------------------- Generators --------------------------------

	/// Returns a pointer to a code generator for blocks with if semantics
	virtual simple::AbstractSimpleGenerator *ifGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer
			, bool elseIsEmpty
			, bool needInverting);

	/// Returns a pointer to a code generator for infinite loops
	virtual simple::AbstractSimpleGenerator *infiniteLoopGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for 'while-do' and 'do-while' loops
	virtual simple::AbstractSimpleGenerator *whileLoopGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer
			, bool doWhileForm
			, bool needInverting);

	/// Returns a pointer to a code generator for loops in 'for' form
	virtual simple::AbstractSimpleGenerator *forLoopGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for switch first enumeration block.
	virtual simple::AbstractSimpleGenerator *switchHeadGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer, const QStringList &values);

	/// Returns a pointer to a code generator for switch enumeration block somewhere in the middle.
	virtual simple::AbstractSimpleGenerator *switchMiddleGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer, const QStringList &values);

	/// Returns a pointer to a code generator for switch enumeration block in the end (default case).
	virtual simple::AbstractSimpleGenerator *switchDefaultGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a threads instantiation generator
	virtual simple::AbstractSimpleGenerator *forkCallGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer, const QMap<qReal::Id, QString> &threads);

	/// Returns a pointer to a generator of thread joins
	virtual simple::AbstractSimpleGenerator *joinGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer, const QStringList &joinedThreads, const QString &mainThreadId);

	/// Returns a pointer to a code generator for blocks with regular semantics
	virtual simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for 'break' instruction
	virtual simple::AbstractSimpleGenerator *breakGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for 'continue' instruction
	virtual simple::AbstractSimpleGenerator *continueGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for goto label declaration
	virtual simple::AbstractSimpleGenerator *labelGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for 'goto' instruction
	virtual simple::AbstractSimpleGenerator *gotoSimpleGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer);

	/// Returns a pointer to a code generator for blocks with final-blocks semantics
	virtual simple::AbstractSimpleGenerator *finalNodeGenerator(const qReal::Id &id
			, GeneratorCustomizer &customizer, bool fromMainGenerator);

	/// Returns a pointer to a code generator that replaces all @@RANDOM_ID@@ occurences to random c++ identifier string
	virtual simple::AbstractSimpleGenerator *randomIdGenerator(simple::AbstractSimpleGenerator *other);

	/// Implementation must return a list of paths to folders containing templates for
	/// customizing concrete generator. List must be sorted by folder priority --- generator looks for template
	/// in a first folder, then, if there is no template with such name, it looks in a next folder and so on.
	/// It is needed to be able to redefine only part of templates in different generator plugins.
	virtual QStringList pathsToTemplates() const = 0;

	// ----------------------------- Converters --------------------------------

	/// Produces converter for expressions which should have int type
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *intPropertyConverter(const qReal::Id &block
			, const QString &property) const;

	/// Produces converter for expressions which should have float type
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *floatPropertyConverter(const qReal::Id &block
			, const QString &property) const;

	/// Produces converter for expressions which should have bool type
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *boolPropertyConverter(const qReal::Id &block
			, const QString &property, bool needInverting) const;

	/// Produces converter for expressions which should have string type
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *stringPropertyConverter(const qReal::Id &block
			, const QString &property) const;

	/// Produces a converter that returns an expression that obtain values of system variables
	/// getting its name or the given string othrewise. Transfers ownership.
	virtual simple::Binding::ConverterInterface *reservedVariableNameConverter() const;

	/// Produces converter for transformation a string into valid c++-style identifier
	/// without taking ownership on it
	virtual simple::Binding::ConverterInterface *nameNormalizerConverter() const;

	/// Produces converter for transformation function block code into
	/// generator-dependent code without taking ownership on it
	virtual simple::Binding::ConverterInterface *functionBlockConverter(const qReal::Id &block
			, const QString &property) const;

	/// Produces converter for transformation repo property of the type 'Sign' to
	/// generator-dependent infix inequality sign without taking ownership on it
	virtual simple::Binding::ConverterInterface *inequalitySignConverter() const;

	/// Produces multiconverter for ports specification in engine blocks
	/// without taking ownership on it
	virtual simple::Binding::MultiConverterInterface *enginesConverter() const;

	/// Produces converter for port names without taking ownership on it
	virtual simple::Binding::ConverterInterface *portNameConverter() const;

	/// Produces converter for engine break mode specification without taking ownership on it
	virtual simple::Binding::ConverterInterface *breakModeConverter() const;

	/// Produces converter for variable type specification without taking ownership on it
	virtual simple::Binding::ConverterInterface *typeConverter() const;

	/// Returns a pointer to a converter that makes one composite switch enumeration block from a set
	/// of their values. Accepts an expression that will be compared to @arg values.
	virtual simple::Binding::ConverterInterface *switchConditionsMerger(const QStringList &values) const;

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

	/// Implementation must prepare device variables controller
	virtual void initDeviceVariables();

	/// @todo Why "init*" methods and corresponding "*" methods are both virtual? It is not clear what we supposed
	/// to override in concrete generators?

	/// Returns sensors configuration for the given robot model.
	QMap<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> currentConfiguration() const;

	const qrRepo::RepoApi &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	lua::LuaProcessor &mLuaTranslator;
	qReal::Id mDiagram;
	parts::Variables *mVariables;
	parts::Subprograms *mSubprograms;
	parts::Threads *mThreads;
	parts::Engines *mEngines;
	parts::Sensors *mSensors;
	parts::Functions *mFunctions;
	parts::DeviceVariables *mDeviceVariables;  // Has ownership.
};

}
