# pragma once

#include <QtCore/QObject>
#include "../editorManager/listener.h"
#include "../kernel/ids.h"

namespace qReal {

	// TODO: Reimplement ListenerManager to be able to work with new models.
	class ListenerManager: public QObject {
		Q_OBJECT
	public:
		ListenerManager(QList<Listener *> const &listeners/*, model::ModelAssistApi *api*/);
	signals:
		void objectCreated(qReal::Id const &id);
		void contextMenuActionTriggered(QString const &name);
	private:
		ListenerManager(ListenerManager const &); // Nachahmung ist verboten!
		ListenerManager &operator =(ListenerManager const &); // Assignment is also forbidden

		void initListeners(QList<Listener *> const &listeners /*, model::ModelAssistApi *api*/);
		void bindListenerSlotsByName(QList<Listener *> const &listeners);
	};

}
