#pragma once

#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QSignalMapper>

namespace qrTest {

//class ProtocolTester : public QObject
//{
//	Q_OBJECT

//public:
//	ProtocolTester();

//	template <typename Func>
//	void expectSignal(const typename QtPrivate::FunctionPointer<Func>::Object *sender
//			, Func signal, QString const &signalName)
//	{
//		mMapper.setMapping(sender, signalName);
//		connect(sender, signal, &mMapper, &QSignalMapper::map);
//	}

//	bool allIsGood() const;

//private slots:
//	void onSignal(QString const &signalName);

//private:
//	QSignalMapper mMapper;
//	QSet<QString> mSignals;
//};

}
