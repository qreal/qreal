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

#include "fantom.h"

#include <utils/robotCommunication/robotCommunicationException.h>
#include <qrkernel/settingsManager.h>

#include "fantomMethods.h"
#include "macFantom.h"
#include "windowsFantom.h"
#include "linuxFantom.h"

using namespace nxt::communication;

unsigned long const fantomDriverUnavailableResult = 100500;

Fantom::Fantom()
{
}

Fantom *Fantom::correctFantom()
{
#ifdef Q_OS_WIN
	return new WindowsFantom;
#endif

#ifdef Q_OS_LINUX
	return new LinuxFantom;
#endif

#ifdef Q_OS_MAC
	return new MacFantom;
#endif
}

bool Fantom::isAvailable() const
{
	return mFantomLibrary.isLoaded();
}

unsigned long Fantom::nFANTOM100_createNXT(char resString[], int status, unsigned char checkFVersion)
{
	NFANTOM100_createNXT nFANTOM100_createNXTHandle = (NFANTOM100_createNXT)
			(mFantomLibrary.resolve("nFANTOM100_createNXT"));
	if (nFANTOM100_createNXTHandle) {
		return nFANTOM100_createNXTHandle(resString, status, checkFVersion);
	} else {
		return onDriverUnavailable();
	}
}

unsigned Fantom::nFANTOM100_iNXT_sendDirectCommand(unsigned long nxtHandle
		, bool requireResponse
		, const char *inputBufferPtr
		, int inputBufferSize
		, char *outputBufferPtr
		, int outputBufferSize
		, int &status)
{
	NFANTOM100_iNXT_sendDirectCommand nFANTOM100_iNXT_sendDirectCommandHandle
			= (NFANTOM100_iNXT_sendDirectCommand)(mFantomLibrary.resolve("nFANTOM100_iNXT_sendDirectCommand"));
	if (nFANTOM100_iNXT_sendDirectCommandHandle) {
		return nFANTOM100_iNXT_sendDirectCommandHandle(nxtHandle, requireResponse, inputBufferPtr
				, inputBufferSize, outputBufferPtr, outputBufferSize, status);
	} else {
		return onDriverUnavailable();
	}
}

unsigned long Fantom::nFANTOM100_createNXTIterator(unsigned char searchBluetooth
		, unsigned long bluetoothSearchTimeout, int &status)
{
	NFANTOM100_createNXTIterator nFANTOM100_createNXTIteratorHandle = (NFANTOM100_createNXTIterator)
			(mFantomLibrary.resolve("nFANTOM100_createNXTIterator"));
	if (nFANTOM100_createNXTIteratorHandle) {
		return nFANTOM100_createNXTIteratorHandle(searchBluetooth, bluetoothSearchTimeout, status);
	} else {
		return onDriverUnavailable();
	}
}

void Fantom::nFANTOM100_iNXTIterator_getName(unsigned long NXTIterHandle, char resString[], int &status)
{
	NFANTOM100_iNXTIterator_getName nFANTOM100_iNXTIterator_getNameHandle = (NFANTOM100_iNXTIterator_getName)
			(mFantomLibrary.resolve("nFANTOM100_iNXTIterator_getName"));
	if (nFANTOM100_iNXTIterator_getNameHandle) {
		nFANTOM100_iNXTIterator_getNameHandle(NXTIterHandle, resString, status);
	} else {
		onDriverUnavailable();
	}
}

unsigned long Fantom::nFANTOM100_iNXTIterator_getNXT(unsigned long nxtIterHandle, int &status)
{
	NFANTOM100_iNXTIterator_getNXT nFANTOM100_iNXTIterator_getNXTHandle = (NFANTOM100_iNXTIterator_getNXT)
			(mFantomLibrary.resolve("nFANTOM100_iNXTIterator_getNXT"));
	if (nFANTOM100_iNXTIterator_getNXTHandle) {
		return nFANTOM100_iNXTIterator_getNXTHandle(nxtIterHandle, status);
	} else {
		return onDriverUnavailable();
	}
}

void Fantom::nFANTOM100_destroyNXTIterator(unsigned long nxtIteratorHandle, int &status)
{
	NFANTOM100_destroyNXTIterator nFANTOM100_destroyNXTIteratorHandle = (NFANTOM100_destroyNXTIterator)
			(mFantomLibrary.resolve("nFANTOM100_destroyNXTIterator"));
	if (nFANTOM100_destroyNXTIteratorHandle) {
		nFANTOM100_destroyNXTIteratorHandle(nxtIteratorHandle, status);
	} else {
		onDriverUnavailable();
	}
}

void Fantom::nFANTOM100_iNXTIterator_advance(unsigned long NXTIterHandle, int &status)
{
	NFANTOM100_iNXTIterator_advance nFANTOM100_iNXTIterator_advanceHandle = (NFANTOM100_iNXTIterator_advance)
			(mFantomLibrary.resolve("nFANTOM100_iNXTIterator_advance"));
	if (nFANTOM100_iNXTIterator_advanceHandle) {
		nFANTOM100_iNXTIterator_advanceHandle(NXTIterHandle, status);
	} else {
		onDriverUnavailable();
	}
}

void Fantom::nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(char resString[], int &status)
{
	NFANTOM100_iNXT_findDeviceInFirmwareDownloadMode nFANTOM100_iNXT_findDeviceInFirmwareDownloadModeHandle
			= (NFANTOM100_iNXT_findDeviceInFirmwareDownloadMode)
					(mFantomLibrary.resolve("nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode"));
	if (nFANTOM100_iNXT_findDeviceInFirmwareDownloadModeHandle) {
		nFANTOM100_iNXT_findDeviceInFirmwareDownloadModeHandle(resString, status);
	} else {
		onDriverUnavailable();
	}
}

void Fantom::nFANTOM100_destroyNXT(unsigned long nxtHandle, int &status)
{
	NFANTOM100_destroyNXT nFANTOM100_destroyNXT
			= (NFANTOM100_destroyNXT)(mFantomLibrary.resolve("nFANTOM100_destroyNXT"));
	if (nFANTOM100_destroyNXT && nxtHandle) {
		nFANTOM100_destroyNXT(nxtHandle, status);
	} else {
		onDriverUnavailable();
	}
}

unsigned long Fantom::onDriverUnavailable()
{
	return fantomDriverUnavailableResult;
}

void Fantom::checkConsistency()
{
	const QString selectedKit = qReal::SettingsManager::value("SelectedRobotKit").toString();
	if (selectedKit != "nxtKit") {
		return;
	}

	const QString selectedRobotModel = qReal::SettingsManager::value("SelectedModelFor" + selectedKit).toString();
	if (selectedRobotModel != "NxtUsbRealRobotModel") {
		return;
	}

	if (!isAvailable()) {
		const QString fantomDownloadLink = qReal::SettingsManager::value("fantomDownloadLink").toString();
		QString errorMessage = tr("Fantom Driver is unavailable. Usb connection to robot is impossible.");
		if (!fantomDownloadLink.isEmpty()) {
			errorMessage += tr(" You can download Fantom Driver on <a href='%1'>Lego website</a>")
				.arg(fantomDownloadLink);
		}

		emit errorOccured(errorMessage);
	}
}
