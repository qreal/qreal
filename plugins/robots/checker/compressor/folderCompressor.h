/* Copyright 2015 CyberTech Labs Ltd.
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

class QString;
class QDataStream;

namespace compressor {

class FolderCompressor {
public:
	/// A recursive function that scans all files inside the source folder
	/// and serializes all files in a row of file names and compressed
	/// binary data in a single file.
	static bool compressFolder(const QString &sourceFolder, const QString &destinationFile);

	/// A function that deserializes data from the compressed file and
	/// creates any needed subfolders before saving the file.
	static bool decompressFolder(const QString &sourceFile, const QString &destinationFolder);

private:
	static bool compress(const QString &sourceFolder, const QString &prefix, QDataStream &dataStream);
};

}
