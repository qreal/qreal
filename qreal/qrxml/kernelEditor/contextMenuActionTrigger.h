#pragma once

#include "../../qrgui/pluginInterface/listenerInterface.h"

namespace kernelEditor {

	class ContextMenuActionTrigger: public qReal::ListenerInterface {
		Q_OBJECT
	public slots:
		void contextMenuActionTriggered(QString const &nam);
	};

}
