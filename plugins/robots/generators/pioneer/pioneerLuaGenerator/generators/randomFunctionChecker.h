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

#pragma once

#include <QtCore/QSharedPointer>

#include <qrkernel/ids.h>
#include <qrtext/core/ast/node.h>

namespace qReal {
class EditorManagerInterface;
}

namespace qrRepo {
class RepoApi;
}

namespace qrtext {
class LanguageToolboxInterface;
}

namespace pioneer {
namespace lua {

class RandomGeneratorPart;

/// Class that checks a node if it uses "random()" function.
class RandomFunctionChecker
{
public:
	/// Constructor. Gets reference to metamodel, it is needed to query element properties.
	RandomFunctionChecker(
			const qrRepo::RepoApi &repo
			, const qReal::EditorManagerInterface &metamodel
			, qrtext::LanguageToolboxInterface &languageToolbox
			, RandomGeneratorPart &randomGeneratorPart);


	/// Checks that properties of a node use "random()" function, and if they do, notifies RandomGeneratorPart that
	/// RND initialization is needed.
	void checkNode(const qReal::Id &id);

private:
	/// Checks if given abstract sybtax tree uses "random()" function, and if it does, notifies RandomGeneratorPart that
	/// RND initialization is needed.
	void checkAst(QSharedPointer<qrtext::core::ast::Node> ast);

	/// Adds hints to a text language parser if needed by a node. For example, type inference will not work for random
	/// initialization node without additional hint that initialized variable is in fact integer.
	void checkForAdditionalLanguageInfo(const qReal::Id &id);

	/// Repository with a model.
	const qrRepo::RepoApi &mRepo;

	/// Information about visual language currently being generated.
	const qReal::EditorManagerInterface &mMetamodel;

	/// Reference to a text language parser.
	qrtext::LanguageToolboxInterface &mLanguageToolbox;

	/// Reference to a generator part that keeps track of usages of random number generator.
	RandomGeneratorPart &mRandomGeneratorPart;
};

}
}
