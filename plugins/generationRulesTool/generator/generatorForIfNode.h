#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "ast/ifNode.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

/// Class that generates code for if node.
class GeneratorForIfNode
{
public:
	/// Returns generated string.
	/// @param ifNode - node to generate code for.
	/// @param generatorConfigurer - information about model and metamodel.
	static QString generatedResult(QSharedPointer<simpleParser::ast::IfNode> ifNode
			, GeneratorConfigurer generatorConfigurer);
};

}
}
