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
class QRKERNEL_EXPORT SettingsListener : public QObject
{
public:
	/// Starts listening of the settings manager`s updates by the given key. The usage syntax is similar to
	/// QObject::connect() function in member case. The slot must be parameterless.
	template <typename Func>
	static void listen(QString const &key
			, typename QtPrivate::QEnableIf<
					QtPrivate::FunctionPointer<Func>::ArgumentCount <= 0
					, typename QtPrivate::FunctionPointer<Func>::Object *>::Type sender, Func signal)
	{
		instance().mListeners.insertMulti(key, new SlotListener0<Func>(sender, signal));
	}

	/// Starts listening of the settings manager`s updates by the given key. The usage syntax is similar to
	/// QObject::connect() function in member case. The slot must accept one parameter of the arbitary type
	/// to which modified settings value will be casted by qvariant_cast.
	template <typename Func>
	static void listen(QString const &key
			, typename QtPrivate::QEnableIf<
					QtPrivate::FunctionPointer<Func>::ArgumentCount == 1
					, typename QtPrivate::FunctionPointer<Func>::Object *>::Type sender, Func signal)
	{
		instance().mListeners.insertMulti(key, new SlotListener1<
				typename QtPrivate::FunctionPointer<Func>::Arguments::Car, Func>(sender, signal));
	}

	/// Starts listening of the settings manager`s updates by the given key. The usage syntax is similar to
	/// QObject::connect() function in lambda case. The lambda must be parameterless.
	static void listen(QString const &key, std::function<void()> const &lambda)
	{
		instance().mListeners.insertMulti(key, new LambdaListener0(lambda));
	}

	/// Starts listening of the settings manager`s updates by the given key. The usage syntax is similar to
	/// QObject::connect() function in lambda case. The lambda must accept one parameter of the arbitary type
	/// to which modified settings value will be casted by qvariant_cast.
	template <typename Func, typename Type
			= typename QtPrivate::FunctionPointer<decltype(&Func::operator())>::Arguments::Car>
	static void listen(QString const &key, Func lambda)
	{
		instance().mListeners.insertMulti(key, new LambdaListener1<Type>(lambda));
	}

private slots:
	void onSettingsChanged(QString const &name, QVariant const &oldValue, QVariant const &newValue);

private:
	SettingsListener();
	~SettingsListener();

	static SettingsListener &instance();

	QMultiMap<QString, AbstractListener *> mListeners;
};

}
