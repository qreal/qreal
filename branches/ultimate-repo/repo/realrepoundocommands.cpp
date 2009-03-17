#include "realrepoundocommands.h"
#include "realrepomodel.h"

RealUndoCommand::RealUndoCommand( RealRepoModel *model, const QModelIndex& index, QVariant oldval, const QVariant & newval, int role):
	model(model), index(index), oldval(oldval), newval(newval), role(role)
{
	qDebug() << oldval << newval;
}

void RealUndoCommand::undo()
{
	model->safeSetData(index, oldval, role);
}

void RealUndoCommand::redo()
{
	model->safeSetData(index, newval, role);
}

ChangePositionCommand::ChangePositionCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role) : RealUndoCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing position from (%1:%2) to (%3:%4)").arg(oldval.toPoint().x()).arg(oldval.toPoint().y()).arg(newval.toPoint().x()).arg(newval.toPoint().y()));
}

ChangeConfigurationCommand::ChangeConfigurationCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role) : RealUndoCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing configuration from (%1) to (%2)").arg(oldval.toString()).arg(newval.toString()));
}

ChangeUserRoleCommand::ChangeUserRoleCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role) : RealUndoCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing UserRole from (%1) to (%2)").arg(oldval.toString()).arg(newval.toString()));
}

ChangeEditCommand::ChangeEditCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role) : RealUndoCommand(model, index, oldval, newval, role)
{
	setText(QString("Changing Edit from (%1) to (%2)").arg(oldval.toString()).arg(newval.toString()));
}
