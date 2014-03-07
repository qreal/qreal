#include "ids.h"

#include <QtCore/QVariant>
#include <QtCore/QUuid>

using namespace qReal;

Id Id::loadFromString(QString const &string)
{
	QStringList const path = string.split('/');
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

Id Id::createElementId(QString const &editor, QString const &diagram, QString const &element)
{
	return Id(editor, diagram, element, QUuid::createUuid().toString());
}

Id Id::rootId()
{
	return Id("ROOT_ID", "ROOT_ID", "ROOT_ID", "ROOT_ID");
}

Id::Id(QString const &editor, QString  const &diagram, QString  const &element, QString  const &id)
		: mEditor(editor)
		, mDiagram(diagram)
		, mElement(element)
		, mId(id)
{
	Q_ASSERT(checkIntegrity());
}

Id::Id(Id const &base, QString const &additional)
		: mEditor(base.mEditor)
		, mDiagram(base.mDiagram)
		, mElement(base.mElement)
		, mId(base.mId)
{
	unsigned const baseSize = base.idSize();
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

QVariant IdListHelper::toVariant(IdList const &list)
{
	QVariant v;
	v.setValue(list);
	return v;
}

QDataStream& operator<< (QDataStream &out, Id const &id)
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
