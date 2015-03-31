#pragma once

#include <QtXml/QDomDocument>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT xmlUtils
{
public:
	static QDomDocument loadDocument(const QString &fileName
		, QString *errorMessage = 0, int *errorLine = 0, int *errorColumn = 0);
};

}
