#pragma once

#include <QSharedPointer>

#include "robotModel/twoD/trikTwoDRobotModel.h"
#include "trikKitInterpreterCommon/trikbrick.h"
#include "trikScriptRunner/trikScriptRunner.h"

#include "declSpec.h"

namespace trik {

class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT TrikQtsInterpreter
		: public QObject
{
	Q_OBJECT

public:
	TrikQtsInterpreter(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model);
//	~TrikQtsInterpreter() override;

	void interpretStringScript(const QString &script);
	void abort();

	void init();

private:
	//QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;
	TrikBrick mBrick;
	trikScriptRunner::TrikScriptRunner mScriptRunner;
};

}
