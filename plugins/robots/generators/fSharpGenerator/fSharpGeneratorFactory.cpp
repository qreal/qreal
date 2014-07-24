#include "fSharpGeneratorFactory.h"
#include <generatorBase/converters/regexpMultiConverter.h>
#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>
#include "converters/engineV4PortConverter.h"
#include "converters/engineV6PortConverter.h"
#include "converters/encoderV4PortConverter.h"
#include "converters/encoderV6PortConverter.h"
#include "converters/fSharpStringPropertyConverter.h"
#include "simpleGenerators/waitForInfraredSensorGenerator.h"
#include "simpleGenerators/waitForMotionGenerator.h"
#include "simpleGenerators/fSharpEnginesGenerator.h"
#include "simpleGenerators/fSharpEnginesStopGenerator.h"
#include "simpleGenerators/fSharpNullificationEncoderGenerator.h"
#include "parts/fSharpVariables.h"

using namespace fSharp;
using namespace fSharp::simple;
using namespace generatorBase;
using namespace generatorBase::simple;

FSharpGeneratorFactory::FSharpGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager)
{
}

FSharpGeneratorFactory::~FSharpGeneratorFactory()
{
}

AbstractSimpleGenerator *FSharpGeneratorFactory::simpleGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();
	if (elementType.contains("EnginesForward")
		|| elementType.contains("EnginesBackward")
		|| elementType.contains("AngularServo")
	)
	{
		return new FSharpEnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType.contains("EnginesStop")) {
		return new FSharpEnginesStopGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("ClearEncoder")) {
		return new FSharpNullificationEncoderGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitForEnter") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForEnter.t", this);
	} else if (elementType == "TrikWaitForLeft") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForLeft.t", this);
	} else if (elementType == "TrikWaitForRight") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForRight.t", this);
	} else if (elementType == "TrikWaitForUp") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForUp.t", this);
	} else if (elementType == "TrikWaitForDown") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForDown.t", this);
	} else if (elementType == "TrikWaitForPower") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForPower.t", this);
	} else if (elementType == "TrikWaitForMotion") {
		return new WaitForMotionGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitForIRDistance") {
		return new WaitForInfraredSensorGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString FSharpGeneratorFactory::pathToTemplates() const
{
	return ":/FSharp/templates";
}

Binding::MultiConverterInterface *FSharpGeneratorFactory::enginesConverter() const
{
	return new generatorBase::converters::RegexpMultiConverter(motorPortSplitRegexp(), motorPortConverter());
}

Binding::MultiConverterInterface *FSharpGeneratorFactory::encodersConverter() const
{
	return new generatorBase::converters::RegexpMultiConverter(motorPortSplitRegexp(), encoderPortConverter());
}

Binding::ConverterInterface *FSharpGeneratorFactory::inputPortConverter() const
{
	return new Binding::EmptyConverter;
}

Binding::ConverterInterface *FSharpGeneratorFactory::outputPortConverter() const
{
	return encoderPortConverter();
}

generatorBase::simple::Binding::ConverterInterface *FSharpGeneratorFactory::stringPropertyConverter() const
{
	return new converters::FSharpStringPropertyConverter(*mVariables);
}

void FSharpGeneratorFactory::initVariables()
{
	mVariables = new parts::FSharpVariables(pathToTemplates(), mRobotModelManager.model());
}

Binding::ConverterInterface *FSharpGeneratorFactory::motorPortConverter() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return new converters::PowerV4MotorPortConverter;
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return new converters::PowerV6MotorPortConverter;
	}

	/// @todo: Inconsistent scenario
	return new converters::PowerV6MotorPortConverter;
}

Binding::ConverterInterface *FSharpGeneratorFactory::encoderPortConverter() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return new converters::EncoderV4PortConverter;
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return new converters::EncoderV6PortConverter;
	}

	/// @todo: Inconsistent scenario
	return new converters::EncoderV6PortConverter;
}

QString FSharpGeneratorFactory::motorPortSplitRegexp() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return converters::PowerV4MotorPortConverter::splitRegexp();
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return converters::PowerV6MotorPortConverter::splitRegexp();
	}

	/// @todo: Inconsistent scenario
	return converters::PowerV6MotorPortConverter::splitRegexp();
}
