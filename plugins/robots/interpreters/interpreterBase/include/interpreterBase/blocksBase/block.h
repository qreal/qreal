#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <interpreterBase/blocksBase/blockInterface.h>
#include <interpreterBase/blocksBase/blockParserInterface.h>
#include <interpreterBase/blocksBase/blocksFactoryInterface.h>
#include <interpreterBase/blocksBase/blocksTableInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace interpreterBase {
namespace blocksBase {

/// Base class for all blocks implementations used in interpreter
class ROBOTS_INTERPRETER_BASE_EXPORT Block : public BlockInterface
{
	Q_OBJECT

public:
	~Block() override;

	void interpret() override;

	void setFailedStatus() override;
	void setIdleStatus() override;
	qReal::Id const id() const override;

	QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedSensors() const override;

	void finishedSteppingInto() override;

	/// @todo Not good.
	void init(qReal::Id const &graphicalId
			, qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, BlockParserInterface * const parser
			, robotModel::RobotModelManagerInterface const &robotModelManager
			);

protected:
	Block();

	QVariant property(QString const &propertyName) const;
	QString stringProperty(QString const &propertyName) const;
	int intProperty(QString const &propertyName) const;
	bool boolProperty(QString const &propertyName) const;

	QVariant property(qReal::Id const &id, QString const &propertyName) const;
	QString stringProperty(qReal::Id const &id, QString const &propertyName) const;
	int intProperty(qReal::Id const &id, QString const &propertyName) const;
	bool boolProperty(qReal::Id const &id, QString const &propertyName) const;

	void error(QString const &message);

	QVariant evaluate(QString const &propertyName);
	bool evaluateBool(QString const &propertyName);

	robotModel::RobotModelInterface &model();

//	QVector<bool> parseEnginePorts() const;

	qReal::Id mNextBlockId;
	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	qReal::LogicalModelAssistInterface const *mLogicalModelApi;  // Does not have ownership

	qReal::Id mGraphicalId;
	BlockParserInterface * mParser;  // Does not have ownership

private slots:
	void finishedRunning();

private:
	enum State {
		idle
		,running
		,failed
	};

	virtual bool initNextBlocks();
	virtual void additionalInit();
	virtual void run() = 0;

	State mState;
	qReal::ErrorReporterInterface * mErrorReporter;  // Doesn't have ownership.
	robotModel::RobotModelManagerInterface const *mRobotModelManager; // Doesn't have ownership.
};

}
}
