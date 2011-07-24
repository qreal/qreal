#pragma once

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtGui/QAction>

#include "../../qrkernel/ids.h"

#include "../../qrrepo/repoControlInterface.h"
#include "usedInterfaces/graphicalModelAssistInterface.h"
#include "usedInterfaces/logicalModelAssistInterface.h"
#include "../mainwindow/mainWindowInterpretersInterface.h"

namespace qReal {

class InterpreterInterface
{
public:
	virtual void init(GraphicalModelAssistInterface const &graphicalModelApi
			, LogicalModelAssistInterface const &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface) = 0;
	virtual void interpret(Id const &diagram) = 0;
	virtual void stop() = 0;
	virtual QList<QAction *> customActions() = 0;
};

}
