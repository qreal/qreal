#pragma once
#include <QtGui/QApplication>

const int kStatusNoError = 0;
bool const FantomAPILoaded = false;

extern "C"__declspec(dllimport) unsigned long nFANTOM100_createNXT(char resString[], int status, unsigned char checkFVersion);
extern "C"__declspec(dllimport) void nFANTOM100_iNXT_sendDirectCommand(unsigned long nxtHandle, bool requireResponse, const char *inputBufferPtr, int inputBufferSize, char *outputBufferPtr, int outputBufferSize, int &status);

extern "C"__declspec(dllimport) unsigned long nFANTOM100_createNXTIterator(unsigned char searchBluetooth, unsigned long bluetoothSearchTimeout, int &status);
extern "C"__declspec(dllimport) void nFANTOM100_iNXTIterator_getName(unsigned long NXTIterHandle, char resString[], int &status);
extern "C"__declspec(dllimport) unsigned long nFANTOM100_iNXTIterator_getNXT(unsigned long nxtIterHandle, int &status);
extern "C"__declspec(dllimport) void nFANTOM100_destroyNXTIterator(unsigned long nxtIteratorHandle, int &status);
extern "C"__declspec(dllimport) void nFANTOM100_iNXTIterator_advance(unsigned long NXTIterHandle, int &status);
extern "C"__declspec(dllimport) void nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(char resString[], int &status);
