/* Copyright 2012-2016 Dmitry Mordvinov, Denis Kogutich
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

#include "explosionCommand.h"

#include <qrkernel/logging.h>
#include <qrutils/inFile.h>

using namespace qReal::commands;

ExplosionCommand::ExplosionCommand(models::LogicalModelAssistApi &logicalApi
		, const models::GraphicalModelAssistApi * const graphicalApi
		, const Id &source, const Id &target, bool mustAdd)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mSource(source)
	, mTarget(target)
	, mMustAdd(mustAdd)
{
}

ExplosionCommand::~ExplosionCommand()
{
}

bool ExplosionCommand::execute()
{
	return processExplosion(mMustAdd);
}

bool ExplosionCommand::restoreState()
{
	return processExplosion(!mMustAdd);
}

bool ExplosionCommand::processExplosion(bool add)
{
	if (!mLogicalApi.isLogicalId(mSource) && mGraphicalApi) {
		mSource = mGraphicalApi->logicalId(mSource);
	}

	if (!mLogicalApi.isLogicalId(mTarget) && mGraphicalApi) {
		mTarget = mGraphicalApi->logicalId(mTarget);
	}

	if (add) {
		mLogicalApi.addExplosion(mSource, mTarget);
		saveTargetShape();
	} else {
		mLogicalApi.removeExplosion(mSource, mTarget);
	}

	return true;
}

void ExplosionCommand::saveTargetShape()
{
	if (!mLogicalApi.mutableLogicalRepoApi().stringProperty(mTarget, "shape").isEmpty()) {
		return;
	}

	QString errorString;
	QDomDocument shape;
	const QString filePath = ":/generated/shapes/" + mSource.element() + "Class.sdf";
	utils::InFile::readAll(filePath, &errorString);
	if (!errorString.isEmpty()) {
		QLOG_ERROR() << "Error while saving shape:" << errorString;
		return;
	}

	mLogicalApi.mutableLogicalRepoApi().setProperty(mTarget, "shape", shape.toString(0));
}
