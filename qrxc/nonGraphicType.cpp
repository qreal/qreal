#include "nonGraphicType.h"

#include <qrutils/outFile.h>

using namespace utils;

NonGraphicType::NonGraphicType()
	: Type(true, NULL)  // Неграфическим типам тоже неплохо бы знать, из какой они диаграммы
{}

bool NonGraphicType::resolve()
{
	return true;
}

void NonGraphicType::generateCode(OutFile &out)
{
	Q_UNUSED(out)
}

void NonGraphicType::generateNameMapping(OutFile &out)
{
	Q_UNUSED(out)
}

void NonGraphicType::generatePropertyDisplayedNamesMapping(utils::OutFile &out)
{
	Q_UNUSED(out)
}

void NonGraphicType::generatePropertyDescriptionMapping(utils::OutFile &out)
{
	Q_UNUSED(out)
}

void NonGraphicType::generateExplosionsMap(OutFile &out)
{
	Q_UNUSED(out)
}

bool NonGraphicType::generateObjectRequestString(OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return false;
}

bool NonGraphicType::generateProperties(OutFile &out, bool isNotFirst, bool isReference)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	Q_UNUSED(isReference)
	return false;
}

bool NonGraphicType::generatePorts(OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return false;
}

bool NonGraphicType::generateContainedTypes(OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return false;
}

bool NonGraphicType::generatePossibleEdges(OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return false;
}

void NonGraphicType::generateMouseGesturesMap(utils::OutFile &out)
{
	Q_UNUSED(out);
}
