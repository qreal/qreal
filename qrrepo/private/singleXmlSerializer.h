#pragma once

#include <QtXml/QDomDocument>

#include "classes/object.h"

namespace qrRepo {
namespace details {

class SingleXmlSerializer
{
public:
	static void exportToXml(QString const &targetFile, QHash<qReal::Id, Object*> const &objects);
	static void exportDiagram(qReal::Id const &diagramId, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportElement(qReal::Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportChildren(qReal::Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportProperties(qReal::Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
};

}
}
