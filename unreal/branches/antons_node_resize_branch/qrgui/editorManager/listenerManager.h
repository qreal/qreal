# pragma once

#include <QtCore/QObject>
#include "../editorManager/listener.h"
#include "../model/modelAssistApi.h"
#include "../kernel/ids.h"

namespace qReal {

	class ListenerManager: public QObject {
		Q_OBJECT
	public:
		ListenerManager(QList<Listener *> const &listeners, model::ModelAssistApi *api);
	signals:
		void objectCreated(qReal::Id const &id);
	private:
		ListenerManager(ListenerManager const &);  // Копирование запрещено
		ListenerManager &operator =(ListenerManager const &);  // Присваивание тоже

		void initListeners(QList<Listener *> const &listeners, model::ModelAssistApi *api);
		void bindListenerSlotsByName(QList<Listener *> const &listeners);
	};

}
