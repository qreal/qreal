/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QObject>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace constraints {
namespace generator {

class Generator : public QObject
{
	Q_OBJECT

public:
	Generator();
	virtual ~Generator();

	/// Initializes generator with logical model reference and means to report errors.
	/// @param logical model - information about logical model.
	/// @param errorReporter - reporter resposible for reporting error.
	void init(
			const qReal::LogicalModelAssistInterface &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	/// Starts generation process.
	void generate(const qReal::Id &metamodel);

	/// Returns full name of constraint model.
	QString constraintModelFullName() const;

	/// Returns constraints model name.
	QString constraintConstraintsModelName() const;

	/// Returns normalized constraints model name.
	QString constraintNormalizedConstraintsModelName() const;

	/// Returns constraint model id.
	QString constraintModelId() const;

	/// Returns build configuration.
	QString buildConfiguration() const;

private:
	bool isCorrectName(const QString &name) const;

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

