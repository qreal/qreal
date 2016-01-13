#include "generatorForLinks.h"

using namespace constraints::generator::generatorParts;
using namespace qReal;

bool GeneratorForLinks::linkWithGivenTypeExists(
		const qReal::Id &element
		, const QString &type
		, const qrRepo::LogicalRepoApi &api)
{
	for (const Id &link : api.links(element)) {
		if (link.element() == type) {
			return true;
		}
	}

	return false;
}
