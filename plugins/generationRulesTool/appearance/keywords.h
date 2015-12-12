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

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QString>

/// Templates (for highlighter and list of templates in dialog).
namespace generationRules {
namespace keywords {

const QStringList generalTemplates = {"this", "CallGeneratorFor", "Generator", "foreach", "generateToFile"
		, "if", "else", "foreach_excludeLast", "exclude", "in"};
const QStringList textTemplates = {"newline", "tab"};
const QStringList linksTemplates = {"incomingLinks", "outcomingLinks", "links", "transitionEnd", "transitionStart"};

const QMap<QString, QString> templatesAndTooltips = {
	{"this", "this construction allows to apply to current element"}
	, {"CallGeneratorFor", "this construction allows to call some other generator; \n "
		"example: CallGeneratorFor(elementName, generatorName)"}
	, {"Generator", "this construction allows to define generator for given element; \n "
		"example: Generator newGenerator {}" }
	, {"foreach", ""}
	, {"generateToFile", ""}
	, {"if", ""}
	, {"else", ""}
	, {"foreach_excludeLast", ""}
	, {"exclude", ""}
	, {"in", ""}
};

}
}
