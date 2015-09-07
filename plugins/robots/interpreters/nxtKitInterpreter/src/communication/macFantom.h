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

#pragma once

#include <QtCore/QLibrary>

namespace nxt {
namespace communication {

/// Fantom driver is not available for Mac OS X, so there is empty implementation.
class Fantom
{
public:
	/// Returns available if fantom driver is available, notexist otherwise. Return x64 if it's 64-bit Mac.
	enum class Status { available, notexist, x64 };

	/// Constructor. Tries to load a dlib.
	Fantom();

	Status availability();

	// Fantom library methods

	unsigned long nFANTOM100_createNXT(char resString[], int status, unsigned char checkFVersion);
	unsigned nFANTOM100_iNXT_sendDirectCommand(unsigned long nxtHandle, bool requireResponse, const char *inputBufferPtr
			, int inputBufferSize, char *outputBufferPtr, int outputBufferSize, int &status);

	unsigned long nFANTOM100_createNXTIterator(unsigned char searchBluetooth
			, unsigned long bluetoothSearchTimeout, int &status);
	void nFANTOM100_iNXTIterator_getName(unsigned long NXTIterHandle, char resString[], int &status);
	unsigned long nFANTOM100_iNXTIterator_getNXT(unsigned long nxtIterHandle, int &status);
	void nFANTOM100_destroyNXTIterator(unsigned long nxtIteratorHandle, int &status);
	void nFANTOM100_iNXTIterator_advance(unsigned long NXTIterHandle, int &status);
	void nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(char resString[], int &status);
	void nFANTOM100_destroyNXT(unsigned long nxtHandle, int &status);

private:
	unsigned long onDriverUnavailable();

	/// Instance of the Fantom library, if it is present.
	QLibrary mFantomLibrary;

	Status mAvailability;
};

}
}
