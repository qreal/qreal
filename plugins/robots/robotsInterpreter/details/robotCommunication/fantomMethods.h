#pragma once

typedef unsigned long (*NFANTOM100_createNXT)(char resString[], int status, unsigned char checkFVersion);
typedef void (*NFANTOM100_iNXT_sendDirectCommand)(unsigned long nxtHandle, bool requireResponse, const char *inputBufferPtr, int inputBufferSize, char *outputBufferPtr, int outputBufferSize, int &status);

typedef unsigned long (*NFANTOM100_createNXTIterator)(unsigned char searchBluetooth, unsigned long bluetoothSearchTimeout, int &status);
typedef void (*NFANTOM100_iNXTIterator_getName)(unsigned long NXTIterHandle, char resString[], int &status);
typedef unsigned long (*NFANTOM100_iNXTIterator_getNXT)(unsigned long nxtIterHandle, int &status);
typedef void (*NFANTOM100_destroyNXTIterator)(unsigned long nxtIteratorHandle, int &status);
typedef void (*NFANTOM100_iNXTIterator_advance)(unsigned long NXTIterHandle, int &status);
typedef void (*NFANTOM100_iNXT_findDeviceInFirmwareDownloadMode)(char resString[], int &status);
