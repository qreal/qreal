/* Copyright 2007-2016 QReal Research Group
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

#include "ids.h"

#include <QtCore/QVariant>
#include <QtCore/QUuid>

using namespace qReal;

Id Id::loadFromString(const QString &string)
{
	const QStringList path = string.split('/');
	Q_ASSERT(path.count() > 0 && path.count() <= 5);
	Q_ASSERT(path[0] == "qrm:");

	Id result;
	switch (path.count()) {
	case 5: result.mId = path[4];
		// Fall-thru
	case 4: result.mElement = path[3];
		// Fall-thru
	case 3: result.mDiagram = path[2];
		// Fall-thru
	case 2: result.mEditor = path[1];
		// Fall-thru
	}
	Q_ASSERT(string == result.toString());
	return result;
}

Id Id::createElementId(const QString &editor, const QString &diagram, const QString &element)
{
	return Id(editor, diagram, element, QUuid::createUuid().toString());
}

Id Id::rootId()
{
	return Id("ROOT_ID", "ROOT_ID", "ROOT_ID", "ROOT_ID");
}

Id::Id(const QString &editor, QString  const &diagram, QString  const &element, QString  const &id)
		: mEditor(editor)
		, mDiagram(diagram)
		, mElement(element)
		, mId(id)
{
	Q_ASSERT(checkIntegrity());
}

Id::Id(const Id &base, const QString &additional)
		: mEditor(base.mEditor)
		, mDiagram(base.mDiagram)
		, mElement(base.mElement)
		, mId(base.mId)
{
	const unsigned baseSize = base.idSize();
	switch (baseSize) {
	case 0:
		mEditor = additional;
		break;
	case 1:
		mDiagram = additional;
		break;
	case 2:
		mElement = additional;
		break;
	case 3:
		mId = additional;
		break;
	default:
		Q_ASSERT(!"Can not add a part to Id, it will be too long");
	}
	Q_ASSERT(checkIntegrity());
}

bool Id::isNull() const
{
	return mEditor.isEmpty() && mDiagram.isEmpty()
			&& mElement.isEmpty() && mId.isEmpty();
}

QString Id::editor() const
{
	return mEditor;
}

QString Id::diagram() const
{
	return mDiagram;
}

QString Id::element() const
{
	return mElement;
}

QString Id::id() const
{
	return mId;
}

Id Id::type() const
{
	return Id(mEditor, mDiagram, mElement);
}

Id Id::sameTypeId() const
{
	return Id(mEditor, mDiagram, mElement, QUuid::createUuid().toString());
}

unsigned Id::idSize() const
{
	if (!mId.isEmpty()) {
		return 4;
	} if (!mElement.isEmpty()) {
		return 3;
	} if (!mDiagram.isEmpty()) {
		return 2;
	} if (!mEditor.isEmpty()) {
		return 1;
	}
	return 0;
}

QUrl Id::toUrl() const
{
	return QUrl(toString());
}

QString Id::toString() const
{
	QString path = "qrm:/" + mEditor;
	if (mDiagram != "") {
		path += "/" + mDiagram;
	} if (mElement != "") {
		path += "/" + mElement;
	} if (mId != "") {
		path += "/" + mId;
	}
	return path;
}

bool Id::checkIntegrity() const
{
	bool emptyPartsAllowed = true;

	if (!mId.isEmpty()) {
		emptyPartsAllowed = false;
	}

	if (!mElement.isEmpty()) {
		emptyPartsAllowed = false;
	} else if (!emptyPartsAllowed) {
		return false;
	}

	if (!mDiagram.isEmpty()) {
		emptyPartsAllowed = false;
	} else if (!emptyPartsAllowed) {
		return false;
	}

	if (mEditor.isEmpty() && !emptyPartsAllowed) {
		return false;
	}

	return true;
}

QVariant Id::toVariant() const
{
	QVariant result;
	result.setValue(*this);
	return result;
}

QVariant IdListHelper::toVariant(const IdList &list)
{
	QVariant v;
	v.setValue(list);
	return v;
}

QDataStream& operator<< (QDataStream &out, const Id &id)
{
	out << id.toString();
	return out;
}

QDataStream& operator>> (QDataStream &in, Id &id)
{
	QString idString;
	in >> idString;
	id = Id::loadFromString(idString);
	return in;
}
