TEMPLATE = subdirs

# editor plugins
SUBDIRS	= \
	dragonDiagram/dragonDiagram.pro \
	blockDiagram/blockDiagram.pro \
#	hascol/hascol.pro \
	metaEditor/metaEditor.pro \
	constraintsEditor/constraintsEditor.pro \
#	constraintsAllMetamodels/constraintsAllMetamodels.pro \
#	constraintsTestConstraintsBlockMetamodel/constraintsTestConstraintsBlockMetamodel.pro \
	TEMP/constraintsBlockDiagramMetamodelConstraints_1/constraintsBlockDiagramMetamodelConstraints_1.pro \
	requirements/requirements.pro \


# tool plugins
SUBDIRS += \
#	testInvocation/testInvocation.pro \
	exterminatus/exterminatus.pro \
	visualInterpreter/visualInterpreter.pro \
	refactoring/refactoring.pro \

# QReal:Robots
SUBDIRS += \
#	robots/robots.pro \


# QReal:BP
SUBDIRS += \
#	bpmn/bpmn.pro \
#	rulesChecker/rulesChecker.pro \
#	classDiagram/classDiagram.pro \


# QReal:Ubiq
SUBDIRS += \
#	ubiq/ubiq.pro \
