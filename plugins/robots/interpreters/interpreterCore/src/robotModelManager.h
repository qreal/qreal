#pragma once

#include <QtCore/QObject>

#include <interpreterBase/robotModel/robotModelInterface.h>

namespace interpreterCore {

/// Manages selected robot models providing
class RobotModelManager : public QObject
{
	Q_OBJECT

public:
	/// Returns single instance of the robot model manager
	static RobotModelManager *instance();

	/// Returns current selected robot model
	static interpreterBase::robotModel::RobotModelInterface *selectedModel();

	/// Must be called every time when user selects other robot model by robot model selector itself
	static void setRobotModel(interpreterBase::robotModel::RobotModelInterface * const robotModel);

signals:
	/// Emitted every time when user selected other robot model
	void robotModelChanged();

private:
	RobotModelManager();
	~RobotModelManager();

	interpreterBase::robotModel::RobotModelInterface *mRobotModel;  // Does not take ownership
};

}
