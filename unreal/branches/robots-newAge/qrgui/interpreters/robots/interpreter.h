#pragma once

#include <QtCore/QObject>

#include "../../models/graphicalModelAssistApi.h"
#include "../../models/logicalModelAssistApi.h"
#include "../../mainwindow/errorReporter.h"

#include "robotCommunicationInterface.h"
#include "details/thread.h"

namespace qReal {

namespace interpreters {

namespace robots {

class Interpreter : public QObject
{
	Q_OBJECT

public:
	Interpreter(models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi);
	~Interpreter();

public slots:
	void interpret();
	void stop();
};

}

}

}
