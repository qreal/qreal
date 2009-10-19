#include "solutionFileFixer.h"
#include <QFile>
#include <qDebug>

void SolutionFileFixer::fix()
{
	qDebug() << "Solution fixing started";
	QFile solutionFile("..\\..\\..\\unreal\\trunk\\qreal.sln");
	if (solutionFile.open(QIODevice::ReadWrite))
	{
		QTextStream solutionFileStream(&solutionFile);
		QString str;
		do {
			str = solutionFileStream.readLine();
			solutionFileStringList << str;
		} while (!str.isNull());
		fixFile();
		solutionFileStream.seek(0);
		foreach(QString solutionFileString, solutionFileStringList)
		{
			solutionFileStream << solutionFileString << "\n";
		}
		solutionFile.close();
	}
	qDebug() << "Solution fixing finished";
}

void SolutionFileFixer::fixFile()
{
	writeToQrxcDependencies();
	writeQrGuiDependencies();
}

QString SolutionFileFixer::findProjectHash(const QString &projectName)
{
	QString qrxcProjectHash;
	for (int i = 0; i < solutionFileStringList.size(); i++)
	{
		QString solutionFileString = solutionFileStringList[i];
		if (solutionFileString.contains(projectName))
		{
			int position = solutionFileString.lastIndexOf(", ") + 3;
			for (int j = position; j < solutionFileString.length() - 1; j++)
			{
				qrxcProjectHash.append(solutionFileString[j]);
			}
		}
	}
	return qrxcProjectHash;
}

void SolutionFileFixer::writeToQrxcDependencies()
{
	QString qrxcProjectHash = findProjectHash("qrxc");
	for (int i = 0; i < solutionFileStringList.size(); i++)
	{
		QString solutionFileString = solutionFileStringList[i];
		if ((solutionFileString.contains("vcproj"))&& (!solutionFileString.contains("qrxc")))
		{
			solutionFileStringList.insert(i + 1, "\tProjectSection(ProjectDependencies) = postProject");
			solutionFileStringList.insert(i + 2, "\t\t" + qrxcProjectHash + " = " + qrxcProjectHash);
			solutionFileStringList.insert(i + 3, "\tEndProjectSection");
		}
	}
}

void SolutionFileFixer::writeQrGuiDependencies()
{
	QStringList projectHashes;
	for (int i = 0; i < solutionFileStringList.size(); i++)
	{
		QString solutionFileString = solutionFileStringList[i];
		if ((solutionFileString.contains("vcproj")) && (!solutionFileString.contains("qrgui")) && (!solutionFileString.contains("qrxc")))
		{
			int position = solutionFileString.lastIndexOf(", ") + 3;
			QString projectHash;
			for (int j = position; j < solutionFileString.length() - 1; j++)
			{
				projectHash.append(solutionFileString[j]);
			}
			projectHashes << projectHash;
		}
	}
	for (int i = 0; i < solutionFileStringList.size(); i++)
	{
		QString solutionFileString = solutionFileStringList[i];
		if (solutionFileString.contains("qrgui"))
		{
			foreach(QString projectHash, projectHashes)
			{
				solutionFileStringList.insert(i + 2, "\t\t" + projectHash + " = " + projectHash);
			}
			break;
		}
	}
}