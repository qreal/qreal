#pragma once

#include <QtCore/QObject>

#include <qrutils/utilsDeclSpec.h>

namespace qReal {

/// Casts plugins to interfaces.
template <class InterfaceType>
class QRUTILS_EXPORT InterfaceWrapper
{
public:
	/// Casts one object to interface.
	static InterfaceType *wrappedInterface(QObject *interfaceToWrap)
	{
		InterfaceType *castedInterface = qobject_cast<InterfaceType *>(interfaceToWrap);

		if (castedInterface) {
			return castedInterface;
		} else {
			return nullptr;
		}
	}

	/// Casts list of objects to list of interfaces.
	static QList<InterfaceType *> listOfInterfaces(QList<QObject *> interfacesToWrap)
	{
		QList<InterfaceType *> interfacesList;

		for (QObject const *currentInterface : interfacesToWrap) {
			InterfaceType *castedInterface = qobject_cast<InterfaceType *>(currentInterface);
			if (castedInterface) {
				interfacesList.append(castedInterface);
			}
		}

		return interfacesList;
	}
};
}
