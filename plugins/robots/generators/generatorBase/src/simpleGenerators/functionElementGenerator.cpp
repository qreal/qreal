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

#include "functionElementGenerator.h"

#include "generatorBase/generatorCustomizer.h"
#include "generatorBase/parts/functions.h"

using namespace generatorBase::simple;
using namespace qReal;

FunctionElementGenerator::FunctionElementGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t", QList<Binding *>()
			<< Binding::createConverting("@@BODY@@", "Body", customizer.factory()->functionBlockConverter(id, "Body"))
			, parent)
	, mGenerateToInit(false)  // maybe it will be useful one day...
{
}

QString FunctionElementGenerator::generate()
{
	const QString body = BindingGenerator::generate();
	if (mGenerateToInit) {
		mCustomizer.factory()->functions()->registerFunctionInInitialization(body);
		return QString();
	}

	return body;
}
