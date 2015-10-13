/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include "private/listeners.h"
#include "qrkernel/kernelDeclSpec.h"

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
///     SettingsLister::listen("coolSetting", coolWidget, &Widget::setVisible);
/// or this:
///     SettingsLister::listen("coolSetting", []() { /* Do whatever you want */ });
/// @warning: You should be careful when using lambda-listeners. They cannot be unsubscribed from listening so if they
/// access objects that may be disposed next setting with registered key modification will cause segfaults. Slot-like
/// connection better suits for such situations because listener will be automaticly unsubscribed on disposal.
class QRKERNEL_EXPORT SettingsListener : public QObject
{
	Q_OBJECT

public:
	/// Starts listening of the settings manager`s updates by the given key. The usage syntax is similar to
	/// QObject::connect() function in member case. The slot must be parameterless.
	template <typename Func>
	static void listen(const QString &key
			, typename QtPrivate::QEnableIf<
					QtPrivate::FunctionPointer<Func>::ArgumentCount <= 0
					, typename QtPrivate::FunctionPointer<Func>::Object *>::Type sender, Func signal)
	{
		instance().mListeners.insertMulti(key, new SlotListener0<Func>(sender, signal));
		/// @todo: remove this restriction to QObject
		connect(sender, SIGNAL(destroyed(QObject*)), &instance(), SLOT(disconnect(QObject*)));
	}

	/// Starts listening of the settings manager`s updates by the given key. The usage syntax is similar to
	/// QObject::connect() function in member case. The slot must accept one parameter of the arbitary type
	/// to which modified settings value will be casted by qvariant_cast.
	template <typename Func>
	static void listen(const QString &key
			, typename QtPrivate::QEnableIf<
					QtPrivate::FunctionPointer<Func>::ArgumentCount == 1
					, typename QtPrivate::FunctionPointer<Func>::Object *>::Type sender, Func signal)
	{
		instance().mListeners.insertMulti(key, new SlotListener1<
				typename QtPrivate::FunctionPointer<Func>::Arguments::Car, Func>(sender, signal));
		/// @todo: remove this restriction to QObject
		connect(sender, SIGNAL(destroyed(QObject*)), &instance(), SLOT(disconnect(QObject*)));
	}

	/// Starts listening of the settings manager`s updates by the given key. The usage syntax is similar to
	/// QObject::connect() function in lambda case. The lambda must be parameterless.
	/// @param owner The QObject whoose disposal will disconnect this lambda too.
	static void listen(const QString &key, const std::function<void()> &lambda, QObject *owner = 0)
	{
		instance().mListeners.insertMulti(key, new LambdaListener0(lambda, owner));
		if (owner) {
			connect(owner, SIGNAL(destroyed(QObject*)), &instance(), SLOT(disconnect(QObject*)));
		}
	}

	/// Starts listening of the settings manager`s updates by the given key. The usage syntax is similar to
	/// QObject::connect() function in lambda case. The lambda must accept one parameter of the arbitary type
	/// to which modified settings value will be casted by qvariant_cast.
	/// @param owner The QObject whoose disposal will disconnect this lambda too.
	template <typename Func, typename Type
			= typename QtPrivate::FunctionPointer<decltype(&Func::operator())>::Arguments::Car>
	static void listen(const QString &key, Func lambda, QObject *owner = 0)
	{
		instance().mListeners.insertMulti(key, new LambdaListener1<Type>(lambda, owner));
		if (owner) {
			connect(owner, SIGNAL(destroyed(QObject*)), &instance(), SLOT(disconnect(QObject*)));
		}
	}

public slots:
	/// Stops listening all settings keys for the given listener.
	static void disconnect(QObject *listener);

	/// Stops listening the given key for the given listener.
	static void disconnect(const QString &key, QObject *listener);

private slots:
	void onSettingsChanged(const QString &name, const QVariant &oldValue, const QVariant &newValue);

private:
	SettingsListener();
	~SettingsListener();

	static SettingsListener &instance();

	QMultiMap<QString, AbstractListener *> mListeners;
};

}
