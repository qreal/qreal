#pragma once

#include <QtCore/QObject>

#include "robotCommunicationInterface.h"
#include "../../models/graphicalModelAssistApi.h"
#include "../../mainwindow/errorReporter.h"

namespace qReal {

namespace interpreters {

namespace robots {

class Interpreter : public QObject
{
	Q_OBJECT

public:
	Interpreter(models::GraphicalModelAssistApi const &modelApi);
	~Interpreter();

public slots:
	void interpret();
	void stop();
};

}

}

}
