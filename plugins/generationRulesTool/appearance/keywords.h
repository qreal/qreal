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
#include <QtCore/QObject>

/// Templates (for highlighter and list of templates in dialog).
namespace generationRules {
namespace keywords {

const QStringList generalTemplates = {"this", "CallGeneratorFor", "Generator", "foreach", "generateToFile"
		, "if", "else", "foreach_excludeLast", "exclude", "in"};
const QStringList textTemplates = {"newline", "tab"};
const QStringList linksTemplates = {"incomingLinks", "outcomingLinks", "links", "transitionEnd", "transitionStart"};

const QString thisTooltip = QObject::tr("This keyword allows to apply to current element");
const QString callGeneratorForTooltip = QObject::tr("This keyword allows to call some other generator; \n "
		"example: CallGeneratorFor(elementName, generatorName)");
const QString generatorTooltip = QObject::tr("This keyword allows to define generator for given element; \n "
		"example: Generator newGenerator {}");
const QString foreachTooltip = QObject::tr("This keyword allows to traverse collection of elements; \n"
		"example: foreach (identifier in list) {}");
const QString generateToFileTooltip = QObject::tr("This keyword allows to redirect results of generation "
		"for given element into file with given name; \n"
		"example: generateToFile(element, fileName, generatorName)");
const QString ifTooltip = QObject::tr("This keyword allows to check some condition and depending on "
		"results of this check execute first of second subprogram; \n"
		"example: if (condition) {program1} else {program2}");
const QString elseTooltip = QObject::tr("This keyword is a part of if-expression");
const QString excludeLastTooltip = QObject::tr("This keyword allows to traverse some collection "
		"excluding last element; \n"
		"example: foreach_excludeLast (identifier in list) {}");
const QString excludeTooltip = QObject::tr("This keyword allows to exclude some text from the output");
const QString inTooltip = QObject::tr("This keyword is a part of foreach-expression");

const QString newlineTooltip = QObject::tr("This keyword inserts newline symbol into generated code");
const QString tabTooltip = QObject::tr("This keyword inserts tab into generated code");

const QString incomingLinksTooltip = QObject::tr("This keyword allows to obtain incoming links for given element");
const QString outcomingLinksTooltip = QObject::tr("This keyword allows to obtain outcoming links for given element");
const QString linksTooltip = QObject::tr("This keyword allows to obtain all links connected to given element");
const QString transitionEndTooltip = QObject::tr("This keyword allows to obtain ending element for given link");
const QString transitionStartTooltip = QObject::tr("This keyword allows to obtain starting element for given link");

const QMap<QString, QString> templatesAndTooltips = {
	{"this", thisTooltip}
	, {"CallGeneratorFor", callGeneratorForTooltip}
	, {"Generator", generatorTooltip}
	, {"foreach", foreachTooltip}
	, {"generateToFile", generateToFileTooltip}
	, {"if", ifTooltip}
	, {"else", elseTooltip}
	, {"foreach_excludeLast", excludeLastTooltip}
	, {"exclude", excludeTooltip}
	, {"in", inTooltip}

	, {"newline", newlineTooltip}
	, {"tab", tabTooltip}

	, {"incomingLinks", incomingLinksTooltip}
	, {"outcomingLinks", outcomingLinksTooltip}
	, {"links", linksTooltip}
	, {"transitionEnd", transitionEndTooltip}
	, {"transitionStart", transitionStartTooltip}
};

}
}
