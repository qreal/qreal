#include "listenerManager.h"

#include <QtCore/QMetaMethod>
#include <QtCore/QDebug>

#include "../kernel/exception/exception.h"

using namespace qReal;

ListenerManager::ListenerManager(QList<Listener *> const &listeners, model::ModelAssistApi *)
{
	bindListenerSlotsByName(listeners);
}

void ListenerManager::initListeners(QList<Listener *> const &listeners, model::ModelAssistApi *api)
{
	foreach (Listener *listener, listeners)
		listener->init(api);
}

void ListenerManager::bindListenerSlotsByName(QList<Listener *> const &listeners)
{
	foreach (Listener *listener, listeners) {
		int methodCount = listener->metaObject()->methodCount();
		for (int i = 0; i < methodCount; ++i) {

			QMetaMethod slotMethod = listener->metaObject()->method(i);
			char const * const slotName = slotMethod.signature();
			int slotId = listener->metaObject()->indexOfSlot(slotName);

			if (slotMethod.access() != QMetaMethod::Public
				|| slotId == -1
				|| QString(slotName).startsWith("deleteLater"))
			{
				continue;
			}

			int thisMethodCount = this->metaObject()->methodCount();
			bool connected = false;
			for (int j = 0; j < thisMethodCount; ++j) {
				char const * const signalName = this->metaObject()->method(j).signature();

				if (signalName != QString(slotName))
					continue;

				int signalId = this->metaObject()->indexOfSignal(signalName);

				connected |= QMetaObject::connect(this, signalId, listener, slotId);
			}

			if (!connected)
				throw Exception("Unknown plugin event listener public slot " + QString(slotName));
		}
	}
}
