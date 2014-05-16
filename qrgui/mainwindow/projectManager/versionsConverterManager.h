#pragma once

#include "models/logicalModelAssistApi.h"

namespace qReal {

class MainWindow;

/// Performs validation and convertion of models due to editor versions that created them.
/// @todo: This class is not implemented at the moment.
/// It only displays error message if versions mismatch detected.
class VersionsConverterManager
{
public:
	explicit VersionsConverterManager(MainWindow &mainWindow);

	/// Performs validation and convertion of models due to editor versions that created them.
	bool validateCurrentProject();

private:
	void displayCannotConvertError(Version const &oldVersion);

	MainWindow &mMainWindow;
};

}
