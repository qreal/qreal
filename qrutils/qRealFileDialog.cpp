#include "qRealFileDialog.h"

#include <qrkernel/settingsManager.h>

using namespace utils;

QString QRealFileDialog::getOpenFileName(QString const &id
		, QWidget *parent
		, QString const &caption
		, QString const &dir
		, QString const &filter
		, QString *selectedFilter
		, QFileDialog::Options options)
{
	QString const lastDir = lastSelectedDirectory(id, dir);
	QString const result = QFileDialog::getOpenFileName(parent, caption, lastDir, filter, selectedFilter, options);
	saveState(id, directoryOf(result));
	return result;
}

QString QRealFileDialog::getSaveFileName(QString const &id
		, QWidget *parent
		, QString const &caption
		, QString const &dir
		, QString const &filter
		, QString *selectedFilter
		, QFileDialog::Options options)
{
	QString const lastDir = lastSelectedDirectory(id, dir);
	QString const result = QFileDialog::getSaveFileName(parent, caption, lastDir, filter, selectedFilter, options);
	saveState(id, directoryOf(result));
	return result;
}

QString QRealFileDialog::getExistingDirectory(QString const &id
		, QWidget *parent
		, QString const &caption
		, QString const &dir
		, QFileDialog::Options options)
{
	QString const lastDir = lastSelectedDirectory(id, dir);
	QString const result = QFileDialog::getExistingDirectory(parent, caption, lastDir, options);
	saveState(id, result);
	return result;
}

QStringList QRealFileDialog::getOpenFileNames(QString const &id
		, QWidget *parent
		, QString const &caption
		, QString const &dir
		, QString const &filter
		, QString *selectedFilter
		, QFileDialog::Options options)
{
	QString const lastDir = lastSelectedDirectory(id, dir);
	QStringList const result = QFileDialog::getOpenFileNames(parent, caption, lastDir, filter, selectedFilter, options);
	if (!result.isEmpty()) {
		saveState(id, directoryOf(result[0]));
	}

	return result;
}

QString QRealFileDialog::lastSelectedDirectory(QString const &id, QString const &defaultDirectory)
{
	return qReal::SettingsManager::value(lastDirectoryKey(id), defaultDirectory).toString();
}

QString QRealFileDialog::lastDirectoryKey(QString const &id)
{
	return id + "FileDialogLastDir";
}

QString QRealFileDialog::directoryOf(QString const &file)
{
	if (file.isEmpty()) {
		return QString();
	}

	return QFileInfo(file).absoluteDir().absolutePath();
}

void QRealFileDialog::saveState(QString const &id, QString const &directory)
{
	if (!directory.isEmpty()) {
		qReal::SettingsManager::setValue(lastDirectoryKey(id), directory);
	}
}
