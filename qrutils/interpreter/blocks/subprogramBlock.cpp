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

#include "subprogramBlock.h"

#include <qrutils/nameNormalizer.h>

using namespace qReal::interpretation::blocks;

SubprogramBlock::SubprogramBlock()
{
}

void SubprogramBlock::run()
{
	const Id logicalId = mGraphicalModelApi->logicalId(id());

	const QString name = mLogicalModelApi->name(logicalId);

	const QString validName = utils::NameNormalizer::normalizeStrongly(name, false);
	if (validName.isEmpty()) {
		error(tr("Please enter valid c-style name for subprogram \"") + name + "\"");
		return;
	}

	const QString properties = mLogicalModelApi->logicalRepoApi().stringProperty(logicalId, "dynamicProperties");
	if (!properties.isEmpty()) {
		QDomDocument dynamicProperties;
		dynamicProperties.setContent(properties);
		for (QDomElement element = dynamicProperties.firstChildElement("properties").firstChildElement("property");
			!element.isNull();
			element = element.nextSiblingElement("property"))
		{
			const QString type = element.attribute("type");
			const QString value = element.attribute("value");
			const QString name = element.attribute("text");

			if (type == "bool") {
				evalCode<bool>(name + " = " + value);
			} else if (type == "int") {
				evalCode<int>(name + " = " + value);
			} else {
				evalCode<QString>(name + " = " + value);
			}
		}
	}

	const Id logicalDiagram = mLogicalModelApi->logicalRepoApi().outgoingExplosion(logicalId);
	const IdList diagrams = mGraphicalModelApi->graphicalIdsByLogicalId(logicalDiagram);
	if (!diagrams.isEmpty()) {
		emit stepInto(diagrams[0]);
	}
}

void SubprogramBlock::finishedSteppingInto()
{
	emit done(mNextBlockId);
}
