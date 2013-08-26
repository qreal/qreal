#pragma once

#include <QtCore/QList>

#include "smartLine.h"

namespace qReal {
namespace robots {
namespace generators {

class ControlFlow : public QObject
{
public:
	explicit ControlFlow(QObject *parent = 0);

private:
	QList<SmartLine> mCode;
};

}
}
}
