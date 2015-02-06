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
	static QString getOpenFileName(const QString &id
			, QWidget *parent = 0
			, const QString &caption = QString()
			, const QString &dir = QString()
			, const QString &filter = QString()
			, QString *selectedFilter = 0
			, QFileDialog::Options options = 0);

	/// Makes same as QFileDialog::getSaveFileName but with restoring selected
	/// directory last time when promted dialog with the same id
	static QString getSaveFileName(const QString &id
			, QWidget *parent = 0
			, const QString &caption = QString()
			, const QString &dir = QString()
			, const QString &filter = QString()
			, QString *selectedFilter = 0
			, QFileDialog::Options options = 0);

	/// Makes same as QFileDialog::getExistingDirectory but with restoring selected
	/// directory last time when promted dialog with the same id
	static QString getExistingDirectory(const QString &id
			, QWidget *parent = 0
			, const QString &caption = QString()
			, const QString &dir = QString()
			, QFileDialog::Options options = QFileDialog::ShowDirsOnly);

	/// Makes same as QFileDialog::getOpenFileNames but with restoring selected
	/// directory last time when promted dialog with the same id
	static QStringList getOpenFileNames(const QString &id
			, QWidget *parent = 0
			, const QString &caption = QString()
			, const QString &dir = QString()
			, const QString &filter = QString()
			, QString *selectedFilter = 0
			, QFileDialog::Options options = 0);

private:
	static QString lastSelectedDirectory(const QString &id, const QString &defaultDirectory);
	static QString lastDirectoryKey(const QString &id);
	static QString directoryOf(const QString &file);
	static void saveState(const QString &id, const QString &directory);
};

}
