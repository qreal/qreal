#pragma once

#include "../../qrgui/editorManager/listener.h"

namespace kernelEditor {

	class ContextMenuActionTrigger: public qReal::Listener {
		Q_OBJECT
	public slots:
		void contextMenuActionTriggered(QString const &nam);
	private:
		void writeMethodPropertiesToModel(qReal::Id methodElemId, QString name, QString returnType, 
				QString parameters, QString displayName);
		void writeFieldPropertiesToModel(qReal::Id fieldElemId, QString name, QString type,
				QString displayName);
	};

}
