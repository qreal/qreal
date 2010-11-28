#pragma once

#include <QtCore/QObject>

#include "../pluginInterface/listenerInterface.h"

namespace qReal {

	class Listener: public ListenerInterface {
		Q_OBJECT
	public:
		Listener()/*: mApi(NULL)*/ {}

		// TODO: Rework listeners to use new models.
		void init(/*model::ModelAssistApi *api*/) {
			// mApi = NULL;
		}
	protected:
		// model::ModelAssistApi *mApi;
	};

}
