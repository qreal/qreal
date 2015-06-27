#pragma once

#include <QtCore/QObject>

namespace qReal {
namespace gui {

class ScriptAPI;

/// Implements keyboard typing emulation for GUI scripting.
class VirtualKeyboard : public QObject
{
	Q_OBJECT

public:
	explicit VirtualKeyboard(ScriptAPI &mScriptAPI);

	/// Emulates keyboard to type \a message for \a duration time overally.
	Q_INVOKABLE void type(const QString &message, int duration);

private:
	void printValue(const QString &value, int duration);

	ScriptAPI &mScriptAPI;
};

}
}
