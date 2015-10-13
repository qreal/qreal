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

#include "nonGraphicType.h"

#include <QtCore/QDebug>

using namespace qrmc;

NonGraphicType::NonGraphicType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id)
	: Type(true, diagram, api, id)
{}

bool NonGraphicType::resolve()
{
	return true;
}

bool NonGraphicType::isGraphicalType() const
{
	return false;
}

void NonGraphicType::print()
{
	qDebug() << "non graphic type";
}

QString NonGraphicType::generateProperties(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generatePropertyDefaults(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generatePropertyDisplayedNames(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateElementDescription(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateReferenceProperties(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate);
	return "";
}

QString NonGraphicType::generatePortTypes(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate);
	return "";
}

QString NonGraphicType::generatePropertyName(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate);
	return "";
}

QString NonGraphicType::generateParents(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate);
	return "";
}

QString NonGraphicType::generateContainers(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateConnections(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateUsages(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateIsNodeOrEdge(const QString &lineTemplate) const
{
	QString line = lineTemplate;
	line.replace(elementNameTag, name()).replace(isNodeOrEdgeTag, "0");
	return line;
}

QString NonGraphicType::generatePossibleEdges(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateNodeClass(const QString &classTemplate)
{
	Q_UNUSED(classTemplate)
	return "";
}

QString NonGraphicType::generateEdgeClass(const QString &classTemplate) const
{
	Q_UNUSED(classTemplate)
	return "";
}

QString NonGraphicType::generateResourceLine(const QString &resourceTemplate) const
{
	Q_UNUSED(resourceTemplate)
	return "";
}
