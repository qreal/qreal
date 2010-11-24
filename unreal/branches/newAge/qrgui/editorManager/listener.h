#pragma once

#include <QtCore/QObject>

#include "../model/modelAssistApi.h"
#include "../pluginInterface/listenerInterface.h"

namespace qReal {

	class Listener: public ListenerInterface {
		Q_OBJECT
	public:
		Listener(): mApi(NULL) {}
		void init(model::ModelAssistApi *api) {
			mApi = api;
		}
	protected:
		model::ModelAssistApi *mApi;
	};

}
