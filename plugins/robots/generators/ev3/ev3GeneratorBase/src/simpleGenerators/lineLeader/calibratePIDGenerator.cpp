/* Copyright 2017 CyberTech Labs Ltd.
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

#include "calibratePIDGenerator.h"

#include <generatorBase/generatorCustomizer.h>

#include "ev3GeneratorBase/ev3GeneratorFactory.h"

using namespace ev3::simple::lineLeader;
using namespace generatorBase::simple;
using namespace qReal;

CalibratePIDGenerator::CalibratePIDGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "sensors/lineLeader/calibratePID.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port"
					, static_cast<Ev3GeneratorFactory *>(customizer.factory())->portNameConverter())
			<< Binding::createConverting("@@SET_POINT@@", "SetPoint"
					, customizer.factory()->intPropertyConverter(id, "SetPoint"))
			<< Binding::createConverting("@@P@@", "P"
					, customizer.factory()->intPropertyConverter(id, "P"))
			<< Binding::createConverting("@@K@@", "K"
					, customizer.factory()->intPropertyConverter(id, "K"))
			<< Binding::createConverting("@@D@@", "D"
					, customizer.factory()->intPropertyConverter(id, "D"))
			<< Binding::createConverting("@@KPF@@", "PFactor"
					, customizer.factory()->intPropertyConverter(id, "PFactor"))
			<< Binding::createConverting("@@KIF@@", "IFactor"
					, customizer.factory()->intPropertyConverter(id, "IFactor"))
			<< Binding::createConverting("@@KDF@@", "DFactor"
					, customizer.factory()->intPropertyConverter(id, "DFactor"))
			, parent)
{
}
