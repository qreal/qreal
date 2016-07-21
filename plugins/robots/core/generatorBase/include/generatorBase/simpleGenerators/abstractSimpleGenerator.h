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

#include <qrrepo/repoApi.h>
#include "generatorBase/templateParametrizedEntity.h"

namespace generatorBase {

class GeneratorCustomizer;

namespace simple {

/// A base for all code generators generating code for one block.
class AbstractSimpleGenerator : public QObject, public TemplateParametrizedEntity
{
public:
	AbstractSimpleGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);

	virtual ~AbstractSimpleGenerator();

	/// Returns a reference to repository used by this generator.
	const qrRepo::RepoApi &repo() const;

	/// Returns a reference to an object that customizes this generator.
	GeneratorCustomizer &customizer() const;

	/// Returns an id of the block that this generator generates code for.
	qReal::Id id() const;

	/// Generates and returns code using parameters specified in constructor
	virtual QString generate() = 0;

protected:
	const qrRepo::RepoApi &mRepo;
	GeneratorCustomizer &mCustomizer;

	const qReal::Id mId;
};

}
}
