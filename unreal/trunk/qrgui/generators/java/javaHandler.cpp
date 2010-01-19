#include "javaHandler.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/definitions.h"
#include "../../../qrrepo/repoApi.h"

using namespace qReal;
using namespace generators;

JavaHandler::JavaHandler(qrRepo::RepoApi const &api)
    : mApi(api)
{
}

QString JavaHandler::generateToJava(QString const &pathToDir)
{
	mErrorText = "";
	this->pathToDir = pathToDir;
    
	Id repoId = ROOT_ID;
    
	IdList rootDiagrams = mApi.children(repoId);
    
	foreach (Id const typeDiagram, rootDiagrams) {
		serializeChildren(typeDiagram);
	}
    
	qDebug() << "Done.";
	return mErrorText;
}

QString JavaHandler::serializeChildren(Id const &idParent)
{
	QString result = "";
	IdList childElems = mApi.children(idParent);
    
	foreach (Id const id, childElems) {
        result += serializeObject(id);
	}
    
	return result;
}

QString JavaHandler::serializeObject(Id const &id)
{
	QString result = "";
    
    QString const objectType = mApi.typeName(id);
    
	// class diagramm
    
    if (objectType == "Class_Diagram_Class") {
        
        //-----------
        QString const pathToFile = pathToDir + "/" + mApi.name(id) + ".java";
        
        QFile file(pathToFile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return "";
        
        QTextStream out(&file);
        //-----------
        
        QString visibility = getVisibility(id);
        QString multiplicity = getMultiplicity(id);
        QString isFinalField = hasModifier(id, "final");
        QString isAbstractField = hasModifier(id, "abstract");
        QString parents = getSuperclass(id);
        
        if (isAbstractField == "abstract " && isFinalField == "final ") {
            addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". \"abstract final\" declaration doesn't make sense");
        }
        
        out << isAbstractField + isFinalField + visibility + "class " + mApi.name(id) + parents +  " {" + "\n";
        out << serializeChildren(id);
        
        if (!mApi.links(id).isEmpty()) {
            // search for the Class-typed attrbutes
            IdList linksOut = mApi.outcomingLinks(id);
            foreach (Id const aLink, linksOut) {
                if (aLink.element() == "Class_Diagram_Directed_Relationship" || aLink.element() == "Class_Diagram_Relation") {
                    QString type = mApi.name(mApi.otherEntityFromLink(aLink, id)) + " ";
                    QString visibility = getVisibility(aLink);
                    QString isFinalField = hasModifier(aLink, "final");
                    QString isStaticField = hasModifier(aLink, "static");
                    QString isVolatileField = hasModifier(aLink, "volatile");
                    QString isTransientField = hasModifier(aLink, "transient");
                    
                    if (isVolatileField == "volatile " && isFinalField == "final ") {
                        addError("unable to serialize object " + objectType + " with id: " + aLink.toString() + ". \"final volatile\" declaration doesn't make sense");
                    }
                    
                    out << isFinalField + visibility + isStaticField + isVolatileField + isTransientField + type + mApi.name(aLink);
                    out << ";\n";
                }
            }
            
            //search for bidirectional assocciation
            IdList linksIn = mApi.incomingLinks(id);
            foreach (Id const aLink, linksIn) {
                if (aLink.element() == "Class_Diagram_Relation") {
                    QString type = mApi.name(mApi.otherEntityFromLink(aLink, id)) + " ";
                    QString visibility = getVisibility(aLink);
                    QString isFinalField = hasModifier(aLink, "final");
                    QString isStaticField = hasModifier(aLink, "static");
                    QString isVolatileField = hasModifier(aLink, "volatile");
                    QString isTransientField = hasModifier(aLink, "transient");
                    
                    if (isVolatileField == "volatile " && isFinalField == "final ") {
                        addError("unable to serialize object " + objectType + " with id: " + aLink.toString() + ". \"final volatile\" declaration doesn't make sense");
                    }
                    
                    out << isFinalField + visibility + isStaticField + isVolatileField + isTransientField + type + mApi.name(aLink);
                    out << ";\n";
                }
            }
        }
        
        if (multiplicity != "") {
            bool ok = false;
            int classMultiplicity = multiplicity.toInt(&ok);
            
            if (!ok || classMultiplicity <= 0) {
                if (classMultiplicity == 1) {
                    // класс singleton
                } else {
                    // класс в виде расширенного singleton'а (т.е. может быть ограниченное число экземпляров и за этим надо проследить)
                }
            } else {
                addError("object " + objectType + " with id  " + id.toString() + " has invalid multiplicity property: " + multiplicity);
            }
        }
        
        out << "}\n";
    } else if (objectType == "Class_Diagram_View") {
		//	    to do someting
    } else if (objectType == "Class_Diagram_Class_method") {
        IdList parents = mApi.parents(id);
        if (!parents.isEmpty()) {
            Id parentId = parents.at(0);
            QString const parentType = mApi.typeName(parentId);
            
            if (parentType == "Class_Diagram_Class") {
                QString visibility = getVisibility(id);
                QString type = getType(id);
                QString operationFactors = getOperationFactors(id);
                QString isFinalField = hasModifier(id, "final");
                QString isAbstractField = hasModifier(id, "abstract");
                QString isStaticField = hasModifier(id, "static");
                QString isSynchronizedField = hasModifier(id, "synchronized");
                QString isNativeField = hasModifier(id, "native");
                
                if (isAbstractField == "abstract ") {
                    if (isFinalField == "final ") {
                        addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". \"abstract final\" declaration doesn't make sense");
                    } else if (isStaticField == "static ") {
                        addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". \"abstract static\" declaration doesn't make sense");
                    }
                }
                result += isAbstractField + isFinalField + isStaticField + isSynchronizedField + isNativeField +
                          visibility + type  + mApi.name(id) + "(" + operationFactors + "){};" + "\n";
            } else {
                this->addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". Move it inside some Class");
            }
        }
    } else if (objectType == "Class_Diagram_Class_field") {
        IdList parents = mApi.parents(id);
        if (!parents.isEmpty()) {
            Id parentId = parents.at(0);
            QString const parentType = mApi.typeName(parentId);
            
            if (parentType == "Class_Diagram_Class") {
                QString visibility = getVisibility(id);
                QString type = getType(id);
                QString defaultValue = getDefaultValue(id);
                QString isFinalField = hasModifier(id, "final");
                QString isStaticField = hasModifier(id, "static");
                QString isVolatileField = hasModifier(id, "volatile");
                QString isTransientField = hasModifier(id, "transient");
                
                if (isVolatileField == "volatile " && isFinalField == "final ") {
                    addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". \"final volatile\" declaration doesn't make sense");
                }
                result += isFinalField + visibility + isStaticField + isVolatileField + isTransientField + type + mApi.name(id);
                if (defaultValue != "") {
                    result += " " + defaultValue;
                }
                result += ";\n";
            } else {
                addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". Move it inside some Class");
            }
        }
	}
    
    // activity diagram
    
    else if (objectType == "Activity_Diagram_Initial_Node") {
      //  IdList parents = mApi.parents(id);
        //if (mApi.typeName(parents.at(0)) != "Activity_Diagram_Structured_Activity_Node") {
            //-----------
            QString const pathToFile = pathToDir + "/" + "activityTest" + ".java";

            QFile file(pathToFile);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return "";

            QTextStream out(&file);
            //-----------

            out << "//the test of the activity diagram generator\n";
            out << "testProc() {\n";
            if (!mApi.links(id).isEmpty()) {
                if (!mApi.incomingLinks(id).isEmpty()) {
                    addError("object " + objectType + " with id  " + id.toString() + " can not have incoming links");
                }

                IdList linksOut = mApi.outcomingLinks(id);
                foreach (Id const aLink, linksOut) {
                    //imagine, that Initial Node has just one outcoming link
                    if (aLink.element() == "Activity_Diagram_Control_Flow") {
                        Id toConsider = mApi.otherEntityFromLink(aLink, id);
                        out << serializeObject(toConsider);
                    } else {
                        addError("object " + objectType + " with id  " + id.toString() + " can not have not Control Flow link as outcoming link.");
                    }
                }
            }
       // }
    } else if (objectType == "Activity_Diagram_Action") {
        result += mApi.name(id) + "();" + "\n";
        if (!mApi.links(id).isEmpty()) {
            IdList linksOut = mApi.outcomingLinks(id);
            foreach (Id const aLink, linksOut) {
                if (aLink.element() == "Activity_Diagram_Control_Flow") {
                    Id toConsider = mApi.otherEntityFromLink(aLink, id);
                    result += serializeObject(toConsider);
                }
            }
        }
    } else if (objectType == "Activity_Diagram_Activity_Final_Node") {
        result += "} //testProc\n";
    } else if (objectType == "Activity_Diagram_Decision_Node") {
        result += "if (";
        if (!mApi.links(id).isEmpty()) {
            IdList linksOut = mApi.outcomingLinks(id);
            if (!linksOut.isEmpty()) {
                foreach (Id const aLink, linksOut) {
                    if (aLink.element() == "Activity_Diagram_Control_Flow") {
                        Id toConsider = mApi.otherEntityFromLink(aLink, id);
                        result += serializeObject(toConsider);
                    }
                }
            } else {
                addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". Is must have at least one outgoing edge.");
            }
        }
    }
    
	return result;
}

QString JavaHandler::getVisibility(Id const &id)
{
	QString result = "";
    
	QString const objectType = mApi.typeName(id);
    
	if (mApi.hasProperty(id, "visibility")) {
		QString visibility = mApi.stringProperty(id, "visibility");
        
		if (visibility == "+")
			visibility = "public";
		else if (visibility == "-")
			visibility = "private";
		else if (visibility == "#")
			visibility = "protected";
		else if (visibility == "~")
			visibility = "";
        
		if (isVisibilitySuitable(visibility)) {
			result = visibility;
			if (result != "")
				result += " ";
		} else {
			addError("object " + objectType + " with id  " + id.toString() + " has invalid visibility property: " + visibility);
		}
	}
    
	return result;
}

QString JavaHandler::getMultiplicity(Id const &id)
{
	QString result = "";
    
	QString const objectType = mApi.typeName(id);
    
	if (mApi.hasProperty(id, "multiplicity")) {
		QString multiplicity = mApi.stringProperty(id, "multiplicity");
	}
    
	return result;
}

QString JavaHandler::getType(Id const &id)
{
	QString result = "";
    
	QString const objectType = mApi.typeName(id);
    
	if (mApi.hasProperty(id, "type")) {
		QString type = mApi.stringProperty(id, "type");
        
        if (isTypeSuitable(type) || (objectType == "Class_Diagram_Class_method" && type == "void")) {
			result = type;
			if (result != "")
				result += " ";
		} else {
			addError("object " + objectType + " with id " + id.toString() + " has invalid type: " + type);
		}
	}
    
	return result;
}

QString JavaHandler::getSuperclass(Id const &id)
{
	QString result = "";
	bool hasParentClass = false;
    
	QString const objectType = mApi.typeName(id);
    
	if (!mApi.links(id).isEmpty()) {
		IdList links = mApi.outcomingLinks(id);
        
		foreach (Id const aLink, links) {
            if (aLink.element() == "Class_Diagram_Generalization") {
				if (hasParentClass == false) {
					hasParentClass = true;
					if (id == mApi.otherEntityFromLink(aLink, id)) {
						addError("object " + objectType + " with id " + id.toString() + " can not be his own superclass");
					} else {
						QString parentClass = mApi.name(mApi.otherEntityFromLink(aLink, id));
						result += " extends " + parentClass;
					}
				} else {
					addError("object " + objectType + " with id " + id.toString() + " has too much superclasses");
				}
			}
		}
        
	}
    
	return result;
}

QString JavaHandler::hasModifier(Id const &id, QString const &modifier)
{
	QString result = "";
    
	QString const objectType = mApi.typeName(id);
    
	QString isModifier = "";
    
	if (modifier == "final") {
		isModifier = "isLeaf";
	} else {
		isModifier = "is" + modifier.left(1).toUpper() + modifier.mid(1, modifier.length());
	}
    
	if (mApi.hasProperty(id, isModifier)) {
		QString hasModifier = mApi.stringProperty(id, isModifier);
        
		if (hasModifier == "true") {
			result = modifier + " ";
		}else if (hasModifier != "false" && hasModifier != "") {
			addError("object " + objectType + " with id " + id.toString() + " has invalid " + isModifier + " value: " + hasModifier);
		}
	}
    
	return result;
}

QString JavaHandler::getOperationFactors(Id const &id)
{
	QString result = "";
    
	if (mApi.hasProperty(id, "type")) {
		QString operationFactors = mApi.stringProperty(id, "operationFactors");
        
		//	to check for the correct data
		//	if (isTypeSuitable(type) || (objectType == "cnClassMethod" && type == "void")) {
		result = operationFactors;
		//	} else {
		//		addError("object " + objectType + " with id " + id.toString() + " has invalid type: " + type);
		//    	}
	}
    
	return result;
}

QString JavaHandler::getDefaultValue(Id const &id)
{
	QString result = "";
    
	QString const objectType = mApi.typeName(id);
    
	if (mApi.hasProperty(id, "defaultValue")) {
		QString defaultValue = mApi.stringProperty(id, "defaultValue");
        
		//	if (isTypeSuitable(defaultValue)) {
		//	to check for the corract data
		result = defaultValue;
		//	} else {
		//		addError("object " + objectType + " with id " + id.toString() + " has invalid default value: " + defaultValue);
		//    	}
	}

	return result;
}

bool JavaHandler::isTypeSuitable(QString const &type) const
{
	//  todo: check class-type for correctness
	return type == "int" || type == "float" || type == "double" || type == "boolean"
			|| type == "char" || type == "byte" || type == "long" || type == "short";
}

bool JavaHandler::isVisibilitySuitable(QString const &visibility) const
{
	return visibility == "public" || visibility == "private" || visibility == "protected" || visibility == "";
}

void JavaHandler::addError(QString const &errorText)
{
	mErrorText += errorText + "\n";
}
