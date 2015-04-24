/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

	static void exportDiagram(const qReal::Id &diagramId, QDomDocument &doc, QDomElement &root
			, QHash<qReal::Id, Object*> const &objects);

	static void exportElement(const qReal::Id &id, QDomDocument &doc, QDomElement &root
			, QHash<qReal::Id, Object*> const &objects);

	static void exportChildren(const qReal::Id &id, QDomDocument &doc, QDomElement &root
			, QHash<qReal::Id, Object*> const &objects);

	static void exportProperties(const qReal::Id &id, QDomDocument &doc, QDomElement &root
			, QHash<qReal::Id, Object*> const &objects);

private:
	/// Creating is prohibited, utility class instances can not be created.
	SingleXmlSerializer();
};

}
}
