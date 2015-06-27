#pragma once

#include <toolPluginInterface/usedInterfaces/hintInterface.h>

namespace qReal {
namespace gui {

class HintReporter;
class Arrow;
class ScriptAPI;

/// This class provides scripting access to hinting features in system API. Allows to show beautiful bubbling messages
/// and arrows pointing given widgets.
class HintAPI : public QObject, public HintInterface
{
	Q_OBJECT

public:
	HintAPI();

	/// Sends beautifully bubble-like message appearing in parent widget existing for \a lifeTime
	/// (conidering fade-away effect).
	Q_INVOKABLE void addHint(const QString &message, int lifeTime, QWidget *parent);

	/// Draws arrow pointing to \a target widget with the given tilt \a angle. Exists for lifeTime
	/// (considering fade-away effect).
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle, int lifeTime, QWidget *parent);
};

}
}
