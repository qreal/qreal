#include "logicalModelAssistApi.h"

#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace models::details;

LogicalModelAssistApi::LogicalModelAssistApi(LogicalModel &logicalModel
		, EditorManagerInterface const &editorManagerInterface)
	: mModelsAssistApi(logicalModel, editorManagerInterface)
	, mLogicalModel(logicalModel)
	, mEditorManager(editorManagerInterface)
{
	connect(&logicalModel, &LogicalModel::elementAdded, this, &LogicalModelAssistApi::elementAdded);
}

LogicalModelAssistApi::~LogicalModelAssistApi()
{
}

EditorManagerInterface const &LogicalModelAssistApi::editorManagerInterface() const
{
	return mModelsAssistApi.editorManagerInterface();
}

qrRepo::LogicalRepoApi const &LogicalModelAssistApi::logicalRepoApi() const
{
	return mLogicalModel.api();
}

qrRepo::LogicalRepoApi &LogicalModelAssistApi::mutableLogicalRepoApi()
{
	return mLogicalModel.mutableApi();
}

Id LogicalModelAssistApi::createElement(Id const &parent, Id const &type)
{
	Q_ASSERT(type.idSize() == 3);
	Q_ASSERT(parent.idSize() == 4);

	Id const newElementId(type, QUuid::createUuid().toString());
	QString const elementFriendlyName = mModelsAssistApi.editorManagerInterface().friendlyName(type);
	mLogicalModel.addElementToModel(parent, newElementId, Id(), elementFriendlyName, QPointF(0, 0));
	return newElementId;
}

Id LogicalModelAssistApi::createElement(Id const &parent, Id const &id
		, bool isFromLogicalModel, QString const &name
		, QPointF const &position, Id const &preferedLogicalId)
{
	Q_UNUSED(preferedLogicalId)
	return mModelsAssistApi.createElement(parent, id, id, isFromLogicalModel, name, position);
}

IdList LogicalModelAssistApi::children(Id const &element) const
{
	return mLogicalModel.api().children(element);
}

void LogicalModelAssistApi::changeParent(Id const &element, Id const &parent, QPointF const &position)
{
	Q_UNUSED(position);
	mLogicalModel.changeParent(mLogicalModel.indexById(element), mLogicalModel.indexById(parent), QPointF());
}

void LogicalModelAssistApi::addExplosion(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().addExplosion(source, destination);
}

void LogicalModelAssistApi::removeExplosion(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().removeExplosion(source, destination);
}

void LogicalModelAssistApi::setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName)
{
	int const roleIndex = mModelsAssistApi.roleIndexByName(elem, roleName);
	if (roleIndex < roles::customPropertiesBeginRole) {
		return;
	}
	mModelsAssistApi.setProperty(elem, newValue, roleIndex);
}

QVariant LogicalModelAssistApi::propertyByRoleName(Id const &elem, QString const &roleName) const
{
	int roleIndex = mModelsAssistApi.roleIndexByName(elem, roleName);
	if (roleIndex < roles::customPropertiesBeginRole)
		return "";
	return mModelsAssistApi.property(elem, roleIndex);
}

bool LogicalModelAssistApi::isLogicalId(Id const &id) const
{
	return mModelsAssistApi.indexById(id) != QModelIndex();
}

void LogicalModelAssistApi::setName(Id const &elem, QString const &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), Qt::DisplayRole);
}

QString LogicalModelAssistApi::name(Id const &elem) const
{
	return mModelsAssistApi.property(elem, Qt::DisplayRole).value<QString>();
}

void LogicalModelAssistApi::setTo(Id const &elem, Id const &newValue)
{
	mModelsAssistApi.setTo(elem, newValue);
}

Id LogicalModelAssistApi::to(Id const &elem) const
{
	return mModelsAssistApi.to(elem);
}

void LogicalModelAssistApi::setFrom(Id const &elem, Id const &newValue)
{
	mModelsAssistApi.setFrom(elem, newValue);
}

Id LogicalModelAssistApi::from(Id const &elem) const
{
	return mModelsAssistApi.from(elem);
}

QModelIndex LogicalModelAssistApi::indexById(Id const &id) const
{
	return mModelsAssistApi.indexById(id);
}

Id LogicalModelAssistApi::idByIndex(QModelIndex const &index) const
{
	return mModelsAssistApi.idByIndex(index);
}

QPersistentModelIndex LogicalModelAssistApi::rootIndex() const
{
	return mModelsAssistApi.rootIndex();
}

Id LogicalModelAssistApi::rootId() const
{
	return mModelsAssistApi.rootId();
}

bool LogicalModelAssistApi::hasRootDiagrams() const
{
	return mModelsAssistApi.hasRootDiagrams();
}

int LogicalModelAssistApi::childrenOfRootDiagram() const
{
	return mModelsAssistApi.childrenOfRootDiagram();
}

int LogicalModelAssistApi::childrenOfDiagram(const Id &parent) const
{
	return mModelsAssistApi.childrenOfDiagram(parent);
}

void LogicalModelAssistApi::removeReferencesTo(Id const &id)
{
	IdList backReferences = mLogicalModel.api().property(id, "backReferences").value<IdList>();

	foreach (Id const &reference, backReferences) {
		mLogicalModel.api().removeBackReference(id, reference);
		removeReference(reference, id);
	}
}

void LogicalModelAssistApi::removeReferencesFrom(Id const &id)
{
	QStringList referenceProperties = mEditorManager.referenceProperties(id.type());

	foreach (QString const &property, referenceProperties) {
		QStringList propertyValue = mLogicalModel.api().property(id, property).toString().split(','
				, QString::SkipEmptyParts);
		foreach (QString const &value, propertyValue) {
			Id idValue = Id::loadFromString(value);
			mLogicalModel.api().removeBackReference(idValue, id);
		}
	}
}

void LogicalModelAssistApi::removeReference(Id const &id, Id const &reference)
{
	QStringList referenceProperties = mEditorManager.referenceProperties(id.type());

	foreach (QString const &propertyName, referenceProperties) {
		QStringList stringData = mLogicalModel.api().property(id, propertyName).toString().split(','
				, QString::SkipEmptyParts);
		stringData.removeAll(reference.toString());
		mLogicalModel.mutableApi().setProperty(id, propertyName, stringData.join(','));
	}
}

void LogicalModelAssistApi::removeElement(Id const &logicalId)
{
	QPersistentModelIndex const index = indexById(logicalId);
	if (logicalRepoApi().exist(logicalId) && index.isValid()) {
		removeReferencesTo(logicalId);
		mLogicalModel.removeRow(index.row(), index.parent());
	}
}

QMap<Id, Version> LogicalModelAssistApi::editorVersions() const
{
	QMap<Id, Version> result;

	QStringList const metaInformationKeys = logicalRepoApi().metaInformationKeys();
	for (Id const &editor : editorManagerInterface().editors()) {
		QString const key = editor.editor() + "Version";
		if (metaInformationKeys.contains(key)) {
			result[editor] = Version::fromString(logicalRepoApi().metaInformation(key).toString());
		}
	}

	return result;
}
