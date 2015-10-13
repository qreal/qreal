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

typedef unsigned long (*NFANTOM100_createNXT)(char resString[], int status, unsigned char checkFVersion);
typedef unsigned int (*NFANTOM100_iNXT_sendDirectCommand)(unsigned long nxtHandle, bool requireResponse
		, const char *inputBufferPtr, unsigned long inputBufferSize, char *outputBufferPtr
		, unsigned long outputBufferSize, int &status);

typedef unsigned long (*NFANTOM100_createNXTIterator)(unsigned char searchBluetooth
		, unsigned long bluetoothSearchTimeout, int &status);
typedef void (*NFANTOM100_iNXTIterator_getName)(unsigned long NXTIterHandle, char resString[], int &status);
typedef unsigned long (*NFANTOM100_iNXTIterator_getNXT)(unsigned long nxtIterHandle, int &status);
typedef void (*NFANTOM100_destroyNXTIterator)(unsigned long nxtIteratorHandle, int &status);
typedef void (*NFANTOM100_iNXTIterator_advance)(unsigned long NXTIterHandle, int &status);
typedef void (*NFANTOM100_iNXT_findDeviceInFirmwareDownloadMode)(char resString[], int &status);
typedef void (*NFANTOM100_destroyNXT)(unsigned long nxtHandle, int &status);
