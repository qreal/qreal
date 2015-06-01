#pragma once

#include <QtCore/QStringList>

/// Templates (for highlighter and list of templates in dialog).
namespace generationRules {
namespace keywords {

const QStringList generalTemplates = {"this", "CallGeneratorFor", "Generator", "foreach", "generateToFile"
		, "if", "else", "foreach_excludeLast", "exclude", "in"};
const QStringList textTemplates = {"newline", "tab"};
const QStringList linksTemplates = {"incomingLinks", "outcomingLinks", "links", "transitionEnd", "transitionStart"};

}
}
