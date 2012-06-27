#pragma once

#include <QtCore/QObject>

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

class SbtGenerator : public QObject
{
	Q_OBJECT

public:
	SbtGenerator();
	virtual ~SbtGenerator();

	/// Initializes SbtGenerator with logical model reference and means to report errors.
	void init(qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

public slots:
	/// Starts generation process.
	void generate();

private:
	/// Logical model, from which SbtGenerator takes all information.
	qReal::LogicalModelAssistInterface const *mLogicalModel;

	/// Error reporter object, used to show generation errors in output window.
	qReal::ErrorReporterInterface *mErrorReporter;
};
