#pragma once

#include "../../qrgui/pluginInterface/listenerInterface.h"

namespace kernelEditor {

	class ContextMenuActionTrigger: public qReal::ListenerInterface {
		Q_OBJECT
	public slots:
		void contextMenuActionTriggered(QString const &nam);
	private:
		void writeMethodPropertiesToModel(qReal::Id methodElemId, QString name, QString returnType, 
				QString parameters, QString visibility, QString displayName);
		void writeFieldPropertiesToModel(qReal::Id fieldElemId, QString name, QString type,
				QString visibility, QString displayName);
	};

}
