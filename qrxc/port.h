#pragma once

#include <QDomElement>
#include "../qrutils/scalableItem.h"

namespace utils {
	class OutFile;
}

class Port : public utils::ScalableItem
{
public:
	virtual bool init(QDomElement const &element, int width, int height) = 0;
	virtual void generateCode(utils::OutFile &out) = 0;
	virtual Port *clone() const = 0;
};
