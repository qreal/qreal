#pragma once

#include <QtCore/QObject>

#include "../../../qrkernel/ids.h"
#include "../../../qrgui/mainwindow/projectManager/projectManagementInterface.h"
#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"


namespace qReal{
namespace rulesChecker{

class RulesChecker : public QObject
{
	Q_OBJECT

public slots:
	 void check();

public:
	 RulesChecker(GraphicalModelAssistInterface &graphicalModelApi
			 , qReal::gui::MainWindowInterpretersInterface &interpretersInterface);

private:
	 void makeDetour();
	 void getElementsList();



	 GraphicalModelAssistInterface *mGraphicalModelApi;
	 qReal::gui::MainWindowInterpretersInterface *mWindowInterface;


};

}
}

