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

#include "generator.h"

#include <QtCore/QDebug>

#include "messageGenerator.h"
#include "customClassGenerator.h"
#include "dispatcherGenerator.h"

using namespace ubiq::generator;

Generator::Generator()
{
}

Generator::~Generator()
{
}

void Generator::init(qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
{
	mLogicalModel = &logicalModel;
	mErrorReporter = &errorReporter;
}

void Generator::generate()
{
	MessageGenerator generator("./templates", "./output", *mLogicalModel, *mErrorReporter);
	generator.generate();

	CustomClassGenerator customClassGenerator("./templates/", "./output/", *mLogicalModel, *mErrorReporter);
	customClassGenerator.generate();

	DispatcherGenerator dispatcherGenerator("./templates", "./output", *mLogicalModel, *mErrorReporter);
	dispatcherGenerator.generate();
}
