#include "generatorBase/converters/regexpMultiConverter.h"

using namespace generatorBase::converters;

RegexpMultiConverter::RegexpMultiConverter(QString const &splitRegexp
		, simple::Binding::ConverterInterface const * const converter)
	: mSplitRegexp(splitRegexp)
	, mSimpleConverter(converter)
{
}

RegexpMultiConverter::~RegexpMultiConverter()
{
	delete mSimpleConverter;
}

QStringList RegexpMultiConverter::convert(QString const &data) const
{
	QStringList const parts = data.toUpper().split(QRegExp(mSplitRegexp), QString::SkipEmptyParts);
	QStringList result;
	for (QString const &part : parts) {
		result << mSimpleConverter->convert(part);
	}

	return result;
}
