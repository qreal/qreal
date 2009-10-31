#include "nonGraphicType.h"
#include "outFile.h"

NonGraphicType::NonGraphicType()
	: Type(true)
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

bool NonGraphicType::generateObjectRequestString(OutFile &out, bool notIsFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(notIsFirst)
	return false;
}

bool NonGraphicType::generateProperties(OutFile &out, bool notIsFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(notIsFirst)
	return false;
}