#include "nonGraphicType.h"
#include "utils/outFile.h"

#include <QtCore/QDebug>

using namespace qrmc;

NonGraphicType::NonGraphicType(Diagram *diagram, qrRepo::RepoApi *api, qReal::Id const &id)
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
