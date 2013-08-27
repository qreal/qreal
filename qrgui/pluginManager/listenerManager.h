#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>

#include "editorPluginInterface/listenerInterface.h"

namespace qReal {

namespace models {
	class LogicalModelAssistApi;
	class GraphicalModelAssistApi;
}

class ListenerManager: public QObject {
	Q_OBJECT

public:
	ListenerManager(QList<ListenerInterface *> const &listeners
			, models::LogicalModelAssistApi &logicalApi, models::GraphicalModelAssistApi &graphicalApi);
	~ListenerManager();
signals:
	void objectCreated(qReal::Id const &id);
	void contextMenuActionTriggered(QString const &name);

private:
	ListenerApiInterface *mListenerApiInterface;

	ListenerManager(ListenerManager const &); // Nachahmung ist verboten!
	ListenerManager &operator =(ListenerManager const &); // Assignment is also forbidden

	void initListeners(QList<ListenerInterface *> const &listeners);

	void bindListenerSlotsByName(QList<ListenerInterface *> const &listeners);
};

}
