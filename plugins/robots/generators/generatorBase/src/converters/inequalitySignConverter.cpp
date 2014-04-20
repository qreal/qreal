#include "inequalitySignConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

InequalitySignConverter::InequalitySignConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping(QString::fromUtf8("меньше"), "signs/less.t");
	addMapping(QString::fromUtf8("больше"), "signs/greater.t");
	addMapping(QString::fromUtf8("не меньше"), "signs/notLess.t");
	addMapping(QString::fromUtf8("не больше"), "signs/notGreater.t");
	addMapping(QString::fromUtf8("равно"), "signs/equals.t");
}
