#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>

#include <qrgui/mainwindow/projectManager/projectManagementInterface.h>
#include <qrgui/mainwindow/mainWindowInterpretersInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>

namespace robotsInterpreterCore {
namespace interpreter {

class InterpreterInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~InterpreterInterface() {}

	virtual void init(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, qReal::ProjectManagementInterface const &projectManager
	) = 0;
};

}
}
