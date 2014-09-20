#include "listenerManager.h"

#include <QtCore/QMetaMethod>
#include <QtCore/QDebug>

#include <qrkernel/exception/exception.h>

#include "plugins/pluginManager/listenerApi.h"
#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

using namespace qReal;

ListenerManager::ListenerManager(QList<ListenerInterface *> const &listeners
		, models::LogicalModelAssistApi &logicalApi, models::GraphicalModelAssistApi &graphicalApi)
	: mListenerApiInterface(new ListenerApi(logicalApi, graphicalApi))
{
	initListeners(listeners);
	bindListenerSlotsByName(listeners);
}

ListenerManager::~ListenerManager()
{
	delete mListenerApiInterface;
}

void ListenerManager::initListeners(QList<ListenerInterface *> const &listeners)
{
	foreach (ListenerInterface *listener, listeners)
		listener->init(mListenerApiInterface);
}

void ListenerManager::bindListenerSlotsByName(QList<ListenerInterface *> const &listeners)
{
	foreach (ListenerInterface *listener, listeners) {
		int methodCount = listener->metaObject()->methodCount();
		for (int i = 0; i < methodCount; ++i) {

			QMetaMethod slotMethod = listener->metaObject()->method(i);
			QByteArray const slotName = slotMethod.methodSignature();
			int slotId = listener->metaObject()->indexOfSlot(slotName.data());

			if (slotMethod.access() != QMetaMethod::Public
				|| slotId == -1
				|| QString(slotName).startsWith("deleteLater"))
			{
				continue;
			}

			int thisMethodCount = this->metaObject()->methodCount();
			bool connected = false;
			for (int j = 0; j < thisMethodCount; ++j) {
				QByteArray const signalName = this->metaObject()->method(j).methodSignature();

				if (signalName != QString(slotName))
					continue;

				int signalId = this->metaObject()->indexOfSignal(signalName.data());

				connected |= (bool) QMetaObject::connect(this, signalId, listener, slotId);
			}

			if (!connected)
				throw Exception("Unknown plugin event listener public slot " + QString(slotName));
		}
	}
}
