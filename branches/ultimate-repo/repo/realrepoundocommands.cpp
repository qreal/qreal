#include "realrepoundocommands.h"
#include "realrepomodel.h"

static const int ChangePositionCommandId = 10;
static const int ChangeConfigurationCommandId = 11;
static const int ChangeUserRoleCommandId = 12;
static const int ChangeEditRoleCommandId = 13;

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
	setText(QString("Changing position from (%1:%2) to (%3:%4)").arg(oldval.toPoint().x()).arg(oldval.toPoint().y()).arg(newval.toPoint().x()).arg(newval.toPoint().y()));
}
bool ChangePositionCommand::mergeWith(const QUndoCommand* command)
{
    if (command->id() != ChangePositionCommandId)
	return false;

	const ChangePositionCommand *other = static_cast<const ChangePositionCommand*>(command);
	if (index != other->index)
	return false;

	newval = other->newval;
	return true;
}
int ChangePositionCommand::id() const
{
	return ChangePositionCommandId;
}

ChangeConfigurationCommand::ChangeConfigurationCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role):
	ChangeRoleCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing configuration from (%1:%2) to (%3:%4)").arg(oldval.toPoint().x()).arg(oldval.toPoint().y()).arg(newval.toPoint().x()).arg(newval.toPoint().y()));
}
bool ChangeConfigurationCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != ChangeConfigurationCommandId)
	return false;

	const ChangeConfigurationCommand *other = static_cast<const ChangeConfigurationCommand*>(command);
	if (index != other->index)
	return false;

	newval = other->newval;
	return true;
}
int ChangeConfigurationCommand::id() const
{
	return ChangeConfigurationCommandId;
}

ChangeUserRoleCommand::ChangeUserRoleCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role):
	ChangeRoleCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing UserRole from (%1:%2) to (%3:%4)").arg(oldval.toPoint().x()).arg(oldval.toPoint().y()).arg(newval.toPoint().x()).arg(newval.toPoint().y()));
}
bool ChangeUserRoleCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != ChangeUserRoleCommandId)
	return false;

	const ChangeUserRoleCommand *other = static_cast<const ChangeUserRoleCommand*>(command);
	if (index != other->index)
	return false;

	newval = other->newval;
	return true;
}
int ChangeUserRoleCommand::id() const
{
	return ChangeUserRoleCommandId;
}

ChangeEditRoleCommand::ChangeEditRoleCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role):
	ChangeRoleCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing EditRole from (%1:%2) to (%3:%4)").arg(oldval.toPoint().x()).arg(oldval.toPoint().y()).arg(newval.toPoint().x()).arg(newval.toPoint().y()));
}
bool ChangeEditRoleCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != ChangeEditRoleCommandId)
	return false;

	const ChangeEditRoleCommand *other = static_cast<const ChangeEditRoleCommand*>(command);
	if (index != other->index)
	return false;

	newval = other->newval;
	return true;
}
int ChangeEditRoleCommand::id() const
{
	return ChangeEditRoleCommandId;
}
