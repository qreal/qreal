#pragma once

#include <QtXml/QDomDocument>

#include "classes/object.h"

namespace qrRepo {
namespace details {

/// Class that saves repository contents as one oncompressed XML file.
class SingleXmlSerializer
{
public:
	static void exportToXml(const QString &targetFile, QHash<qReal::Id, Object*> const &objects);
	static void exportDiagram(const qReal::Id &diagramId, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportElement(const qReal::Id &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportChildren(const qReal::Id &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);
	static void exportProperties(const qReal::Id &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects);

private:
	/// Creating is prohibited, utility class instances can not be created.
	SingleXmlSerializer();
};

}
}
