#ifndef _ENTITY_TYPES_H_
#define _ENTITY_TYPES_H_

#define R_BAD_ID -1

enum entityType {
	allTypes, typeDeleted, typeUnknown, typeComment,
	typeModel, typeActor, typeUseCase,
	typeDiagram_UseCase, typeDiagram_Requirements, typeDiagram_StateMachine,
	typeDiagram_Activity, typeDiagram_Timing, typeDiagram_Communication,
	typeDiagram_Sequence, typeDiagram_InteractionOverview, typeDiagram_Package,
	typeDiagram_Deployment, typeDiagram_CompositeStructure, typeDiagram_Object,
	typeDiagram_Component, typeDiagram_Class 
};

#endif
