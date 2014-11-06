#pragma once

#include <QtCore/QObject>

namespace qReal {

namespace gui {

class ScriptAPI;

/// This class provides tool for emulate keyboard typing.
class VirtualKeyboard : public QObject
{
	Q_OBJECT

public:
	VirtualKeyboard(ScriptAPI *mScriptAPI);

	/// Emulates keyboard to type message for over duration time.
	Q_INVOKABLE void type(QString const &message, int const duration);

private:
	void printValue(QString const &value, int const duration);

	ScriptAPI *mScriptAPI;
};

}
}
