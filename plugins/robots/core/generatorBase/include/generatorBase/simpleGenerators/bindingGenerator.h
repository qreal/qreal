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

#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace simple {

/// A base for all generators that use bindings and templates for generating code
class ROBOTS_GENERATOR_EXPORT BindingGenerator : public AbstractSimpleGenerator
{
public:
	BindingGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, const QString &pathToTemplate
			, QList<Binding *> const &bindings
			, QObject *parent = 0);

	virtual ~BindingGenerator();

	virtual QString generate();

protected:
	/// Useful for calling binding depending on virtual method not to do it in constructor.
	/// Takes ownership on binding.
	void addBinding(Binding * const binding);

private:
	const QString mPathToTemplate;
	QList<Binding *> mBindings;  // Takes ownership
};

}
}
