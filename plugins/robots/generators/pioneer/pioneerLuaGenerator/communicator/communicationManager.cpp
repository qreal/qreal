/* Copyright 2017 QReal Research Group
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

#include "communicator/communicationManager.h"

#include <qrkernel/settingsManager.h>
#include <pioneerKit/constants.h>

#include "communicator/httpCommunicator.h"

using namespace pioneer;
using namespace pioneer::lua;
using namespace qReal;

CommunicationManager::CommunicationManager(
		qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		)
	: mHttpCommunicator(new HttpCommunicator(errorReporter))
{
	Q_UNUSED(robotModelManager)

	auto connectCommunicator = [this](CommunicatorInterface &communicator) {
		connect(
				&communicator
				, &CommunicatorInterface::uploadCompleted
				, this
				, &CommunicationManager::onUploadCompleted
		);

		connect(
				&communicator
				, &CommunicatorInterface::startCompleted
				, this
				, &CommunicationManager::onStartCompleted
		);

		connect(
				&communicator
				, &CommunicatorInterface::stopCompleted
				, this
				, &CommunicationManager::onStopCompleted
		);
	};

	connectCommunicator(*mHttpCommunicator);
}

CommunicationManager::~CommunicationManager()
{
	// Empty destructor to keep QScopedPointer happy.
}

void CommunicationManager::uploadProgram(const QFileInfo &program)
{
	mCurrentAction = Action::uploading;
	communicator().uploadProgram(program);
}

void CommunicationManager::runProgram(const QFileInfo &program)
{
	mCurrentAction = Action::starting;
	communicator().uploadProgram(program);
}

void CommunicationManager::stopProgram()
{
	mCurrentAction = Action::stopping;
	communicator().stopProgram();
}

void CommunicationManager::onUploadCompleted(bool isSuccessful)
{
	Q_UNUSED(isSuccessful);
	done();
}

void CommunicationManager::onStartCompleted(bool isSuccessful)
{
	Q_UNUSED(isSuccessful);
	done();
}

void CommunicationManager::onStopCompleted(bool isSuccessful)
{
	Q_UNUSED(isSuccessful);
	done();
}

CommunicatorInterface &CommunicationManager::communicator() const
{
	return *mHttpCommunicator;
}

void CommunicationManager::done()
{
	switch (mCurrentAction) {
	case Action::none:
		return;
	case Action::starting:
		emit runCompleted();
		break;
	case Action::stopping:
		emit stopCompleted();
		break;
	case Action::uploading:
		emit uploadCompleted();
	}

	mCurrentAction = Action::none;
}
