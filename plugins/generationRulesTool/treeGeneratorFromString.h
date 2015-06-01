#pragma once

#include <QtCore/QSharedPointer>

#include "ast/node.h"

namespace generationRules {

/// Class that generates ast for given string.
class TreeGeneratorFromString
{
public:
	/// Returns ast for given string.
	/// @param stream - string to generate tree for.
	static QSharedPointer<simpleParser::ast::Node> generatedTreeFromString(const QString &stream);
};

}
