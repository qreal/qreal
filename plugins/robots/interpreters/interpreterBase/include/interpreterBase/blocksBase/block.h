#pragma once

#include <QtCore/QObject>
#include <QtGui/QColor>

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

/// Base class for all blocks implementations used in interpreter.
class ROBOTS_INTERPRETER_BASE_EXPORT Block : public BlockInterface
{
	Q_OBJECT

public:
	void interpret() override;

	void setFailedStatus() override;

	qReal::Id const id() const override;

	QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices() const override;

	void finishedSteppingInto() override;

	/// Initializes this block with external components that may be used during the intepretetation process.
	/// @todo Not good.
	void init(qReal::Id const &graphicalId
			, qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, BlockParserInterface * const parser
			, robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::lua::LuaToolbox &newParser
			);

protected:
	Block();

	/// Returns a property of current block with given name as QVariant.
	QVariant property(QString const &propertyName) const;

	/// Returns a property of current block with given name as QString.
	QString stringProperty(QString const &propertyName) const;

	/// Returns a property of current block with given name as int, or 0 if it can't be converted to int.
	int intProperty(QString const &propertyName) const;

	/// Returns a property of current block with given name as bool, or "false" if it can't be converted to bool.
	bool boolProperty(QString const &propertyName) const;

	/// Returns a property with given name of block with given id as QVariant.
	QVariant property(qReal::Id const &id, QString const &propertyName) const;

	/// Returns a property with given name of block with given id as QString.
	QString stringProperty(qReal::Id const &id, QString const &propertyName) const;

	/// Returns a property with given name of block with given id as int, or 0 if it can't be converted to int.
	int intProperty(qReal::Id const &id, QString const &propertyName) const;

	/// Returns a property with given name of block with given id as bool, or "false" if it can't be converted to bool.
	bool boolProperty(qReal::Id const &id, QString const &propertyName) const;

	/// Returns a property of current block with given name as color.
	QColor propertyToColor(QString const &property) const;

	/// Reports error and emits "failure" signal.
	void error(QString const &message);

	/// Returns calculated value of an expression in a property with given name as QVariant.
	QVariant evaluate(QString const &propertyName);

	/// Returns calculated value of an expression in a property with given name as bool.
	bool evaluateBool(QString const &propertyName);

	/// Reference to a robot model which is used by this block.
	robotModel::RobotModelInterface &model();

	/// @todo: there is no such things as protected fields. State of a class shall not be directly available to
	/// descendants.
	qReal::Id mNextBlockId;
	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	qReal::LogicalModelAssistInterface const *mLogicalModelApi;  // Does not have ownership

	qReal::Id mGraphicalId;
	BlockParserInterface * mParser;  // Does not have ownership
	qrtext::lua::LuaToolbox * mNewParser;  // Does not have ownership

private slots:
	void finishedRunning();

private:
	enum State {
		idle
		, running
		, failed
	};

	/// Shall be reimplemented to set ids of next blocks. Default implementation covers usual sequential blocks, like
	/// "motors on", it is reimplemented in "if", "loop" and such kinds of blocks.
	virtual bool initNextBlocks();

	/// Shall be reimplemented to provide semantics of block execution.
	virtual void run() = 0;

	State mState;
	qReal::ErrorReporterInterface * mErrorReporter;  // Doesn't have ownership.
	robotModel::RobotModelManagerInterface const *mRobotModelManager;  // Doesn't have ownership.
};

}
}
