#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QSignalMapper>
#include <QtCore/QList>

#include "fakeSender.h"

namespace qrTest {

class ProtocolTester : public QObject
{
	Q_OBJECT

public:
	ProtocolTester();
	~ProtocolTester() override;

	template <typename Func>
	void expectSignal(typename QtPrivate::FunctionPointer<Func>::Object *sender
			, Func signal, QString const &signalName)
	{
		FakeSender *fakeSender = new FakeSender();
		connect(sender, signal, fakeSender, &FakeSender::receive);
		mSenders.append(fakeSender);

		mMapper.setMapping(fakeSender, signalName);
		connect(fakeSender, &FakeSender::send, &mMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
		mSignals.insert(signalName, 0);
	}

	bool isSignalEmitted(QString const &signalName);

	bool allIsGood() const;

	void wait(int timeout);

private slots:
	void onSignal(QString const &signalName);
	void onTimeout();

private:
	QList<FakeSender *> mSenders;
	QSignalMapper mMapper;
	QHash<QString, int> mSignals;
	QTimer mTimer;
	QEventLoop mEventLoop;
};

}
