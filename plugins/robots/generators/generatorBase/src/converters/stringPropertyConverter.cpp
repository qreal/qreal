#include "generatorBase/converters/stringPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

QString StringPropertyConverter::convert(QString const &data) const
{
	QString result = data;
	return "\"" + result.replace("\"", "\\\"") + "\"";
}
