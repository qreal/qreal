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

QString NonGraphicType::generateReferenceProperties(const QString &lineTemplate) const
{
	Q_UNUSED(lineTemplate);
	return "";
}

QString NonGraphicType::generatePropertyName(const QString &lineTemplate) const//debugfix
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
