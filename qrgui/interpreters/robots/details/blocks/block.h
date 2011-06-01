#pragma once

#include <QtCore/QObject>

#include "../../../../kernel/ids.h"
#include "../../../../models/graphicalModelAssistApi.h"
#include "../../../../models/logicalModelAssistApi.h"
#include "../../../../mainwindow/errorReporter.h"

#include "../blocksTable.h"

#include "../../../visualDebugger/blockParser.h"

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
	Id const id() const;

	virtual QList<SensorPortPair> usedSensors() const;

signals:
	void done(blocks::Block * const nextBlock);
	void newThread(details::blocks::Block * const startBlock);
	void failure();

protected:
	Block *mNextBlock;  // Does not have ownership
	models::GraphicalModelAssistApi const *mGraphicalModelApi;  // Does not have ownership
	models::LogicalModelAssistApi const *mLogicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership

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
			, BlockParser * const parser
			);

private:
	enum State {
		idle
		, running
	};

	State mState;
	Id mGraphicalId;
	gui::ErrorReporter * mErrorReporter;
	BlockParser * mParser;

	virtual bool initNextBlocks();
	virtual void additionalInit() {};
	virtual void run() = 0;
};

}
}
}
}
}
