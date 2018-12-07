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

#include <QtXml/QDomDocument>

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

	const QList<DynamicParameter> parameters = dynamicParameters();
	for (const DynamicParameter &param : parameters) {
		if (param.type == "bool") {
			setVariableValue<bool>(param.name, evalCode<bool>(param.code));
		} else if (param.type == "int") {
			setVariableValue<int>(param.name, evalCode<int>(param.code));
		} else if (param.type == "float") {
			setVariableValue<qreal>(param.name, evalCode<qreal>(param.code));
		} else {
			setVariableValue<QString>(param.name, evalCode<QString>(param.code));
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

QList<SubprogramBlock::DynamicParameter> SubprogramBlock::dynamicParameters() const
{
	const Id logicalId = mGraphicalModelApi->logicalId(id());
	const QString properties = mLogicalModelApi->logicalRepoApi().stringProperty(logicalId, "dynamicProperties");
	if (properties.isEmpty()) {
		return {};
	}

	QDomDocument dynamicProperties;
	dynamicProperties.setContent(properties);
	QList<DynamicParameter> result;
	for (QDomElement element = dynamicProperties.firstChildElement("properties").firstChildElement("property")
		; !element.isNull()
		; element = element.nextSiblingElement("property"))
	{
		result << DynamicParameter {element.attribute("displayedName")
				, element.attribute("type")
				, element.attribute("dynamicPropertyValue")
		};
	}

	return result;
}
