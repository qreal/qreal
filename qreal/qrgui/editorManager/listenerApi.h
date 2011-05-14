#pragma once

#include <QtCore/QObject>

#include "../pluginInterface/listenerApiInterface.h"
#include "../models/graphicalModelAssistApi.h"
#include "../models/logicalModelAssistApi.h"

namespace qReal {

	class ListenerApi: public ListenerApiInterface {
	public:
		ListenerApi(models::LogicalModelAssistApi &logicalApi, models::GraphicalModelAssistApi &graphicalApi)
			: mLogicalApi(logicalApi), mGraphicalApi(graphicalApi) {}

	private:
		models::LogicalModelAssistApi &mLogicalApi;
		models::GraphicalModelAssistApi &mGraphicalApi;
	};

}
