#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QSettings>

#include <qrrepo/logicalRepoApi.h>

namespace Ui {
class SpecifyPathToGeneratedCodeDialog;
}

namespace qReal {
namespace gui {

/// In this window you can specify path to generated code.
class SpecifyPathToGeneratedCodeDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param metamodelRepoApi - metamodel data (paths are stored in metamodel).
	/// @param parent - parent of this dialog.
	SpecifyPathToGeneratedCodeDialog(
			qrRepo::LogicalRepoApi *metamodelRepoApi
			, QWidget *parent = 0);
	~SpecifyPathToGeneratedCodeDialog();

	/// Returns current path to folder with generated code.
	QString currentPathToFolder() const;
	/// Returns current main file name.
	QString currentFileName() const;

signals:
	/// Emitted when ok button pushed and paths saved.
	void pathsSpecified();

private slots:
	void restoreSettings();

	void saveSettings();

	void specifyFolder();

private:
	bool directoryCreated(const QString &directory) const;

	bool directoryRemoved(const QString &directory) const;

	static void clearDir(const QString &path);

	qrRepo::LogicalRepoApi *mMetamodelRepoApi;  // doesnt' have ownership.

	Ui::SpecifyPathToGeneratedCodeDialog *mUi;  // doesn't have owhership.
};

}
}
