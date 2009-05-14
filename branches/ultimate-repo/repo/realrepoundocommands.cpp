#include "realrepoundocommands.h"
#include "realrepomodel.h"
#include "realreporoles.h"

static const int ChangePositionCommandId = 10;
static const int ChangeConfigurationCommandId = 11;
static const int ChangeUserRoleCommandId = 12;
static const int ChangeEditRoleCommandId = 13;
static const int DeleteElementCommandId = 14;

ChangeRoleCommand::ChangeRoleCommand( RealRepoModel *model, const QModelIndex& index, QVariant oldval, const QVariant & newval, int role):
	model(model), index(index), oldval(oldval), newval(newval), role(role)
{
	qDebug() << oldval << newval;
}
void ChangeRoleCommand::undo()
{
	model->setData(index, oldval, role);
}
void ChangeRoleCommand::redo()
{
	model->setData(index, newval, role);
}

ChangePositionCommand::ChangePositionCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role):
	ChangeRoleCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing Position from (%1:%2) to (%3:%4)").arg(oldval.toPoint().x()).arg(oldval.toPoint().y()).arg(newval.toPoint().x()).arg(newval.toPoint().y()));
}
bool ChangePositionCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != ChangePositionCommandId)
	return false;

	const ChangePositionCommand *other = static_cast<const ChangePositionCommand*>(command);
	if (index != other->index)
	return false;

	newval = other->newval;
	setText(QString("Changing Position from (%1:%2) to (%3:%4)").arg(oldval.toPoint().x()).arg(oldval.toPoint().y()).arg(newval.toPoint().x()).arg(newval.toPoint().y()));
	return true;
}
int ChangePositionCommand::id() const
{
	return ChangePositionCommandId;
}

ChangeConfigurationCommand::ChangeConfigurationCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role):
	ChangeRoleCommand(model, index, oldval, newval, role)
{
	QString commandDescription = "Changing Configuration from ";
	QPolygon poly(oldval.value<QPolygon>());
	foreach ( QPoint point, poly ) commandDescription += QString("(%1,%2);").arg(point.x()).arg(point.y());
	commandDescription += " to ";
	poly = newval.value<QPolygon>();
	foreach ( QPoint point, poly ) commandDescription += QString("(%1,%2);").arg(point.x()).arg(point.y());

	setText(commandDescription);
}
bool ChangeConfigurationCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != ChangeConfigurationCommandId)
	return false;

	const ChangeConfigurationCommand *other = static_cast<const ChangeConfigurationCommand*>(command);
	if (index != other->index)
	return false;

	newval = other->newval;

	//дублируется код конструктора.

	QString commandDescription = "Changing Configuration from ";
	QPolygon poly(oldval.value<QPolygon>());
	foreach ( QPoint point, poly ) commandDescription += QString("(%1,%2);").arg(point.x()).arg(point.y());
	commandDescription += " to ";
	poly = newval.value<QPolygon>();
	foreach ( QPoint point, poly ) commandDescription += QString("(%1,%2);").arg(point.x()).arg(point.y());
	setText(commandDescription);
	//----

	return true;
}
int ChangeConfigurationCommand::id() const
{
	return ChangeConfigurationCommandId;
}

ChangeUserRoleCommand::ChangeUserRoleCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role):
	ChangeRoleCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing Property from \"%1\" to \"%2\"").arg(oldval.toString()).arg(newval.toString()));
}
bool ChangeUserRoleCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != ChangeUserRoleCommandId)
	return false;

	const ChangeUserRoleCommand *other = static_cast<const ChangeUserRoleCommand*>(command);
	if (index != other->index)
	return false;

	newval = other->newval;
	setText(QString("Changing Property from \"%1\" to \"%2\"").arg(oldval.toString()).arg(newval.toString()));
	return true;
}
int ChangeUserRoleCommand::id() const
{
	return ChangeUserRoleCommandId;
}

ChangeEditRoleCommand::ChangeEditRoleCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role):
	ChangeRoleCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing Name from \"%1\" to \"%2\"").arg(oldval.toString()).arg(newval.toString()));
}
bool ChangeEditRoleCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != ChangeEditRoleCommandId)
	return false;

	const ChangeEditRoleCommand *other = static_cast<const ChangeEditRoleCommand*>(command);
	if (index != other->index)
	return false;

	newval = other->newval;
	setText(QString("Changing Name from \"%1\" to (%2)").arg(oldval.toString()).arg(newval.toString()));
	return true;
}
int ChangeEditRoleCommand::id() const
{
	return ChangeEditRoleCommandId;
}

DeleteElementCommand::DeleteElementCommand(RealRepoModel *model, QModelIndex index):
	model(model)
{
	setText("Delete Element");
	RealRepoModel::RepoTreeItem *curItem = static_cast<RealRepoModel::RepoTreeItem*>(index.internalPointer());
	repoId = curItem->id;

	type = model->hashTypes[repoId];

	//сохраняем старые значение ролей
	name = QVariant(model->hashNames[repoId]);
	position = QVariant(model->hashDiagramElements[curItem->parent->id][curItem->id].position);
	configuration = QVariant(model->hashDiagramElements[curItem->parent->id][curItem->id].configuration);

	QStringList propertyNameList = model->info.getColumnNames(type);
	for(int i = 0; i < propertyNameList.size();i++){
		roleValues[model->info.roleByColumnName(type, propertyNameList.at(i))] = model->repoClient->getPropValue(repoId, propertyNameList.at(i));
	}

}
void DeleteElementCommand::undo()
{
	qDebug() << "undo deleteElement" << parentId;
	int i;
	for (i = 0; i < model->hashTreeItems[parentId].size(); i++)
	{
		qDebug() << "testing" << model->type(model->hashTreeItems[parentId].at(i)->parent) << RealRepoModel::Container;
		if (model->type(model->hashTreeItems[parentId].at(i)->parent) == RealRepoModel::Container)
			break;
	}
	if (i == model->hashTreeItems[parentId].size())
		i--;
	RealRepoModel::RepoTreeItem* parentItem = model->hashTreeItems[parentId].at(i);

	QModelIndex parentIndex = model->index(parentItem);

	model->beginInsertRows(parentIndex, parentItem->children.size(), parentItem->children.size());

	qDebug() << "creating object with parent";
	IdType id = model->repoClient->createObjectWithParent(type, name.toString(), parentId, repoId);

	model->endInsertRows();

	//возвращаем старые значения
	RealRepoModel::RepoTreeItem* curItem = model->createItem(parentItem, repoId, type);
	QModelIndex curIndex = model->index(curItem);

	QMap<int, QVariant>::const_iterator it = roleValues.constBegin();
	while (it != roleValues.constEnd()) {
		model->setData(curIndex,it.value(),it.key());
		++it;
	}

	//дописываем имя и позицию
	model->setData(curIndex, name, Qt::EditRole);
	model->setData(curIndex, position, Unreal::PositionRole);
	model->setData(curIndex, configuration, Unreal::ConfigurationRole);


}
void DeleteElementCommand::redo()
{
	qDebug() << "undo deleteElement";

	//находим соответствущий ему RepoTreeItem
	int i;
	for (i = 0; i < model->hashTreeItems[repoId].size(); i++)
		//хак, чтобы отличить аватар от оригинала
		if (model->type(model->hashTreeItems[repoId].at(i)->parent) == RealRepoModel::Container)
			break;
	if (i == model->hashTreeItems[repoId].size())
		return;
	RealRepoModel::RepoTreeItem* curItem = model->hashTreeItems[repoId].at(i);

	//получаем индекс в модели найденного RepoTreeItem
	QModelIndex ind = model->index(curItem);

	//получаем айдишник родителя
	RealRepoModel::RepoTreeItem* parentItem = curItem->parent;
	parentId = parentItem->id;

	//начинаем удаление
	model->beginRemoveRows(ind.parent(), ind.row(), ind.row());

	QStringList l = model->info.getColumnNames(type);

	qDebug() << "Processing referrals";
	foreach(QString p, l)
	{
		// When deleting object, process refs first
		if (model->info.isPropertyRef(type, p))
			model->changeRole(ind, "", model->info.roleByColumnName(type, p));
	}

	model->repoClient->deleteObject(curItem->id, parentItem->id);

	model->hashTreeItems[curItem->id].removeAll(parentItem->children.at(ind.row()));

	delete parentItem->children.at(ind.row());
	parentItem->children.removeAt(ind.row());

	for ( int j = 0; j < parentItem->children.size(); j++ )
		parentItem->children[j]->row = j;

	model->endRemoveRows();
}

