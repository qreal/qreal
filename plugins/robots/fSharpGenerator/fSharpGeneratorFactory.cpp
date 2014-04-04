#include "fSharpGeneratorFactory.h"
#include <converters/regexpMultiConverter.h>
#include "converters/servoMotorPortConverter.h"
#include "converters/powerMotorPortConverter.h"
#include "converters/functionFSharpBlockConverter.h"
#include "simpleGenerators/detectLineGenerator.h"
#include "simpleGenerators/initCameraGenerator.h"
#include "simpleGenerators/ledGenerator.h"
#include "simpleGenerators/lineDetectorToVariableGenerator.h"
#include "simpleGenerators/playToneGenerator.h"
#include "simpleGenerators/sadSmileGenerator.h"
#include "simpleGenerators/sayGenerator.h"
#include "simpleGenerators/setBackgroundGenerator.h"
#include "simpleGenerators/smileGenerator.h"
#include "simpleGenerators/systemGenerator.h"
#include "simpleGenerators/fSharpEnginesGenerator.h"
#include "simpleGenerators/fSharpEnginesStopGenerator.h"
#include "simpleGenerators/waitForButtonGenerator.h"
#include "parts/fSharpVariables.h"

using namespace qReal::robots::generators;
using namespace simple;
using namespace fSharp;

FSharpGeneratorFactory::FSharpGeneratorFactory(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: GeneratorFactoryBase(repo, errorReporter)
{
}

FSharpGeneratorFactory::~FSharpGeneratorFactory()
{
}

AbstractSimpleGenerator *FSharpGeneratorFactory::simpleGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();
	if (elementType == "EnginesForward" || elementType == "EnginesBackward") {
		return new FSharpEnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType == "EnginesStop") {
		return new FSharpEnginesStopGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PlayTone") {
		return new PlayToneGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Smile") {
		return new SmileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SadSmile") {
		return new SadSmileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Say") {
		return new SayGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SetBackground") {
		return new SetBackgroundGenerator(mRepo, customizer, id, this);
	} else if (elementType == "System") {
		return new SystemGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Led") {
		return new LedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "DetectLine") {
		return new DetectLineGenerator(mRepo, customizer, id, this);
	} else if (elementType == "InitCamera") {
		return new InitCameraGenerator(mRepo, customizer, id, this);
	} else if (elementType == "LineDetectorToVariable") {
		return new LineDetectorToVariableGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForEnter") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForEnter.t", this);
	} else if (elementType == "WaitForLeft") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForLeft.t", this);
	} else if (elementType == "WaitForRight") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForRight.t", this);
	} else if (elementType == "WaitForUp") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForUp.t", this);
	} else if (elementType == "WaitForDown") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForDown.t", this);
	} else if (elementType == "WaitForPower") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForPower.t", this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString FSharpGeneratorFactory::pathToTemplates() const
{
	return ":/fSharp/templates";
}


Binding::MultiConverterInterface *FSharpGeneratorFactory::enginesConverter(bool powerMotors) const
{
	if (powerMotors) {
		return new converters::RegexpMultiConverter(converters::PowerMotorPortConverter::splitRegexp()
				, new converters::PowerMotorPortConverter);
	}

	return new converters::RegexpMultiConverter(converters::ServoMotorPortConverter::splitRegexp()
			, new converters::ServoMotorPortConverter);
}

Binding::MultiConverterInterface *FSharpGeneratorFactory::enginesConverter() const
{
	return enginesConverter(true);
}

Binding::ConverterInterface *FSharpGeneratorFactory::inputPortConverter() const
{
	return new Binding::EmptyConverter;
}

Binding::ConverterInterface *FSharpGeneratorFactory::outputPortConverter() const
{
	return new converters::PowerMotorPortConverter;
}

Binding::ConverterInterface *FSharpGeneratorFactory::functionBlockConverter() const
{
	return new converters::FunctionFSharpBlockConverter(pathToTemplates()
							, inputPortConverter()
							, outputPortConverter()
							, functionInvocationConverter());
}

void FSharpGeneratorFactory::initVariables()
{
    mVariables = new parts::FSharpVariables(pathToTemplates());
}
