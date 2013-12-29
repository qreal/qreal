#pragma once

#include <QtWidgets/QFileDialog>

#include "utilsDeclSpec.h"

namespace utils {

/// Provides a dialog that allow users to select files or directories
/// with saving in settings last selected diractory
class QRUTILS_EXPORT QRealFileDialog
{
public:
	/// Makes same as QFileDialog::getOpenFileName but with restoring selected
	/// directory last time when promted dialog with the same id
	static QString getOpenFileName(QString const &id
			, QWidget *parent = 0
			, QString const &caption = QString()
			, QString const &dir = QString()
			, QString const &filter = QString()
			, QString *selectedFilter = 0
			, QFileDialog::Options options = 0);

	/// Makes same as QFileDialog::getSaveFileName but with restoring selected
	/// directory last time when promted dialog with the same id
	static QString getSaveFileName(QString const &id
			, QWidget *parent = 0
			, QString const &caption = QString()
			, QString const &dir = QString()
			, QString const &filter = QString()
			, QString *selectedFilter = 0
			, QFileDialog::Options options = 0);

	/// Makes same as QFileDialog::getExistingDirectory but with restoring selected
	/// directory last time when promted dialog with the same id
	static QString getExistingDirectory(QString const &id
			, QWidget *parent = 0
			, QString const &caption = QString()
			, QString const &dir = QString()
			, QFileDialog::Options options = QFileDialog::ShowDirsOnly);

	/// Makes same as QFileDialog::getOpenFileNames but with restoring selected
	/// directory last time when promted dialog with the same id
	static QStringList getOpenFileNames(QString const &id
			, QWidget *parent = 0
			, QString const &caption = QString()
			, QString const &dir = QString()
			, QString const &filter = QString()
			, QString *selectedFilter = 0
			, QFileDialog::Options options = 0);

private:
	static QString lastSelectedDirectory(QString const &id, QString const &defaultDirectory);
	static QString lastDirectoryKey(QString const &id);
	static QString directoryOf(QString const &file);
	static void saveState(QString const &id, QString const &directory);
};

}
