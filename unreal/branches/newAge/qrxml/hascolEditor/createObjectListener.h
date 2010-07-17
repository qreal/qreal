#pragma once

#include "../../qrgui/editorManager/listener.h"

namespace hascolEditor {

	class CreateObjectListener: public qReal::Listener {
		Q_OBJECT
	public slots:
		void objectCreated(qReal::Id const &id);
	};

}
