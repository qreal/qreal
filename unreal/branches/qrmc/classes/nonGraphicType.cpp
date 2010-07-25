#include "nonGraphicType.h"
#include "../utils/outFile.h"

#include <QtCore/QDebug>

using namespace utils;

NonGraphicType::NonGraphicType()
	: Type(true, NULL, NULL, qReal::ROOT_ID)  // Неграфическим типам тоже неплохо бы знать, из какой они диаграммы
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
