#pragma once

#include <QtCore/QObject>

#include "../../../../../qrgui/kernel/ids.h"
#include "../../../../../qrgui/models/graphicalModelAssistApi.h"
#include "../../../../../qrgui/models/logicalModelAssistApi.h"
#include "../../../../../qrgui/mainwindow/errorReporter.h"

#include "../blocksTable.h"

#include "../robotsBlockParser.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class Block : public QObject
{
	Q_OBJECT

public:
	typedef QPair<sensorType::SensorTypeEnum, int> SensorPortPair;

	virtual ~Block() {};

	void interpret();
	void setFailedStatus();
	void setIdleStatus();
	Id const id() const;

	virtual QList<SensorPortPair> usedSensors() const;

	virtual void stopActiveTimerInBlock();

signals:
	void done(blocks::Block * const nextBlock);
	void newThread(details::blocks::Block * const startBlock);
	void failure();

protected:
	Block *mNextBlock;  // Does not have ownership
	models::GraphicalModelAssistApi const *mGraphicalModelApi;  // Does not have ownership
	models::LogicalModelAssistApi const *mLogicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership

	Id mGraphicalId;
	RobotsBlockParser * mParser;

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

private slots:
	void finishedRunning();

private:
	friend class qReal::interpreters::robots::details::BlocksFactory;
	void init(Id const &graphicalId
			, models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi
			, BlocksTable &blocksTable
			, gui::ErrorReporter * const errorReporter
			, RobotsBlockParser * const parser
			);

private:
	enum State {
		idle
		,running
		,failed
	};

	State mState;
	gui::ErrorReporter * mErrorReporter;

	virtual bool initNextBlocks();
	virtual void additionalInit() {};
	virtual void run() = 0;
};

}
}
}
}
}
