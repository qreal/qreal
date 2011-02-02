#pragma once

#include "../../qrgui/editorManager/listener.h"

namespace kernelEditor {

	class ContextMenuActionTrigger: public qReal::Listener {
		Q_OBJECT
	public slots:
		void contextMenuActionTriggered(QString const &nam);
	};

}
