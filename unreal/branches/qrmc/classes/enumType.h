#pragma once

#include "nonGraphicType.h"

#include <QStringList>
#include <QDebug>

namespace utils {
	class OutFile;
}

class EnumType : public NonGraphicType
{
public:
	virtual bool init();
	virtual Type* clone() const;

	void print();

private:

	QStringList mValues;
};
