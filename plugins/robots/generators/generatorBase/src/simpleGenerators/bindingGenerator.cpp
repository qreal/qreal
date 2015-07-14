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

#include "generatorBase/simpleGenerators/bindingGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

BindingGenerator::BindingGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, const QString &pathToTemplate
		, QList<Binding *> const &bindings
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
	, mPathToTemplate(pathToTemplate)
	, mBindings(bindings)
{
}

BindingGenerator::~BindingGenerator()
{
	qDeleteAll(mBindings);
}

QString BindingGenerator::generate()
{
	QString input = readTemplate(mPathToTemplate);
	foreach (Binding * const binding, mBindings) {
		binding->apply(mRepo, mId, input);
	}

	return input;
}

void BindingGenerator::addBinding(Binding * const binding)
{
	mBindings << binding;
}
