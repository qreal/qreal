#pragma once

#include "qrkernel/settingsManager.h"

namespace qReal {

/// Provides fast and convenient way to track settings changes.
/// Suppose that somebody wants to react immediately on modifications of the bool setting with the name 'coolSetting'.
/// The most straightforward way to do it is to subscribe to SettingsManager::settingsChanged signal and to filter out
/// other settings modifications. But this aproach has two minuses. The first one is a very bad influence on the system
/// performance. If we have a large amount of listeners and a great number of settings modifications (and this is
/// something we are going to deal with) then each modification will be broadcasted to each component without real
/// need to it. The second minus is that such way is inconvenient indeed for usage in client's code (we need to create
/// separate slot or lambda specially for casting new value from QVariant and filtering out other settings).
/// This class solves both problems. Listeners are stored into map so each modification will be fastly propagated just
/// to listeners that need it. Also this class contains lots of overrides specially for compactness of the client code.
/// For example if we need to show or hide some widget on our 'coolSetting' modification we can write something like
/// this:
///     SettingsLister::listenBool("coolSetting", coolWidget, &Widget::setVisible);
/// or this:
///     SettingsLister::listenBool("coolSetting", [](bool value) { /* Do whatever you want */ });
class SettingsListener : public QObject
{
public:
};

}
