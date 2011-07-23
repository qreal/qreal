#pragma once

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtGui/QAction>

#include "../../qrkernel/ids.h"

namespace qReal {

class InterpreterInterface
{
public:
	virtual void init() = 0;
	virtual void interpret(Id const &diagram) = 0;
	virtual void stop() = 0;
	virtual QList<QAction *> customActions() = 0;
};

}
