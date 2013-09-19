#include "nameNormalizerConverter.h"

#include <qrutils/nameNormalizer.h>

using namespace qReal::robots::generators::converters;

QString NameNormalizerConverter::convert(QString const &data) const
{
	return utils::NameNormalizer::normalizeStrongly(data, false);
}
