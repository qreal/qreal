#pragma once

#include <QtCore/QString>

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>

namespace constraints {
namespace generator {
namespace generatorParts {

/// This class contains methods responsible for generating code for links.
class GeneratorForLinks
{
public:
	/// Returns true if link with given type and connected to given element exists.
	/// @param element - element id.
	/// @param type - needed type.
	/// @param api - information about model.
	/// @returns true if needed link exists, otherwise false.
	static bool linkWithGivenTypeExists(
			const qReal::Id &element
			, const QString &type
			, const qrRepo::LogicalRepoApi &api);
};


}
}
}
