#ifndef __REAL_REPO_INDO_CONNAMDS_H__
#define __REAL_REPO_INDO_CONNAMDS_H__

#include <QString>
#include <QUndoCommand>
#include <QModelIndex>
#include <QVariant>
#include "realrepoclient.h"

class RealRepoModel;

class UndoCommand : public QUndoCommand
{
};

class ChangeRoleCommand : public UndoCommand
{
public:
	ChangeRoleCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role);

	virtual void undo();
	virtual void redo();

protected:
	RealRepoModel *model;
	QModelIndex index;
	QVariant oldval;
	QVariant newval;
	int role;
};

class ChangePositionCommand : public ChangeRoleCommand
{
public:
	ChangePositionCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role);

	virtual bool mergeWith(const QUndoCommand* command);
	virtual int id() const;
};
class ChangeConfigurationCommand : public ChangeRoleCommand
{
public:
	ChangeConfigurationCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role);
	virtual bool mergeWith(const QUndoCommand* command);
	virtual int id() const;
};
class ChangeUserRoleCommand : public ChangeRoleCommand
{
public:
	ChangeUserRoleCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role);
	virtual bool mergeWith(const QUndoCommand* command);
	virtual int id() const;
};
class ChangeEditRoleCommand : public ChangeRoleCommand
{
public:
	ChangeEditRoleCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role);
	virtual bool mergeWith(const QUndoCommand* command);
	virtual int id() const;
};

class DeleteElementCommand : public UndoCommand
{
public:
	DeleteElementCommand(RealRepoModel *model, IdType id);

	virtual void undo();
	virtual void redo();
protected:
	RealRepoModel *model;
	IdType id;
	TypeIdType type;
};

class AddElementCommand : public UndoCommand
{
public:
	AddElementCommand(RealRepoModel *model, IdType parentId, TypeIdType type);

	virtual void undo();
	virtual void redo();
protected:
	RealRepoModel *model;
	IdType id;
	TypeIdType type;

	IdType parentId;

};

#endif //__REAL_REPO_INDO_CONNAMDS_H__
