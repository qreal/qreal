#pragma once

#include <QtXml/QDomDocument>

namespace utils {

class xmlUtils {
public:
	static QDomDocument loadDocument(QString const& fileName);
};

}
