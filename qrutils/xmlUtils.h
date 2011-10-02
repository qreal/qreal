#pragma once

#include <QtXml/QDomDocument>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT xmlUtils {
public:
	static QDomDocument loadDocument(QString const& fileName);
};

}
