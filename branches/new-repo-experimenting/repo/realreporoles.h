#ifndef REALREPOROLES_H
#define REALREPOROLES_H

#include "reqnFeaturedClass.h"
#include "reqnConceptAlternativeClass.h"
#include "reqnLeafClass.h"
#include "reqnParentClass.h"
#include "reqnParentMandatoryClass.h"
#include "reqnParentOrClass.h"
#include "reqnParentOptionalClass.h"
#include "reqnParentAlternativeClass.h"
#include "reqnParentOpAlternativeClass.h"
#include "krneRelationshipClass.h"
#include "krneDirRelationshipClass.h"
#include "krneComLinkClass.h"
#include "krneElementImportClass.h"
#include "krnePackageImportClass.h"
#include "krneGeneralizationClass.h"
#include "reqeP2NClass.h"


namespace UML {
	enum ElementTypes{
		krnnElement=12,
		krnnDiagram=13,
		krnnNamedElement=14,
		krnnComment=15,
		krnnNamespace=16,
		krnnPackage=17,
		krnnPackageableElem=18,
		krnnType=19,
		krnnTypedElem=20,
		krnnRedefElement=21,
		krnnClassifier=22,
		krnnProperty=23,
		krnnOperation=24,
		krnnParameter=25,
		krnnFeature=26,
		krnnBehavioralFeature=27,
		krnnStructuralFeature=28,
		krneRelationship=29,
		krneDirRelationship=30,
		krneComLink=31,
		krneElementImport=32,
		krnePackageImport=33,
		krneGeneralization=34,
		reqnFeatured=35,
		reqnConceptAlternative=36,
		reqnLeaf=37,
		reqnParent=38,
		reqnParentMandatory=39,
		reqnParentOr=40,
		reqnParentOptional=41,
		reqnParentAlternative=42,
		reqnParentOpAlternative=43,
		reqeP2N=44
	};
};

namespace Unreal{
	enum Roles {
		PositionRole = Qt::UserRole + 64,   // Position on a diagram
		ConfigurationRole,          // Configuration (e.g. for link)
		IdRole,
		TypeRole,
		UserRole = Qt::UserRole + 96        // First role available for other types
	};

	namespace krnnElement {
		enum Roles {
			stateRole = UserRole + 1
		};
	};

	namespace krnnDiagram {
		enum Roles {
			stateRole = UserRole + 1
		};
	};

	namespace krnnNamedElement {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole
		};
	};

	namespace krnnComment {
		enum Roles {
			stateRole = UserRole + 1,
			bodyRole
		};
	};

	namespace krnnNamespace {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole
		};
	};

	namespace krnnPackage {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole
		};
	};

	namespace krnnPackageableElem {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole
		};
	};

	namespace krnnType {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole
		};
	};

	namespace krnnTypedElem {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole,
			typeRole
		};
	};

	namespace krnnRedefElement {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole,
			isLeafRole
		};
	};

	namespace krnnClassifier {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole,
			isAbstractRole
		};
	};

	namespace krnnProperty {
		enum Roles {
		};
	};

	namespace krnnOperation {
		enum Roles {
			isQueryRole = UserRole + 1,
			isOrderedRole,
			isUniqueRole,
			lowerRole,
			upperRole,
			typeRole
		};
	};

	namespace krnnParameter {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole,
			typeRole,
			directionRole
		};
	};

	namespace krnnFeature {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole,
			isLeafRole,
			isStaticRole
		};
	};

	namespace krnnBehavioralFeature {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole,
			isLeafRole,
			isStaticRole,
			isAbstractRole
		};
	};

	namespace krnnStructuralFeature {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			visibilityRole,
			qualifiedNameRole,
			isLeafRole,
			isStaticRole,
			isReadOnlyRole
		};
	};

	namespace krneRelationship {
		enum Roles {
			fromRole = UserRole + 1,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole
		};
	};

	namespace krneDirRelationship {
		enum Roles {
			fromRole = UserRole + 1,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole
		};
	};

	namespace krneComLink {
		enum Roles {
			fromRole = UserRole + 1,
			toRole,
			fromPortRole,
			toPortRole
		};
	};

	namespace krneElementImport {
		enum Roles {
			fromRole = UserRole + 1,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole,
			aliasRole,
			visibilityRole
		};
	};

	namespace krnePackageImport {
		enum Roles {
			fromRole = UserRole + 1,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole,
			visibilityRole
		};
	};

	namespace krneGeneralization {
		enum Roles {
			fromRole = UserRole + 1,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole,
			isSubstitutableRole
		};
	};

	namespace reqnFeatured {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqnConceptAlternative {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqnLeaf {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqnParent {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqnParentMandatory {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqnParentOr {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqnParentOptional {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqnParentAlternative {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqnParentOpAlternative {
		enum Roles {
			stateRole = UserRole + 1,
			nameRole,
			descriptionRole,
			priorityRole,
			sourceRole,
			statusRole
		};
	};

	namespace reqeP2N {
		enum Roles {
			fromRole = UserRole + 1,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole
		};
	};

};

namespace SQLFields {
	static int ElementOffset = 11;

	static const char * Diagram[] = {
		"x", "y", "cfg"
	};

	static const char * Package[] = {
		"foo!"	};

	static const char * Class[] = {
		"properties", "methods"
	};

	static const char * Link[] = {
		"from", "to"
	};

	static const char ** ColumnNames[] = {
		Diagram,
		Package,
		Class,
		Link
	};
};

namespace Unreal {
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
