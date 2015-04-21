#pragma once

#include <QtCore/QString>

namespace qrRepo
{

/// Used for notification about working copy modification.
class WorkingCopyInspectionInterface
{
public:
	virtual ~WorkingCopyInspectionInterface() {}

	/// Called when file was added to working copy and
	/// needs to be included under version control
	virtual bool onFileAdded(const QList<QString> &list, QString const &workingDir) = 0;

	/// Called when file was removed from working copy and
	/// needs to be excluded from version control
	virtual bool onFileRemoved(const QList<QString> &list, QString const &workingDir) = 0;

	/// Called when file in working copy was changed
	virtual bool onFileChanged(const QList<QString> &list, QString const &workingDir) = 0;
};

}
