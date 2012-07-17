#pragma once

#include "ids.h"
#include "roles.h"

enum DragFrom {
	fromLogicalModel = 1,
	fromGraphicalModel = 2,
	fromPalette = 3
};

namespace qReal {

	const char ID_PATH_DIVIDER = '#';

	const QString DEFAULT_MIME_TYPE = "application/x-real-uml-data";

}
