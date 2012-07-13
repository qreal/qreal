#include "folderCompressor.h"

FolderCompressor::FolderCompressor()
{
}

bool FolderCompressor::compressFolder(QString const &sourceFolder, QString const &destinationFile)
{
	if (!QDir(sourceFolder).exists()) { // folder not found
		return false;
	}

	mFile.setFileName(destinationFile);
	if (!mFile.open(QIODevice::WriteOnly)) { // could not open mFile
		return false;
	}

	mDataStream.setDevice(&mFile);

	bool result = compress(sourceFolder, "");
	mFile.close();

	return result;
}

bool FolderCompressor::compress(QString const &sourceFolder, QString const &prefix)
{
	QDir dir(sourceFolder);
	if (!dir.exists()) {
		return false;
	}

	// 1 - list all folders inside the current folder including hidden
	//      ones, like '.svn/' and '.git/' needed for correct versioning
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Hidden);
	QFileInfoList foldersList = dir.entryInfoList();

	// 2 - For each folder in list: call the same function with folders' paths
	foreach (QFileInfo const &folder, foldersList) {
		QString folderName = folder.fileName();
		QString folderPath = dir.absolutePath() + "/" + folderName;
		QString newPrefix = prefix + "/" + folderName;
		compress(folderPath, newPrefix);
	}

	// 3 - List all files inside the current folder
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	QFileInfoList filesList = dir.entryInfoList();

	// 4 - If folder is empty it also needs to be compressed.
	//       Writing '/' character to the end of path to distinguish it from files
	//       (else we can have issue with subversion)
	if (filesList.empty())
	{
		mDataStream << QString(prefix + "/");
		mDataStream << QString(); //empty data part, need for correct decompression
		return true;
	}
	// 5- Else: for each mFile in list: add mFile path and compressed binary data
	foreach (QFileInfo const &fileInfo, filesList) {
		QFile file(dir.absolutePath() + "/" + fileInfo.fileName());
		if (!file.open(QIODevice::ReadOnly)) { // couldn't open file
			return false;
		}

		mDataStream << QString(prefix + "/" + fileInfo.fileName());
		mDataStream << qCompress(file.readAll());

		file.close();
	}

	return true;
}

bool FolderCompressor::decompressFolder(QString const &sourceFile, QString const &destinationFolder)
{
	if (!QFile(sourceFile).exists()) { // mFile not found, to handle later
		return false;
	}

	QDir dir;
	if (!dir.mkpath(destinationFolder)) { // could not create folder
		return false;
	}

	mFile.setFileName(sourceFile);
	if (!mFile.open(QIODevice::ReadOnly)) {
		return false;
	}

	mDataStream.setDevice(&mFile);

	while (!mDataStream.atEnd()) {
		QString fileName;
		QByteArray data;

		mDataStream >> fileName >> data; // extract file name and data in order

		QString subfolder; // create any needed folder
		for(int i = fileName.length() - 1; i > 0; i--) {
			if((QString(fileName.at(i)) == QString("\\"))
					|| (QString(fileName.at(i)) == QString("/")))
			{
				subfolder = fileName.left(i);
				dir.mkpath(destinationFolder+"/"+subfolder);
				break;
			}
		}

		if (!fileName.endsWith('/'))
		{
			//we have an empty directory (see part 4 of compression),
			// it was create earlier, continue
			QFile outFile(destinationFolder + "/" + fileName);
			if (!outFile.open(QIODevice::WriteOnly)) {
				mFile.close();
				return false;
			}
			outFile.write(qUncompress(data));
			outFile.close();
		}
	}

	mFile.close();
	return true;
}

