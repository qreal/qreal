/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QFile>
#include <QtCore/QDir>

/// Utility to compress and decompress folder uzing qCompress function.
class FolderCompressor
{
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



