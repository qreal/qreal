#pragma once

#include <QtCore/QObject>

#include "../../../../../qrkernel/ids.h"
#include "../../../../../qrgui/models/graphicalModelAssistApi.h"
#include "../../../../../qrgui/models/logicalModelAssistApi.h"
#include "../../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

#include "../blocksTable.h"

#include "../robotsBlockParser.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

/// Base class for all blocks implementations used in interpreter
class Block : public QObject
{
	Q_OBJECT

public:
	typedef QPair<robots::enums::sensorType::SensorTypeEnum, int> SensorPortPair;

	virtual ~Block();

	/// Starts block execution
	void interpret();

	virtual void setFailedStatus();
	void setIdleStatus();
	Id const id() const;

	virtual QList<SensorPortPair> usedSensors() const;

	/// Called each time when control flow has reached the end block of the
	/// requested for stepping into diagram
	virtual void finishedSteppingInto();

signals:
	void done(blocks::Block * const nextBlock);
	void newThread(details::blocks::Block * const startBlock);
	void failure();

	/// Emitted each time when execution must be continued from the initial block
	/// of the specified diagram
	void stepInto(Id const &diagram);

protected:
	Block();

	QVariant property(QString const &propertyName) const;
	QString stringProperty(QString const &propertyName) const;
	int intProperty(QString const &propertyName) const;
	bool boolProperty(QString const &propertyName) const;

	QVariant property(Id const &id, QString const &propertyName) const;
	QString stringProperty(Id const &id, QString const &propertyName) const;
	int intProperty(Id const &id, QString const &propertyName) const;
	bool boolProperty(Id const &id, QString const &propertyName) const;

	void error(QString const &message);

	QVariant evaluate(QString const &propertyName);
	bool evaluateBool(QString const &propertyName);

	QVector<bool> parseEnginePorts() const;

	Block *mNextBlock;  // Does not have ownership
	GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	LogicalModelAssistInterface const *mLogicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership

	Id mGraphicalId;
	RobotsBlockParser * mParser;

private slots:
	void finishedRunning();

private:
	friend class qReal::interpreters::robots::details::BlocksFactory;
	void init(Id const &graphicalId
			, GraphicalModelAssistInterface const &graphicalModelApi
			, LogicalModelAssistInterface const &logicalModelApi
			, BlocksTable &blocksTable
			, ErrorReporterInterface * const errorReporter
			, RobotsBlockParser * const parser
			);

private:
	enum State {
		idle
		,running
		,failed
	};

	State mState;
	ErrorReporterInterface * mErrorReporter;

	virtual bool initNextBlocks();
	virtual void additionalInit() {}
	virtual void run() = 0;
};

}
}
}
}
}
