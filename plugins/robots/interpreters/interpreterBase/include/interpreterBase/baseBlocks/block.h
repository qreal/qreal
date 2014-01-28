#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <interpreterBase/baseBlocks/blockInterface.h>
#include <interpreterBase/baseBlocks/blockParserInterface.h>
#include <interpreterBase/baseBlocks/blocksTableInterface.h>

//#include "../blocksTable.h"

//#include "../robotsBlockParser.h"

namespace interpreterBase {
namespace baseBlocks {

/// Base class for all blocks implementations used in interpreter
class ROBOTS_INTERPRETER_BASE_EXPORT Block : public BlockInterface
{
	Q_OBJECT

public:
//	typedef QPair<robots::enums::sensorType::SensorTypeEnum, int> SensorPortPair;

	virtual ~Block();

	// Override.
	void interpret();

	virtual void setFailedStatus();
	virtual void setIdleStatus();
	virtual qReal::Id const id() const;

//	virtual QList<SensorPortPair> usedSensors() const;

	// Override.
	virtual void finishedSteppingInto();

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

//	QVector<bool> parseEnginePorts() const;

	qReal::Id mNextBlockId;
	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	qReal::LogicalModelAssistInterface const *mLogicalModelApi;  // Does not have ownership

	qReal::Id mGraphicalId;
	BlockParserInterface * mParser;

private slots:
	void finishedRunning();

private:
	friend class BlocksFactory;
	void init(qReal::Id const &graphicalId
			, qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, BlockParserInterface * const parser
			);

private:
	enum State {
		idle
		,running
		,failed
	};

	State mState;
	qReal::ErrorReporterInterface * mErrorReporter;  // Doesn't have ownership.

	virtual bool initNextBlocks();
	virtual void additionalInit();
	virtual void run() = 0;
};

}
}
