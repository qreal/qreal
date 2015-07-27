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

#include <qrutils/generator/abstractGenerator.h>

using namespace utils;

namespace ubiq {
namespace generator {

QString const customClassLabel = "CustomClass";

/// Generator for custom class that can be used in Message, handlers or helper functions.
class CustomClassGenerator : public AbstractGenerator
{
public:
	/** Constructor.
	  @param templateDirPath Path to a directory with generation template.
	  @param outputDirPath Path to a directory where <custom class name>.cs shall be generated.
	  @param logicalModel Logical model reference.
	  @param errorReporter Object to return errors to.
	  */
	CustomClassGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~CustomClassGenerator();

	/// Starts generation.
	void generate();

private:
	/// Generates default and "full" constructor for a class.
	QString generateConstructors(qReal::Id const &element);
};

}
}
