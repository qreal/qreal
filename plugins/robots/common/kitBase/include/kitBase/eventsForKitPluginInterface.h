#pragma once

#include <QtCore/QObject>

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {

/// Events provided to kit plugins by interpreter.
class ROBOTS_KIT_BASE_EXPORT EventsForKitPluginInterface : public QObject
{
	Q_OBJECT

signals:
	/// Program interpretation started.
	void interpretationStarted();

	/// Program interpretation finished or stopped by user.
	void interpretationStopped();

	/// Robot model is changed in settings (or selected in a toolbar).
	void robotModelChanged(const QString &newModelName);
};

}
