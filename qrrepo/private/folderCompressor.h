#pragma once

#include <QtCore/QFile>
#include <QtCore/QDir>

class FolderCompressor {
public:
	FolderCompressor();

	/// A recursive function that scans all files inside the source folder
	/// and serializes all files in a row of file names and compressed
	/// binary data in a single file
	bool compressFolder(QString const &sourceFolder, QString const&destinationFile);

	/// A function that deserializes data from the compressed file and
	/// creates any needed subfolders before saving the file
	bool decompressFolder(QString const &sourceFile, QString const &destinationFolder);
private:
	QFile mFile;
	QDataStream mDataStream;
	bool compress(QString const &sourceFolder, QString const &prefix);
};



