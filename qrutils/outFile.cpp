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

#include "outFile.h"

#include <qrkernel/exception/exception.h>

using namespace utils;

OutFile::OutFile(const QString &fileName)
{
	mFile.setFileName(fileName);
	mFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if (!mFile.isOpen()) {
		throw qReal::Exception("File open operation failed");
	}

	mOut.setDevice(&mFile);
	mOut.setCodec("UTF-8");
}

QTextStream &OutFile::operator()()
{
	return mOut;
}

OutFile::~OutFile()
{
	mFile.close();
}
