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
	static QVariant boolXmlToVariant(const QDomElement &element);
	static QVariant charXmlToVariant(const QDomElement &element);
	static QVariant dateXmlToVariant(const QDomElement &element);
	static QVariant dateTimeXmlToVariant(const QDomElement &element);
	static QVariant doubleXmlToVariant(const QDomElement &element);
	static QVariant intXmlToVariant(const QDomElement &element);
	static QVariant lineXmlToVariant(const QDomElement &element);
	static QVariant lineFXmlToVariant(const QDomElement &element);
	static QVariant longLongXmlToVariant(const QDomElement &element);
	static QVariant pointXmlToVariant(const QDomElement &element);
	static QVariant pointFXmlToVariant(const QDomElement &element);
	static QVariant rectXmlToVariant(const QDomElement &element);
	static QVariant rectFXmlToVariant(const QDomElement &element);
	static QVariant sizeXmlToVariant(const QDomElement &element);
	static QVariant sizeFXmlToVariant(const QDomElement &element);
	static QVariant stringXmlToVariant(const QDomElement &element);
	static QVariant timeXmlToVariant(const QDomElement &element);
	static QVariant uIntXmlToVariant(const QDomElement &element);
	static QVariant uLongLongXmlToVariant(const QDomElement &element);
};

}
