/* Copyright 2007-2015 QReal Research Group
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

#include <utils/robotCommunication/robotCommunicationException.h>
#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>
#include "macFantom.h"
#include "fantomMethods.h"
#include "fantom.h"

using namespace nxt::communication;

MacFantom::MacFantom()
{
	mFantomLibrary.setFileName("/Library/Frameworks/Fantom.framework/Versions/1/Fantom");
	if (!mFantomLibrary.load()) {
		QLOG_INFO() << mFantomLibrary.errorString();
	}
	if (mFantomLibrary.isLoaded()) {
		mAvailability = Status::available;
	} else {
		mAvailability = mFantomLibrary.errorString().contains("no matching architecture in universal wrapper")
				? Status::x64 : Status::notFound;
	}
}

MacFantom::Status MacFantom::availability() const
{
	return mAvailability;
}

bool MacFantom::isAvailable() const
{
	return mFantomLibrary.isLoaded();
}

void MacFantom::checkConsistency()
{
	if (availability() == MacFantom::Status::x64) {
		const QString errorMessage = tr("Usb connection to robot is impossible. "
				"Lego doesn't have Fantom Driver for 64-bit Mac. "
				"You will only be able to connect to NXT via Bluetooth.");
		emit errorOccured(errorMessage);
		return;
	}

	if (availability() == MacFantom::Status::notFound) {
		Fantom::checkConsistency();
	}
}
