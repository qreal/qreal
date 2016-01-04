#pragma once

#include <QtCore/QString>

namespace constraints {
namespace generator {

const QString fileNamePRO = "constraintsMetamodel.pro";
const QString fileNamePluginH = "constraintsPlugin.h";
const QString fileNamePluginCPP = "constraintsPlugin.cpp";
const QString fileNameDiagramH = "constraintsDiagram.h";
const QString fileNameDiagramCPP = "constraintsDiagram.cpp";

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
