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
    IdList childElems; //TODO: initialize as NULL

    if (objectType(idParent) == "ActivityDiagram_ActivityDiagramNode") {
        IdList children = mApi.children(idParent);

        foreach (Id aChild, children) {
            //TODO: add those elements that can begin the execution (structured and some else)
            if (aChild.element() == "ActivityDiagram_InitialNode") {
                childElems.append(aChild);
            }
        }
    } else {
        childElems = mApi.children(idParent);
    }


    foreach (Id const id, childElems) {
        result += serializeObject(id);
    }

    return result;
}

QString JavaHandler::serializeObject(Id const &id)
{
    QString result = "";

    // class diagram

    if ((objectType(id) == "ClassDiagram_Class")|(objectType(id) == "ClassDiagram_Interface")) {

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
            addError("unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". \"abstract final\" declaration doesn't make sense");
        }

        out << isAbstractField + isFinalField + visibility + "class " + mApi.name(id) + parents +  " {" + "\n";
        out << serializeChildren(id);

        if (!mApi.links(id).isEmpty()) {
            // search for the Class-typed attrbutes
            IdList outgoingLinks = mApi.outgoingLinks(id);
            foreach (Id const aLink, outgoingLinks) {
                if (aLink.element() == "ClassDiagram_DirectedAssociation" || aLink.element() == "ClassDiagram_Association") {
                    QString type = mApi.name(mApi.otherEntityFromLink(aLink, id)) + " ";
                    QString visibility = getVisibility(aLink);
                    QString isFinalField = hasModifier(aLink, "final");
                    QString isStaticField = hasModifier(aLink, "static");
                    QString isVolatileField = hasModifier(aLink, "volatile");
                    QString isTransientField = hasModifier(aLink, "transient");

                    if (isVolatileField == "volatile " && isFinalField == "final ") {
                        addError("unable to serialize object " + objectType(id) + " with id: " + aLink.toString() + ". \"final volatile\" declaration doesn't make sense");
                    }

                    out << isFinalField + visibility + isStaticField + isVolatileField + isTransientField + type + mApi.name(aLink);
                    out << ";\n";
                }
            }

            //search for bidirectional assocciation
            IdList linksIn = mApi.incomingLinks(id);
            foreach (Id const aLink, linksIn) {
                if (aLink.element() == "ClassDiagram_Association") {
                    QString type = mApi.name(mApi.otherEntityFromLink(aLink, id)) + " ";
                    QString visibility = getVisibility(aLink);
                    QString isFinalField = hasModifier(aLink, "final");
                    QString isStaticField = hasModifier(aLink, "static");
                    QString isVolatileField = hasModifier(aLink, "volatile");
                    QString isTransientField = hasModifier(aLink, "transient");

                    if (isVolatileField == "volatile " && isFinalField == "final ") {
                        addError("unable to serialize object " + objectType(id) + " with id: " + aLink.toString() + ". \"final volatile\" declaration doesn't make sense");
                    }

                    out << isFinalField + visibility + isStaticField + isVolatileField + isTransientField + type + mApi.name(aLink);
                    out << ";\n";
                }
            }
        }

        out << "} //end of class\n";
    } else if (objectType(id) == "ClassDiagram_View") {
        //	    to do someting
    } else if (objectType(id) == "ClassDiagram_ClassMethod") {
        IdList parents = mApi.parents(id);
        if (!parents.isEmpty()) {
            Id parentId = parents.at(0);
            QString const parentType = objectType(parentId);

            if (parentType == "ClassDiagram_Class") {
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
                        addError("unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". \"abstract final\" declaration doesn't make sense");
                    } else if (isStaticField == "static ") {
                        addError("unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". \"abstract static\" declaration doesn't make sense");
                    }
                }

                QString methodBody = getMethodCode(id);

                result += visibility + isAbstractField + isStaticField + isFinalField + isSynchronizedField + isNativeField +
                          type  + mApi.name(id) + "(" + operationFactors + ")" + methodBody + "\n";
            } else {
                this->addError("unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". Move it inside some Class");
            }
        }
    } else if (objectType(id) == "ClassDiagram_ClassField") {
        IdList parents = mApi.parents(id);
        if (!parents.isEmpty()) {
            Id parentId = parents.at(0);
            QString const parentType = objectType(parentId);

            if (parentType == "ClassDiagram_Class") {
                QString visibility = getVisibility(id);
                QString type = getType(id);
                QString defaultValue = getDefaultValue(id);
                QString isFinalField = hasModifier(id, "final");
                QString isStaticField = hasModifier(id, "static");
                QString isVolatileField = hasModifier(id, "volatile");
                QString isTransientField = hasModifier(id, "transient");

                if (isVolatileField == "volatile " && isFinalField == "final ") {
                    addError("unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". \"final volatile\" declaration doesn't make sense");
                }
                result += visibility + isStaticField + isFinalField + isVolatileField + isTransientField + type + mApi.name(id);
                if (defaultValue != "") {
                    result += " " + defaultValue;
                }
                result += ";\n";
            } else {
                addError("unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". Move it inside some Class");
            }
        }
    }

    // activity diagram

    else if (objectType(id) == "ActivityDiagram_InitialNode") {
        //[Superstructure 09-02-02][1] An initial node has no incoming edges.
        if (!mApi.links(id).isEmpty()) {
            if (!mApi.incomingLinks(id).isEmpty()) {
                addError("Object " + objectType(id) + " with id  " + id.toString() + " can not have incoming links.");
            }

            //[Superstructure 09-02-02][2] Only control edges can have initial nodes as source.
            IdList outgoingLinks = mApi.outgoingLinks(id);
            foreach (Id const aLink, outgoingLinks) {
//                //imagine, that Initial Node has just one outcoming link
                if (aLink.element() == "ActivityDiagram_ControlFlow") {
                    result += "{\n";
                    Id nextElement = mApi.otherEntityFromLink(aLink, id);
                    result += serializeObject(nextElement);
                } else {
                    addError("Object " + objectType(id) + " with id  " + id.toString() + ". Only Control Edges can have Initial Nodes as source.");
                }
            }
        }
    } else if (objectType(id) == "ActivityDiagram_Action") {
        result += mApi.name(id) + "\n";

        //serialization of it's children
        IdList outgoingLinks = mApi.outgoingLinks(id);
        foreach (Id const aLink, outgoingLinks) {
            Id nextElement = mApi.otherEntityFromLink(aLink, id);
            if (nextElement.element() != "ActivityDiagram_MergeNode" && nextElement.element() != "ActivityDiagram_JoinNode") {
                result += serializeObject(nextElement);
            }
        }
    } else if (objectType(id) == "ActivityDiagram_ActivityFinalNode") {
        addError("If you want \"return;\" you should write it in the Action before this Final Node.");
        result += "} //final node\n";

        //[Superstructure 09-02-02][1] A final node has no outgoing edges.
        IdList outgoingLinks = mApi.outgoingLinks(id);
        if (!outgoingLinks.isEmpty()) {
            addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". A final node has no outgoing edges.");
        }
    } else if (objectType(id) == "ActivityDiagram_DecisionNode") {
        int isControlFlow = -1; //for checking in and outgoing links connected to DecisionNode
        if (!mApi.links(id).isEmpty()) {
            IdList outgoingLinks = mApi.outgoingLinks(id);
            if (!outgoingLinks.isEmpty()) {
                foreach (Id const aLink, outgoingLinks) {
                    // Very arched way to check:
                    // [Superstructure 09-02-02.pdf][2] The edges coming into and out of a DecisionNode, other than the decisionInputFlow (if any), must be either all ObjectFlows or all ControlFlows.
                    if (aLink.element() == "ActivityDiagram_ControlFlow") {
                        if (isControlFlow == 0) {
                            addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". The edges coming out must be either all Object Flows or all Control Flows.");
                        }
                        isControlFlow = 1;
                    } else if (aLink.element() == "ActivityDiagram_ObjectFlow") {
                        if (isControlFlow == 1) {
                            addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". The edges coming out must be either all Object Flows or all Control Flows.");
                        }
                        isControlFlow = 0;
                    } //TODO: To check if the link is not Control Flow or Object Flow and fail the generation if it is.
                }

                //"if" or "while-do"?
                IdList incomingLinks = mApi.incomingLinks(id);
                if (incomingLinks.length() == 1) {
                    result += ifStatement(id);
                } else if (incomingLinks.length() == 2) {
                    result += whileDoLoop(id);
                } else { //[Superstructure 09-02-02][1] A decision node has one or two incoming edges.
                    addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". A decision Node has one or two incoming edges.");
                }

                result += "//end of the decision node\n";
            } else {
                addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". Is must have at least one outgoing edge.");
            }
        } else {
            addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". Is must have at least one outgoing edge.");
        }
    } else if (objectType(id) == "ActivityDiagram_MergeNode") {
        result += "//merge node \n";

        IdList outgoingLinks = mApi.outgoingLinks(id);
        foreach (Id const aLink, outgoingLinks) {
            Id nextElement = mApi.otherEntityFromLink(aLink, id);
            result += serializeObject(nextElement);
        }
    }

    return result;
}

QString JavaHandler::ifStatement(Id const &id)
{
    QString result = "";

    int existElse = 0; //for checking that there is no 2 outgoing links with "else" as a guard

    //move "else" link to the end of the list
    IdList outgoingLinks = mApi.outgoingLinks(id);
    foreach (Id aLink, outgoingLinks) {
        //if this link represent "else" case than change it with the last link in the serialization sequence
        if (getFlowGuard(aLink) == "else" && existElse == 0) {
            existElse = 1;
            addError("ATTENTION!!! Else link changes with " + getFlowGuard(outgoingLinks.at(outgoingLinks.length()-1)));
            outgoingLinks.swap(outgoingLinks.indexOf(aLink), outgoingLinks.length()-1);
        } else {
            addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". There are two objects with \"else\" as guard.");
        }
    }

    foreach (Id aLink, outgoingLinks) {
        Id caseBody = mApi.otherEntityFromLink(aLink, id);
        QString guard = getFlowGuard(aLink);
        if (guard != "else") {
            result += "if (" + guard + ") ";
        }

        //if it is a straight link from Decision Node to Merge Node
        if (caseBody.element() != "ActivityDiagram_MergeNode") {
            result += "{\n"+ serializeObject(caseBody) + "}";
        } else {
            result += "{\n}";
        }

        if (aLink == outgoingLinks.at(outgoingLinks.length()-1)) { // if it is the last link then we should finish "if"-statement
            result += "/*last link in decision node*/  \n";
        } else { //if it is not the last link, connected to the DecisionNode
            result += " else ";
        }
    }

    IdList mergeNodes;
    IdList incomingConnections = mApi.incomingConnections(id);
    foreach (Id aConnection, incomingConnections) {
        if (aConnection.element() == "ActivityDiagram_MergeNode") {
            mergeNodes.append(aConnection);
        }
    }

    if (mergeNodes.length() == 0) {
        //TODO: fill this
    } else if (mergeNodes.length() == 1) {
        result += serializeObject(mergeNodes.at(0));
    } else {
        addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". Is has to many connected Merge Nodes.");
    }

    return result;
}

QString JavaHandler::whileDoLoop(Id const &id)
{
    QString result = "";

    return result;
}

QString JavaHandler::doWhileLoop(Id const &id)
{
    QString result = "";

    return result;
}

QString JavaHandler::getVisibility(Id const &id)
{
    QString result = "";

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
            addError("Object " + objectType(id) + " with id  " + id.toString() + " has invalid visibility property: " + visibility);
        }
    }

    return result;
}

QString JavaHandler::getFlowGuard(Id const &id)
{
    QString result = "";

    if (mApi.hasProperty(id, "guard")) {
        QString guard = mApi.stringProperty(id, "guard");
        result = guard.simplified(); //delete whitespaces from the start and the end and internal whitespaces replace with a single space

        //TODO: Delete all white spaces, tabs, etc. in guard. Just find the function =)
        if (guard == "") {
            addError("Object " + objectType(id) + " with id  " + id.toString() + " has empty guard property.");
        }
    }

    return result;
}

QString JavaHandler::getMultiplicity(Id const &id)
{
    QString result = "";

    if (mApi.hasProperty(id, "multiplicity")) {
        QString multiplicity = mApi.stringProperty(id, "multiplicity");
        result = multiplicity;
    }

    return result;
}

QString JavaHandler::getType(Id const &id)
{
    QString result = "";

    if (mApi.hasProperty(id, "type")) {
        QString type = mApi.stringProperty(id, "type");

        if (isTypeSuitable(type) || (objectType(id) == "ClassDiagram_ClassMethod" && type == "void")) {
            result = type;
            if (result != "")
                result += " ";
        } else {
            addError("Object " + objectType(id) + " with id " + id.toString() + " has invalid type: " + type);
        }
    }

    return result;
}

QString JavaHandler::getSuperclass(Id const &id)
{
    QString result = "";
    bool hasParentClass = false;

    if (!mApi.links(id).isEmpty()) {
        IdList links = mApi.outgoingLinks(id);

        foreach (Id const aLink, links) {
            if (aLink.element() == "ClassDiagram_Generalization") {
                if (hasParentClass == false) {
                    hasParentClass = true;
                    if (id == mApi.otherEntityFromLink(aLink, id)) {
                        addError("Object " + objectType(id) + " with id " + id.toString() + " can not be his own superclass");
                    } else {
                        QString parentClass = mApi.name(mApi.otherEntityFromLink(aLink, id));
                        result += " extends " + parentClass;
                    }
                } else {
                    addError("Object " + objectType(id) + " with id " + id.toString() + " has too many superclasses");
                }
            }
        }

    }

    return result;
}

QString JavaHandler::getMethodCode(Id const &id)
{
    QString result = "{\n}";

    if (!mApi.outgoingConnections(id).isEmpty()) {
        IdList outgoingConnections = mApi.outgoingConnections(id);
        Id realizationDiagram; //TODO: maybe it should be initialized with null?..

        int realizationsCount = 0;
        foreach (Id aConnection, outgoingConnections) {
            if (aConnection.element() == "ActivityDiagram_ActivityDiagramNode") {
                realizationDiagram = aConnection;
                realizationsCount++;
            }
        }

        if (realizationsCount == 1) { //if everything is ok
            result = serializeChildren(realizationDiagram);
        } else if (realizationsCount == 0) { //if there is no realization
            addError("Method " + objectType(id) + " with id " + id.toString() + " will be empty.");
        } else { //if there is more than one ActivityDiagram connected
            addError("Object " + objectType(id) + " with id " + id.toString() + " has too many realizations");
        }

    } else {
        addError("Method " + objectType(id) + " with id " + id.toString() + " will be empty.");
    }

    return result;
}

QString JavaHandler::hasModifier(Id const &id, QString const &modifier)
{
    QString result = "";

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
            addError("Object " + objectType(id) + " with id " + id.toString() + " has invalid " + isModifier + " value: " + hasModifier);
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
        //	if (isTypeSuitable(type) || (objectType(id) == "cnClassMethod" && type == "void")) {
        result = operationFactors;
        //	} else {
        //		addError("Object " + objectType(id) + " with id " + id.toString() + " has invalid type: " + type);
        //    	}
    }

    return result;
}

QString JavaHandler::getDefaultValue(Id const &id)
{
    QString result = "";

    if (mApi.hasProperty(id, "defaultValue")) {
        QString defaultValue = mApi.stringProperty(id, "defaultValue");

        //	if (isTypeSuitable(defaultValue)) {
        //	to check for the corract data
        result = defaultValue;
        //	} else {
        //		addError("Object " + objectType(id) + " with id " + id.toString() + " has invalid default value: " + defaultValue);
        //    	}
    }

    return result;
}

QString JavaHandler::objectType(Id const &id)
{
    return mApi.typeName(id);
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

void JavaHandler::makeTree(Id const &activityDiagramId)
{
}
