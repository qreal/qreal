#pragma once

#include "type.h"

namespace utils {
	class OutFile;
}

class NonGraphicType : public Type
{
public:
	virtual bool resolve();
	virtual void generateCode(utils::OutFile &out);
	virtual void generateNameMapping(utils::OutFile &out);
	virtual bool generateObjectRequestString(utils::OutFile &out, bool notIsFirst);
	virtual bool generateProperties(utils::OutFile &out, bool notIsFirst);
	virtual bool generateContainedTypes(utils::OutFile &out, bool notIsFirst);
protected:
	NonGraphicType();
};
