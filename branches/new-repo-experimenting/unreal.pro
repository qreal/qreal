QT           += sql svg

INCLUDEPATH	+= view umllib repo dialogs umllib/cppcode

RESOURCES += qtreal.qrc

# SQL model
HEADERS         = dialogs/qsqlconnectiondialog.h \
		    repo/realrepomodel.h \
		    repo/realrepoitem.h \
		    repo/realreponames.h
SOURCES         = dialogs/qsqlconnectiondialog.cpp \
		    repo/realrepomodel.cpp \
		    repo/realrepoitem.cpp \
		    repo/realreponames.cpp
FORMS           = dialogs/qsqlconnectiondialog.ui

# View
HEADERS			+= view/editorview.h \
			view/editorviewscene.h \
			view/editorviewmviface.h
SOURCES			+= view/editorview.cpp \
			view/editorviewscene.cpp \
			view/editorviewmviface.cpp

# UML library
HEADERS		+= umllib/uml_edgeelement.h \
			umllib/uml_element.h \
			umllib/uml_glamour_class.h \
			umllib/uml_guiobjectfactory.h \
			umllib/uml_nodeelement.h
SOURCES		+= umllib/uml_edgeelement.cpp \
			umllib/uml_element.cpp \
			umllib/uml_glamour_class.cpp \
			umllib/uml_guiobjectfactory.cpp \
			umllib/uml_nodeelement.cpp


HEADERS		+= mainwindow.h
SOURCES		+= mainwindow.cpp \
		    main.cpp


HEADERS +=  \ 
	umllib/cppcode/reqnFeaturedClass.h \ 
	umllib/cppcode/reqnConceptAlternativeClass.h \ 
	umllib/cppcode/reqnLeafClass.h \ 
	umllib/cppcode/reqnParentClass.h \ 
	umllib/cppcode/reqnParentMandatoryClass.h \ 
	umllib/cppcode/reqnParentOrClass.h \ 
	umllib/cppcode/reqnParentOptionalClass.h \ 
	umllib/cppcode/reqnParentAlternativeClass.h \ 
	umllib/cppcode/reqnParentOpAlternativeClass.h \ 
	umllib/cppcode/krneRelationshipClass.h \ 
	umllib/cppcode/krneDirRelationshipClass.h \ 
	umllib/cppcode/krneComLinkClass.h \ 
	umllib/cppcode/krneElementImportClass.h \ 
	umllib/cppcode/krnePackageImportClass.h \ 
	umllib/cppcode/krneGeneralizationClass.h \ 
	umllib/cppcode/reqeP2NClass.h

SOURCES +=  \ 
	umllib/cppcode/reqnFeaturedClass.cpp \ 
	umllib/cppcode/reqnConceptAlternativeClass.cpp \ 
	umllib/cppcode/reqnLeafClass.cpp \ 
	umllib/cppcode/reqnParentClass.cpp \ 
	umllib/cppcode/reqnParentMandatoryClass.cpp \ 
	umllib/cppcode/reqnParentOrClass.cpp \ 
	umllib/cppcode/reqnParentOptionalClass.cpp \ 
	umllib/cppcode/reqnParentAlternativeClass.cpp \ 
	umllib/cppcode/reqnParentOpAlternativeClass.cpp \ 
	umllib/cppcode/krneRelationshipClass.cpp \ 
	umllib/cppcode/krneDirRelationshipClass.cpp \ 
	umllib/cppcode/krneComLinkClass.cpp \ 
	umllib/cppcode/krneElementImportClass.cpp \ 
	umllib/cppcode/krnePackageImportClass.cpp \ 
	umllib/cppcode/krneGeneralizationClass.cpp \ 
	umllib/cppcode/reqeP2NClass.cpp
