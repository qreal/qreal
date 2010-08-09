#include "projectTypeFixer.h"
#include <QFile>
#include <QDir>
#include <qDebug>

void ProjectTypeFixer::fix()
{
	qDebug() << "Project fixing started";
	QString editorsDirectoryName("..\\..\\..\\unreal\\trunk\\qrxml");
	QDir editorsDirectory(editorsDirectoryName);
	editorsDirectory.setFilter(QDir::Dirs);
	QStringList nameFilters;
	nameFilters << "*Editor";
	editorsDirectory.setNameFilters(nameFilters);
	QStringList fileNames = editorsDirectory.entryList();
	foreach(QString fileNameShort, fileNames)
	{
		qDebug() << "Fixing project " << fileNameShort;
		fixProject(editorsDirectoryName + "\\" + fileNameShort + "\\" + fileNameShort + ".vcproj");
	}
	qDebug() << "Project fixing finished";
}

void ProjectTypeFixer::fixProject( const QString &fileName )
{
	QFile editorProjectFile(fileName);
	if (editorProjectFile.open(QIODevice::ReadWrite))
	{
		QTextStream editorProjectFileStream(&editorProjectFile);
		QString str;
		do {
			qint64 filePosition = editorProjectFileStream.pos();
			str = editorProjectFileStream.readLine();
			if (str.contains("ConfigurationType"))
			{
				int stringPosition = str.indexOf("1") + 1;
				str.insert(stringPosition,"0");
				qint64 filePosition2 = editorProjectFileStream.pos();
				editorProjectFileStream.seek(filePosition);
				editorProjectFileStream << str;
				editorProjectFileStream.seek(filePosition2);
			}
		} while (!str.isNull());
		editorProjectFile.close();
	}
}