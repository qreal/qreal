#pragma once

#include <QtCore/QLibrary>

namespace robotsInterpreter {
namespace robotCommunication {

/// Proxy for Fantom driver DLL for Windows. Responsible for loading DLL and
/// dispatching all calls to corresponding driver functions.
class Fantom
{
public:
	/// Constructor. Tries to load a DLL.
	Fantom();

	/// Returns true if fantom driver is available, false otherwise.
	bool isAvailable();

	// Fantom library methods

	unsigned long nFANTOM100_createNXT(char resString[], int status, unsigned char checkFVersion);
	void nFANTOM100_iNXT_sendDirectCommand(unsigned long nxtHandle, bool requireResponse, const char *inputBufferPtr, int inputBufferSize, char *outputBufferPtr, int outputBufferSize, int &status);

	unsigned long nFANTOM100_createNXTIterator(unsigned char searchBluetooth, unsigned long bluetoothSearchTimeout, int &status);
	void nFANTOM100_iNXTIterator_getName(unsigned long NXTIterHandle, char resString[], int &status);
	unsigned long nFANTOM100_iNXTIterator_getNXT(unsigned long nxtIterHandle, int &status);
	void nFANTOM100_destroyNXTIterator(unsigned long nxtIteratorHandle, int &status);
	void nFANTOM100_iNXTIterator_advance(unsigned long NXTIterHandle, int &status);
	void nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(char resString[], int &status);

private:
	/// Instance of the Fantom library, if it is present.
	QLibrary mFantomLibrary;
};

}
}
