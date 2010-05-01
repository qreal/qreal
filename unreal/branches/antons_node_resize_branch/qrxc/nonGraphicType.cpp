#include "nonGraphicType.h"
#include "../utils/outFile.h"

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

bool NonGraphicType::generateObjectRequestString(OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return false;
}

bool NonGraphicType::generateProperties(OutFile &out, bool isNotFirst)
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

bool NonGraphicType::generateConnections(utils::OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return false;
}
