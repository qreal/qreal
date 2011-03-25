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
	virtual bool generateObjectRequestString(utils::OutFile &out, bool isNotFirst);
	virtual bool generateProperties(utils::OutFile &out, bool isNotFirst);
	virtual bool generateContainedTypes(utils::OutFile &out, bool isNotFirst);
	virtual bool generateConnections(utils::OutFile &out, bool isNotFirst);
	virtual bool generateUsages(utils::OutFile &out, bool isNotFirst);
	virtual bool generatePossibleEdges(utils::OutFile &out, bool isNotFirst);
	virtual void generateMouseGesturesMap(utils::OutFile &out);
	virtual void generatePropertyDisplayedNamesMapping(utils::OutFile &out);
	virtual void generatePropertyDescriptionMapping(utils::OutFile &out);

protected:
	NonGraphicType();
};
