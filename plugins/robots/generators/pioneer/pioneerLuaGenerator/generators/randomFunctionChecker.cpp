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

#include "randomFunctionChecker.h"

#include <qrgui/plugins/pluginManager/editorManagerInterface.h>
#include <qrrepo/repoApi.h>
#include <qrtext/languageToolboxInterface.h>
#include <qrtext/lua/ast/functionCall.h>
#include <qrtext/lua/ast/identifier.h>

#include "parts/randomGeneratorPart.h"

const QString randomKeyWord = "random";

using namespace pioneer::lua;

RandomFunctionChecker::RandomFunctionChecker(
		const qrRepo::RepoApi &repo
		, const qReal::EditorManagerInterface &metamodel
		, qrtext::LanguageToolboxInterface &languageToolbox
		, RandomGeneratorPart &randomGeneratorPart)
	: mRepo(repo)
	, mMetamodel(metamodel)
	, mLanguageToolbox(languageToolbox)
	, mRandomGeneratorPart(randomGeneratorPart)
{
}

void RandomFunctionChecker::checkNode(const qReal::Id &id)
{
	if (mRandomGeneratorPart.isUsed()) {
		// No point to do expensive AST walk if RNG will be initialized anyway.
		return;
	}

	checkForAdditionalLanguageInfo(id);

	const auto properties = mMetamodel.propertyNames(id.type());
	for (const auto &property : properties) {
		const QString localStringProperty = mRepo.stringProperty(id, property);

		if (localStringProperty.contains(randomKeyWord)) {
			const auto ast = mLanguageToolbox.parse(id, property, localStringProperty);
			if (!ast.isNull()) {
				checkAst(ast);
			}
		}
	}
}

void RandomFunctionChecker::checkAst(QSharedPointer<qrtext::core::ast::Node> ast)
{
	if (ast->is<qrtext::lua::ast::FunctionCall>()) {
		auto call = qrtext::as<qrtext::lua::ast::FunctionCall>(ast);
		if (call->function()->is<qrtext::lua::ast::Identifier>()) {
			// We don't support indirect function calls here, but upstream text language tools do not support them
			// anyways.
			auto idNode = qrtext::as<qrtext::lua::ast::Identifier>(call->function());
			if (idNode->name() == randomKeyWord) {
				mRandomGeneratorPart.registerUsage();
				return;
			}
		}
	}

	for (const auto child : ast->children()) {
		if (child) {
			checkAst(child);
		}
	}
}

void RandomFunctionChecker::checkForAdditionalLanguageInfo(const qReal::Id &id)
{
	if (id.element() == "Randomizer") {
		// Initialize random variable with some integer value to make type inference happy.
		mLanguageToolbox.parse(id, "Variable", mRepo.stringProperty(id, "Variable") + "= 0");
	}
}
