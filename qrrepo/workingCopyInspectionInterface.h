#pragma once

#include <QString>

namespace qrRepo
{
namespace versioning
{

/// Used for notification about working copy modification.
class WorkingCopyInspectionInterface
{
public:
	virtual ~WorkingCopyInspectionInterface() {}

	/// Called when file was added to working copy and
	/// needs to be included under version control
	virtual bool onFileAdded(QString const &filePath, QString const &workingDir) = 0;

	/// Called when file was removed from working copy and
	/// needs to be excluded from version control
	virtual bool onFileRemoved(QString const &filePath, QString const &workingDir) = 0;

	/// Called when file in working copy was changed
	virtual bool onFileChanged(QString const &filePath, QString const &workingDir) = 0;
};

}
}
