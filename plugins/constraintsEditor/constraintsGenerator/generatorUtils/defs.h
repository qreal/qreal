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

#include <QtCore/QString>

namespace constraints {
namespace generator {

const QString fileNamePRO = "constraintsMetamodel.pro.template";
const QString fileNamePluginH = "constraintsPlugin.h.template";
const QString fileNamePluginCPP = "constraintsPlugin.cpp.template";
const QString fileNameDiagramH = "constraintsDiagram.h.template";
const QString fileNameDiagramCPP = "constraintsDiagram.cpp.template";

const QString keywordForAllEdges = "AllEdges";
const QString keywordForAllNodes = "AllNodes";
const QString keywordForAllLanguages = "AllLanguages";

enum metaType {
	node,
	edge,
	empty
};

}
}
