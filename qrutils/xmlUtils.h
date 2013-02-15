#pragma once

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>
#include <QtCore/QFlags>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT xmlUtils
{
public:
	static QDomDocument loadDocument(QString const& fileName);
	static void qVariantToXml(QDomElement &element, QVariant const &variant);
	static QVariant xmlToVariant(QDomElement const &element);
	static void enumToXml(QDomElement &element, QString const &value);
	static void enumToXml(QDomElement &element, int value);
	static int xmlToEnumInt(QDomElement const &element);
	static void flagsToXml(QDomElement &element, QString const &values);

private:
	static QVariant boolXmlToVariant(QDomElement const &element);
	static QVariant charXmlToVariant(QDomElement const &element);
	static QVariant dateXmlToVariant(QDomElement const &element);
	static QVariant dateTimeXmlToVariant(QDomElement const &element);
	static QVariant doubleXmlToVariant(QDomElement const &element);
	static QVariant intXmlToVariant(QDomElement const &element);
	static QVariant lineXmlToVariant(QDomElement const &element);
	static QVariant lineFXmlToVariant(QDomElement const &element);
	static QVariant longLongXmlToVariant(QDomElement const &element);
	static QVariant pointXmlToVariant(QDomElement const &element);
	static QVariant pointFXmlToVariant(QDomElement const &element);
	static QVariant rectXmlToVariant(QDomElement const &element);
	static QVariant rectFXmlToVariant(QDomElement const &element);
	static QVariant sizeXmlToVariant(QDomElement const &element);
	static QVariant sizeFXmlToVariant(QDomElement const &element);
	static QVariant sizePolicyToVariant(QDomElement const &element);
	static QVariant stringXmlToVariant(QDomElement const &element);
	static QVariant timeXmlToVariant(QDomElement const &element);
	static QVariant uIntXmlToVariant(QDomElement const &element);
	static QVariant uLongLongXmlToVariant(QDomElement const &element);
};

}
