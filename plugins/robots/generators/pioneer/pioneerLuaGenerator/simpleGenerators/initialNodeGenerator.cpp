/* Copyright 2017 QReal Research Group
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

#include "initialNodeGenerator.h"

#include <generatorBase/generatorCustomizer.h>

#include <QtCore/QRegularExpression>

using namespace pioneer::lua;

InitialNodeGenerator::InitialNodeGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "initialNode.t", {}, parent)
{
	QString label = customizer.factory()->labelGenerator(id, customizer)->generate();
	QString section = label.section('[', 1).remove(QRegularExpression("].*"));
	addBinding(generatorBase::simple::Binding::createStatic("@@LABEL@@", label));
	addBinding(generatorBase::simple::Binding::createStatic("@@ID@@", section));
}
