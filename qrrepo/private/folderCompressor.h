#pragma once

#include <QtCore/QFile>
#include <QtCore/QDir>

/// Utility to compress and decompress folder uzing qCompress function.
class FolderCompressor {
public:
	/// A recursive function that scans all files inside the source folder
	/// and serializes all files in a row of file names and compressed
	/// binary data in a single file
	/// @returns true if operation was successful.
	static bool compressFolder(const QString &sourceFolder, const QString &destinationFile);

	/// A function that deserializes data from the compressed file and
	/// creates any needed subfolders before saving the file
	/// @returns true if operation was successful.
	static bool decompressFolder(const QString &sourceFile, const QString &destinationFolder);

private:
	/// Creating is prohibited, utility class instances can not be created.
	FolderCompressor();

	static bool compress(const QString &sourceFolder, const QString &prefix, QDataStream &dataStream);
};



