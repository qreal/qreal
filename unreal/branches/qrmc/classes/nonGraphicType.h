#pragma once

#include "type.h"

namespace utils {
	class OutFile;
}

class NonGraphicType : public Type
{
public:
	virtual bool resolve();

	virtual void print();

protected:
	NonGraphicType();
};
