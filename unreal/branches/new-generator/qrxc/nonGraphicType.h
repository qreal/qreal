#pragma once

#include "type.h"

class OutFile;

class NonGraphicType : public Type
{
public:
	NonGraphicType();
	virtual bool resolve(); 
	virtual void generateCode(OutFile &out);
	virtual void generateNameMapping(OutFile &out);
	virtual bool generateObjectRequestString(OutFile &out, bool notIsFirst);
	virtual bool generateProperties(OutFile &out, bool notIsFirst);
};