#ifndef REALREPOROLES_H
#define REALREPOROLES_H

namespace UML {
	enum ElementTypes{
		krnnElement=1,
		krnnDiagram=2,
		krnnNamedElement=3,
		krnnComment=4,
		krnnNamespace=5,
		krnnPackage=6,
		krnnPackageableElem=7,
		krnnType=8,
		krnnTypedElem=9,
		krnnRedefElement=10,
		krnnClassifier=11,
		krnnProperty=12,
		krnnOperation=13,
		krnnParameter=14,
		krnnFeature=15,
		krnnBehavioralFeature=16,
		krnnStructuralFeature=17,
		krneRelationship=18,
		krneDirRelationship=19,
		krneComLink=20,
		krneElementImport=21,
		krnePackageImport=22,
		krneGeneralization=23,
		cnClass=24,
		ceAggregation=25,
		ceComposition=26,
		ceDependency=27
	};
};

namespace Unreal {
	enum Roles {
		PositionRole = Qt::UserRole + 64,   // Position on a diagram
		ConfigurationRole,          // Configuration (e.g. for link)
		IdRole,
		TypeRole,
		UserRole = Qt::UserRole + 96        // First role available for other types
	};

	namespace krnnElement {
		enum Roles {
			nameRole = UserRole + 1,
			stateRole
		};
	};

	namespace krnnDiagram {
		enum Roles {
			nameRole = UserRole + 1,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnNamedElement {
		enum Roles {
			nameRole = UserRole + 1,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnComment {
		enum Roles {
			nameRole = UserRole + 1,
			bodyRole,
			stateRole
		};
	};

	namespace krnnNamespace {
		enum Roles {
			nameRole = UserRole + 1,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnPackage {
		enum Roles {
			nameRole = UserRole + 1,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnPackageableElem {
		enum Roles {
			nameRole = UserRole + 1,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnType {
		enum Roles {
			nameRole = UserRole + 1,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnTypedElem {
		enum Roles {
			nameRole = UserRole + 1,
			typeRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnRedefElement {
		enum Roles {
			nameRole = UserRole + 1,
			isLeafRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnClassifier {
		enum Roles {
			nameRole = UserRole + 1,
			isAbstractRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole,
			isLeafRole
		};
	};

	namespace krnnProperty {
		enum Roles {
			nameRole = UserRole + 1,
			isReadOnlyRole,
			isStaticRole,
			isLeafRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole,
			typeRole
		};
	};

	namespace krnnOperation {
		enum Roles {
			nameRole = UserRole + 1,
			isQueryRole,
			isOrderedRole,
			isUniqueRole,
			lowerRole,
			upperRole,
			typeRole,
			isAbstractRole,
			isStaticRole,
			isLeafRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnParameter {
		enum Roles {
			nameRole = UserRole + 1,
			directionRole,
			typeRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnFeature {
		enum Roles {
			nameRole = UserRole + 1,
			isStaticRole,
			isLeafRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnBehavioralFeature {
		enum Roles {
			nameRole = UserRole + 1,
			isAbstractRole,
			isStaticRole,
			isLeafRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole
		};
	};

	namespace krnnStructuralFeature {
		enum Roles {
			nameRole = UserRole + 1,
			isReadOnlyRole,
			isStaticRole,
			isLeafRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole,
			typeRole
		};
	};

	namespace krneRelationship {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole
		};
	};

	namespace krneDirRelationship {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole
		};
	};

	namespace krneComLink {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole
		};
	};

	namespace krneElementImport {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole,
			aliasRole,
			visibilityRole,
			stateRole
		};
	};

	namespace krnePackageImport {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole,
			visibilityRole,
			stateRole
		};
	};

	namespace krneGeneralization {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole,
			isSubstitutableRole,
			stateRole
		};
	};

	namespace cnClass {
		enum Roles {
			nameRole = UserRole + 1,
			valsRole,
			methodsRole,
			isAbstractRole,
			visibilityRole,
			qualifiedNameRole,
			stateRole,
			isLeafRole
		};
	};

	namespace ceAggregation {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole
		};
	};

	namespace ceComposition {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole
		};
	};

	namespace ceDependency {
		enum Roles {
			nameRole = UserRole + 1,
			fromRole,
			toRole,
			fromPortRole,
			toPortRole,
			stateRole
		};
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
