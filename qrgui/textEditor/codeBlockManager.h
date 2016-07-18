/* Copyright 2015 Dmitry Kopytov
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

#include <QtCore/QMap>

#include "qrgui/textEditor/textEditorDeclSpec.h"
#include <qrkernel/ids.h>

namespace qReal {
namespace text {

/// Manages binding generated textual code to source visual diagram elements.
class QRGUI_TEXT_EDITOR_EXPORT CodeBlockManager
{
public:
	CodeBlockManager();

	/// Reads debug symbols file containing info about binding textual code to diagram elements ids.
	void addNewCode(const QString &path);

	/// Forgets debug symbols in the given file.
	void removeCode(const QString &path);

	/// Starts tracking the given file, future requests will relate only to active file.
	void setActive(const QString &path, bool active);

	/// Returns an interval of lines of code related to an element with the given \a id.
	/// First element of resulting pair is the first line of the interval, second -- last.
	QPair<int, int> intervalById(const QString &path, const Id &id) const;

	/// Returns ids of source elements used for generation of the line with a given number.
	QList<Id> idsByLineNumber(const QString &path, int lineNumber) const;

private:
	void readDbgFile(const QString &path);

	QMap<QString, QMap<Id, QPair<int, int> > > mBlockToCode;
	QMap<QString, QMap<int, QList<Id> > > mCodeToBlock;
	QMap<QString, bool> mActive;
};

}
}
