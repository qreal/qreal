#pragma once

#include <QtCore/QObject>

#include "editorPluginInterface/listenerApiInterface.h"

namespace qReal {

	class ListenerInterface : public QObject
	{
	public:
		virtual ~ListenerInterface() {}
		void init(ListenerApiInterface *listenerApiInterface)
		{
			mListenerApiInterface = listenerApiInterface;
		}
	private:
		ListenerApiInterface *mListenerApiInterface;
	};

}
