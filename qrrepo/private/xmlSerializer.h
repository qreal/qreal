#pragma once

#include <QtXml/QDomDocument>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include "classes/object.h"

namespace qrRepo {
namespace details {

/// exports all repo contents to a single XML file
class XmlSerializer
{
public:
	XmlSerializer();

	void exportTo(QString const &targetFile);
};

}
}
