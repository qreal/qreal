#pragma once

#include "../../qrgui/kernel/definitions.h"
#include "../../qrgui/kernel/NewType.h"
#include "../../qrgui/kernel/ids.h"
#include "classes/object.h"
#include "qrRepoGlobal.h"
#include "serializer.h"

#include <QHash>

namespace qrRepo {

namespace details {

class Client
{
public:
	QRREPO_EXPORT Client(QString const &workingDirectory);
	QRREPO_EXPORT ~Client();
	qReal::IdList children(const qReal::Id &id) const;
	qReal::NewType type(const qReal::Id &id) const;
	qReal::Id parent(const qReal::Id &id) const;
	void changeType(const qReal::Id &id, const qReal::NewType &type) const;
	void setParent(const qReal::Id &id, const qReal::Id &parent);
	void addChild(const qReal::Id &id, const qReal::Id &child, const qReal::NewType &type);
	void addChild(const qReal::Id &id, const qReal::Id &child, qReal::Id const &logicalId, qReal::NewType const &type);
	void removeParent(const qReal::Id &id);
	void removeChild(const qReal::Id &id, const qReal::Id &child);
	void setProperty(const qReal::Id &id, const QString &name, const QVariant &value);
	QVariant property(const qReal::Id &id, const QString &name) const;
	void removeProperty(const qReal::Id &id, const QString &name);
	bool hasProperty(const qReal::Id &id, const QString &name) const;
	void remove(const qReal::Id &id);
	void setTemporaryRemovedLinks(qReal::Id const &id, QString const &direction, qReal::IdList const &linkIdList);
	qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const;
	qReal::IdList temporaryRemovedLinks(qReal::Id const &id) const;
	void removeTemporaryRemovedLinks(qReal::Id const &id);

	qReal::IdList elements() const;
	bool isLogicalId(qReal::Id const &elem) const;
	qReal::Id logicalId(qReal::Id const &elem) const;

	void svnUpdate();
	void svnCommit();

	void printDebug() const;

	void exterminate();
	void open(QString const &workingDir);

	bool exist(qReal::Id const &id) const;

	void saveAll() const;
	void save(qReal::IdList list) const;
	void remove(qReal::IdList list) const;
	void setWorkingDir(QString const &workingDir);

private:
	void init();

	void loadFromDisk();
	void addChildrenToRootObject();

	qReal::IdList idsOfAllChildrenOf(qReal::Id id) const;
	QList<Object*> allChildrenOf(qReal::Id id) const;

	QHash<qReal::Id, Object*> mObjects;
	Serializer serializer;
};

}

}
