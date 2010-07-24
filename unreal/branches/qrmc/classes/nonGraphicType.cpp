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


