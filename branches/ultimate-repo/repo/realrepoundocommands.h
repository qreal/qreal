#ifndef __REAL_REPO_INDO_CONNAMDS_H__
#define __REAL_REPO_INDO_CONNAMDS_H__

#include <QString>
#include <QUndoCommand>
#include <QModelIndex>
#include <QVariant>
#include "realrepoclient.h"

class RealRepoModel;

class RealUndoCommand : public QUndoCommand
{
public: 
	RealUndoCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role); 

	virtual void undo();
	virtual void redo();

private:
	RealRepoModel *model;
	QModelIndex index;
	QVariant oldval;
	QVariant newval;
	int role;
};

class ChangePositionCommand : public RealUndoCommand
{
public:
	ChangePositionCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role); 
};

class ChangeConfigurationCommand : public RealUndoCommand
{
public:
	ChangeConfigurationCommand (RealRepoModel *model, const QModelIndex& index, QVariant oldval,const QVariant& newval,int role); 
};

#endif //__REAL_REPO_INDO_CONNAMDS_H__
