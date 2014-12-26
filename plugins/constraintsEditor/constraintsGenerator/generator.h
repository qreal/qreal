#pragma once

#include <QtCore/QObject>

#include "../../../qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace constraints {
namespace generator {

class Generator : public QObject
{
	Q_OBJECT

public:
	Generator();
	virtual ~Generator();

	/// Initializes generator with logical model reference and means to report errors.
	void init(qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	/// Starts generation process.
	void generate(qReal::Id const &metamodel);

	QString constraintModelFullName();
	QString constraintConstraintsModelName();
	QString constraintNormalizerConstraintsModelName();
	QString constraintModelId();
	QString buildConfiguration();

private:
	bool isCorrectedName(QString const &name);

	/// Logical model, from which generator takes all information.
	qReal::LogicalModelAssistInterface const *mLogicalModel;

	/// Error reporter object, used to show generation errors in output window.
	qReal::ErrorReporterInterface *mErrorReporter;

	QString mConstraintModelFullName;
	QString mConstraintConstraintsModelName;
	QString mConstraintNormalizerConstraintsModelName;
	QString mConstraintModelId;
	QString mBuildConfiguration;
};

}
}

