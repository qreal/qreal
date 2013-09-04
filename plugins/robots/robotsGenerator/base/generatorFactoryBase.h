#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>

#include "simpleGenerators/abstractSimpleGenerator.h"
#include "simpleGenerators/binding.h"

namespace qReal {
namespace robots {
namespace generators {

class GeneratorCustomizer;

class GeneratorFactoryBase : public QObject
{
public:
	explicit GeneratorFactoryBase(qrRepo::RepoApi const &repo);

	virtual ~GeneratorFactoryBase();

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

//	virtual simple::AbstractSimpleGenerator *templateGenratorFor(Id const &id
//			, QString const &templateFile, QList<simple::Binding *> const &bindings);

	// This group of methods can be overridden to customize concrete generator
//	virtual simple::AbstractSimpleGenerator *ifBlock(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *loop(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *function(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *comment(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *enginesForward(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *enginesBackward(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *enginesStop(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *timer(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *beep(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *playTone(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *finalNode(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *nullificationEncoder(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForColor(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForColorIntensity(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForLight(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForTouchSensor(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForSonarDistance(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForEncoder(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForSound(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForGyroscope(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForAccelerometer(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *balance(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *balanceInit(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *waitForButtons(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *drawPixel(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *drawLine(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *drawCircle(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *printText(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *drawRect(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *clearScreen(Id const &id) const;
//	virtual simple::AbstractSimpleGenerator *subprogram(Id const &id) const;

private:
	qrRepo::RepoApi const &mRepo;
};

}
}
}
