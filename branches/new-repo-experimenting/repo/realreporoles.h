#ifndef REALREPOROLES_H
#define REALREPOROLES_H

#include <Qt>

namespace Unreal {
    enum Roles {
	PositionRole = Qt::UserRole + 64,	// Position on a diagram
	ConfigurationRole,			// Configuration (e.g. for link)
	UserRole = Qt::UserRole + 96		// First role available for other types
    };

    enum ClassRoles {
	FieldsRole = UserRole + 1,
	MethodsRole
    };

    enum LinkRoles {
	FromRole = UserRole + 1,
	ToRole,
	FromPortRole,
	ToPortRole
    };
};

#endif
