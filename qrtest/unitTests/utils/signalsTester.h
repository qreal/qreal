#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QSignalMapper>
#include <QtCore/QList>

#include "details/fakeSender.h"

namespace qrTest {

/// Helper class for checking that some signals were emitted, and emitted in required order.
class SignalsTester : public QObject
{
	Q_OBJECT

public:
	enum OrderEnum {
		inOrder = 0
		, random
	};

	/// Constructor.
	/// @param inOrder - if true, tester fails when signals were emitted in wrong order. Order in which expectSignal()
	///        were called is assumed to be correct. If it fails, it no longer awaits other signals and ignores all
	/// other signals that get delivered.
	explicit SignalsTester(OrderEnum order = random);

	/// Destructor.
	~SignalsTester() override;

	/// Tells tester to expect that given signal will be emitted.
	/// @param sender - object that shall emit a signal.
	/// @param signal - pointer to a signal (for example, &QTimer::timeout)
	/// @param signalName - unique signal name (for example, "timeout"), used to identify a signal and show meaningful
	///        error messages.
	template <typename Func>
	void expectSignal(typename QtPrivate::FunctionPointer<Func>::Object *sender
			, Func signal, QString const &signalName)
	{
		details::FakeSender *fakeSender = new details::FakeSender();
		connect(sender, signal, fakeSender, &details::FakeSender::receive);
		mSenders.append(fakeSender);

		mMapper.setMapping(fakeSender, signalName);
		connect(fakeSender, &details::FakeSender::send, &mMapper
				, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
		mSignals.insert(signalName, 0);
		mSignalsOrder.push_back(signalName);
	}

	/// Returns true if given signal was expected and already emitted.
	bool isSignalEmitted(QString const &signalName) const;

	/// Returns true, if all expected signals were emitted.
	bool allIsGood() const;

	/// Launch event loop and wait for asynhronous signals given amount of milliseconds. wait is aborted if all expected
	/// signals were emitted or signals were emitted in incorrect order.
	void wait(int timeout);

private slots:
	void onSignal(QString const &signalName);
	void onTimeout();

private:
	QList<details::FakeSender *> mSenders;
	QSignalMapper mMapper;
	QHash<QString, int> mSignals;
	QList<QString> mSignalsOrder;
	QTimer mTimer;
	QEventLoop mEventLoop;
	OrderEnum mInOrder;
};

}
