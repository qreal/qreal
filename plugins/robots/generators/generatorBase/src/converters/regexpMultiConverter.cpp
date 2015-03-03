#include "generatorBase/converters/regexpMultiConverter.h"

using namespace generatorBase::converters;

RegexpMultiConverter::RegexpMultiConverter(const QString &splitRegexp
		, const simple::Binding::ConverterInterface * const converter)
	: mSplitRegexp(splitRegexp)
	, mSimpleConverter(converter)
{
}

RegexpMultiConverter::~RegexpMultiConverter()
{
	delete mSimpleConverter;
}

QStringList RegexpMultiConverter::convert(const QString &data) const
{
	const QStringList parts = data.toUpper().split(QRegExp(mSplitRegexp), QString::SkipEmptyParts);
	QStringList result;
	for (const QString &part : parts) {
		result << mSimpleConverter->convert(part);
	}

	return result;
}
