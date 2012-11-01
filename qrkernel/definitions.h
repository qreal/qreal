#pragma once

#include "ids.h"
#include "roles.h"

enum DragFrom {
	fromLogicalModel = 1,
	fromGraphicalModel = 2,
	fromPalette = 3
};

namespace qReal {

/// Separator used in situations where there is need to form path consisting of
/// multiple Ids.
const char ID_PATH_DIVIDER = '#';

/// MIME type for drag-and-drop operations inside QReal
const QString DEFAULT_MIME_TYPE = "application/x-real-uml-data";

}
