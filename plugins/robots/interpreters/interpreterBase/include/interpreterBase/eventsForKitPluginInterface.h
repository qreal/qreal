#pragma once

#include <QtCore/QObject>

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {

/// Events provided to kit plugins by interpreter.
class ROBOTS_INTERPRETER_BASE_EXPORT EventsForKitPluginInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~EventsForKitPluginInterface() {}

signals:
	void interpretationStarted();
	void interpretationStopped();
	void robotModelChanged(QString const &newModelName);
};

}
