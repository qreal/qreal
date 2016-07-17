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

#include <QtXml/QDomElement>
#include <QtCore/QPointF>

#include <qrkernel/ids.h>

#include "qrgui/plugins/pluginManager/pattern.h"

namespace qReal {

class EditorManager;
class PatternType;

/// @todo: Code of this class should be implicitly moved into qrxc and qrmc.
class PatternParser
{
public:
	PatternParser();

	bool parseGroup(const EditorManager *editorManager, const PatternType &pattern);
	const Pattern &pattern() const;

private:
	bool parseGroup(const QDomElement &group);
	void parseNode(const QDomElement &node, Pattern &pattern);
	void parseEdge(const QDomElement &edge, Pattern &pattern);

	QString mEditor;
	QString mDiagram;
	const EditorManager *mEditorManager;
	Pattern mPattern;

};
}
