#include "nonGraphicType.h"

#include <QtCore/QDebug>

using namespace qrmc;

NonGraphicType::NonGraphicType(Diagram *diagram, qrRepo::LogicalRepoApi *api, qReal::Id const &id)
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

QString NonGraphicType::generateProperties(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generatePropertyDefaults(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generatePropertyDisplayedNames(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateReferenceProperties(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate);
	return "";
}

QString NonGraphicType::generateParents(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate);
	return "";
}

QString NonGraphicType::generateContainers(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateConnections(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateUsages(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateIsNodeOrEdge(QString const &lineTemplate) const
{
	QString line = lineTemplate;
	line.replace(elementNameTag, name()).replace(isNodeOrEdgeTag, "0");
	return line;
}

QString NonGraphicType::generatePossibleEdges(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate)
	return "";
}

QString NonGraphicType::generateNodeClass(QString const &classTemplate)
{
	Q_UNUSED(classTemplate)
	return "";
}

QString NonGraphicType::generateEdgeClass(QString const &classTemplate) const
{
	Q_UNUSED(classTemplate)
	return "";
}

QString NonGraphicType::generateResourceLine(QString const &resourceTemplate) const
{
	Q_UNUSED(resourceTemplate)
	return "";
}
