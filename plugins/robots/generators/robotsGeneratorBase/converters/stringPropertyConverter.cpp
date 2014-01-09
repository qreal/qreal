#include "stringPropertyConverter.h"

using namespace qReal::robots::generators::converters;

QString StringPropertyConverter::convert(QString const &data) const
{
	QString result = data;
	return "\"" + result.replace("\"", "\\\"") + "\"";
}
