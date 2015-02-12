#include "sceneCustomizer.h"

#include <QtCore/QObject>

using namespace qReal;

SceneCustomizer::SceneCustomizer()
	: mAddExplosionMenuName(QObject::tr("Add connection"))
	, mChangeExplosionMenuName(QObject::tr("Connect to other"))
	, mDeleteExplosionMenuName(QObject::tr("Disconnect"))
	, mGoToConnectedMenuName(QObject::tr("Go to connected element"))
	, mShowExpandConnectionAction(false)
	, mExpandExplosionActionText(QObject::tr("Expand explosion"))
	, mCollapseExplosionActionText(QObject::tr("Collapse explosion"))
{
}

QString SceneCustomizer::addExplosionMenuName() const
{
	return mAddExplosionMenuName;
}

QString SceneCustomizer::changeExplosionMenuName() const
{
	return mChangeExplosionMenuName;
}

QString SceneCustomizer::deleteExplosionMenuName() const
{
	return mDeleteExplosionMenuName;
}

QString SceneCustomizer::goToConnectedMenuName() const
{
	return mGoToConnectedMenuName;
}

bool SceneCustomizer::showExpandConnectionAction() const
{
	return mShowExpandConnectionAction;
}

QString SceneCustomizer::expandExplosionActionText() const
{
	return mExpandExplosionActionText;
}

QString SceneCustomizer::collapseExplosionActionText() const
{
	return mCollapseExplosionActionText;
}

void SceneCustomizer::setAddExplosionMenuName(const QString &text)
{
	mAddExplosionMenuName = text;
}

void SceneCustomizer::setChangeExplosionMenuName(const QString &text)
{
	mChangeExplosionMenuName = text;
}

void SceneCustomizer::setDeleteExplosionMenuName(const QString &text)
{
	mDeleteExplosionMenuName = text;
}

void SceneCustomizer::setGoToConnectedMenuName(const QString &text)
{
	mGoToConnectedMenuName = text;
}

void SceneCustomizer::setShowExpandConnectionAction(bool show)
{
	mShowExpandConnectionAction = show;
}

void SceneCustomizer::setExpandExplosionActionText(const QString &text)
{
	mExpandExplosionActionText = text;
}

void SceneCustomizer::setCollapseExplosionActionText(const QString &text)
{
	mCollapseExplosionActionText = text;
}
