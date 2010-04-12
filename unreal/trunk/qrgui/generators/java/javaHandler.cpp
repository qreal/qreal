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

IdList JavaHandler::getActivityDiagramChildren(Id const &idParent)
{
    IdList childElements, allElements;
    childElements.clear();
    allElements = mApi.children(idParent);

    foreach (Id aChild, allElements) {
        //Search for the nodes that can start control flows
        //We are hoping that there is just one such node on the Activity Diagram
        if (aChild.element() == "ActivityDiagram_InitialNode" || aChild.element() == "ActivityDiagram_ActivityParameterNode"
            || aChild.element() == "ActivityDiagram_AcceptEventAction") {
            childElements.append(aChild);
        }
    }

    if (childElements.length() == 0) {
        addError("Unable to serialize object " + objectType(idParent) + " with id: " + idParent.toString() + ". There is no start nodes (Initial Node, Activity Parameter Node, Accept Event Action).");
    } else if (childElements.length() > 1) { //TODO: to find out if it is possible. If so, write the appropriate serialization
        addError("AAA!!! There is too many start nodes. I don't know which to choose. Diagram is " + mApi.name(idParent));
    } else {
        Id startNode = childElements.at(0);
        childElements.append(findIntermediateNodes(startNode));
    }

    return childElements;
}

IdList JavaHandler::findIntermediateNodes(Id const &startNode)
{
    IdList children;

    IdList outgoingLinks = mApi.outgoingLinks(startNode);

    foreach (Id aLink, outgoingLinks) {
        Id nextElement = mApi.otherEntityFromLink(aLink, startNode);
        if (nextElement.element() == "ActivityDiagram_DecisionNode") {
            children.append(nextElement);

            int isControlFlow = -1; //for checking in and outgoing links connected to DecisionNode
            if (!mApi.links(nextElement).isEmpty()) {
                IdList outgoingLinks = mApi.outgoingLinks(nextElement);
                if (!outgoingLinks.isEmpty()) {
                    // Very arched way to check:
                    // [Superstructure 09-02-02.pdf][2] The edges coming into and out of a DecisionNode, other than the decisionInputFlow (if any), must be either all ObjectFlows or all ControlFlows.
                    foreach (Id const aLink, outgoingLinks) {
                        if (aLink.element() == "ActivityDiagram_ControlFlow") {
                            if (isControlFlow == 0) {
                                addError("Unable to serialize object " + objectType(nextElement) + " with id: " + nextElement.toString() + ". The edges coming out must be either all Object Flows or all Control Flows.");
                            }
                            isControlFlow = 1;
                        } else if (aLink.element() == "ActivityDiagram_ObjectFlow") {
                            if (isControlFlow == 1) {
                                addError("Unable to serialize object " + objectType(nextElement) + " with id: " + nextElement.toString() + ". The edges coming out must be either all Object Flows or all Control Flows.");
                            }
                            isControlFlow = 0;
                        } //TODO: To check if the link is not Control Flow or Object Flow and fail the generation if it is.
                    }

                    //"if" or "while"?
                    IdList incomingLinks = mApi.incomingLinks(nextElement);
                    if (incomingLinks.length() == 1) { //"if"
                        Id mergeNode = findMergeNode(nextElement);

                        if (mergeNode != Id()) {
                            children.append(findIntermediateNodes(mergeNode));
                        }
                    } else if (incomingLinks.length() == 2) { //"while"
                        Id nonBodyLink = findNonBodyLink(nextElement);

                        if (nonBodyLink != Id()) {
                            Id firstNonBodyElement = mApi.otherEntityFromLink(nonBodyLink, nextElement);
                            children.append(firstNonBodyElement);
                            children.append(findIntermediateNodes(firstNonBodyElement));
                        }
                    } else { //[Superstructure 09-02-02][1] A decision node has one or two incoming edges.
                        addError("Unable to serialize object " + objectType(nextElement) + " with id: " + nextElement.toString() + ". A decision Node has one or two incoming edges.");
                    }

                } else {
                    addError("Unable to serialize object " + objectType(nextElement) + " with id: " + nextElement.toString() + ". Is must have at least one outgoing edge.");
                }
            } else {
                addError("Unable to serialize object " + objectType(nextElement) + " with id: " + nextElement.toString() + ". Is must have at least one outgoing edge.");
            }
        } else if (nextElement.element() == "ActivityDiagram_MergeNode") {
            //TODO: fill this
        } //TODO: add other "importaint" nodes
        else {
            children.append(nextElement);
            children.append(findIntermediateNodes(nextElement));;
        }
    }

    return children;
}

Id JavaHandler::findMergeNode(Id const &idDecisionNode)
{
    Id mergeNode;

    //look for Merge Nodes connected with this Decision Node
    IdList mergeNodes;
    IdList incomingConnections = mApi.incomingConnections(idDecisionNode);
    foreach (Id aConnection, incomingConnections) {
        if (aConnection.element() == "ActivityDiagram_MergeNode") {
            mergeNodes.append(aConnection);
        }
    }

    if (mergeNodes.length() == 0) {
        mergeNode = Id();
    } else if (mergeNodes.length() == 1) {
        mergeNode = mergeNodes.at(0);
    } else {
        addError("Unable to serialize object " + objectType(idDecisionNode) + " with id: " + idDecisionNode.toString() + ". Is has to many connected Merge Nodes.");
    }

    return mergeNode;
}

Id JavaHandler::findDecisionNode(Id const &idMergeNode)
{
    Id decisionNode;

    //it is empty because there is no need in it right now, but will be in the future

    return decisionNode;
}

Id JavaHandler::findNonBodyLink(Id const &idDecisionNode)
{
    Id linkId = Id();

    IdList outgoingLinks = mApi.outgoingLinks(idDecisionNode);

    if (outgoingLinks.length() != 2) {
        addError("Unable to serialize object " + objectType(idDecisionNode) + " with id: " + idDecisionNode.toString() + ". May be you forget a Merge Node before this Decision Node.");
    } else {
        QString guard1 = getFlowGuard(outgoingLinks.at(0));
        QString guard2 = getFlowGuard(outgoingLinks.at(1));

        //they can't be equal "" at the same time; one of them ought to be "" or to be "else"
        if (guard1 != "" || guard2 != "") {
            if ( (guard1 == "" || guard1 == "else") || (guard2 == "" || guard2 == "else") ) {
                //Supposing that the first link is represent the loop's body. If not — .swap
                if (guard1 == "" || guard1 == "else") {
                    outgoingLinks.swap(0, 1);
                }

                linkId = outgoingLinks.at(1);
            } else {
                addError("Unable to serialize object " + objectType(idDecisionNode) + " with id: " + idDecisionNode.toString() + ". One of the guards must either empty or equals \"else\".");
            }
        } else {
            addError("Unable to serialize object " + objectType(idDecisionNode) + " with id: " + idDecisionNode.toString() + ". At least one of the links must have a guard.");
        }
    }

    return linkId;
}

Id JavaHandler::findJoinNode(Id const &idForkNode)
{
    Id joinNode;

    //it is empty because there is no need in it right now, but will be in the future

    return joinNode;
}

QString JavaHandler::serializeChildren(Id const &idParent)
{
    QString result = "";
    IdList childElems;

    if (objectType(idParent) == "ActivityDiagram_ActivityDiagramNode") {
        childElems = getActivityDiagramChildren(idParent);
    } else if (objectType(idParent) == "ClassDiagram_Class") {
        IdList allChildren = mApi.children(idParent);
        IdList fields, methods;

        foreach (Id aChild, allChildren) {
            if (aChild.element() == "ClassDiagram_ClassField") {
                fields.append(aChild);
            } else if (aChild.element() == "ClassDiagram_ClassMethod") {
                methods.append(aChild);
            } //TODO: add processing of another nodes that may be in Class
        }

        childElems.append(fields);
        childElems.append(methods);
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

    if ((objectType(id) == "ClassDiagram_Class")||(objectType(id) == "ClassDiagram_Interface")) {

        mIndent = 0;

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

        out << indent() + isAbstractField + isFinalField + visibility + "class " + mApi.name(id) + parents +  " {" + "\n";
        mIndent++;
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

                    out << indent() + isFinalField + visibility + isStaticField + isVolatileField + isTransientField + type + mApi.name(aLink);
                    out << indent() + ";\n";
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

                    out << indent() + isFinalField + visibility + isStaticField + isVolatileField + isTransientField + type + mApi.name(aLink);
                    out << indent() + ";\n";
                }
            }
        }

        mIndent--;
        out << indent() + "} //end of class\n";
    } else if (objectType(id) == "ClassDiagram_View") {
        //	    to do someting
    } else if (objectType(id) == "ClassDiagram_ClassMethod") {
        IdList parents = mApi.parents(id);
        if (!parents.isEmpty()) {
            Id parentId = parents.at(0);
            QString const parentType = objectType(parentId);

            if (parentType == "ClassDiagram_Class") {
                result += indent();

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
                          type  + mApi.name(id) + "(" + operationFactors + ") " + methodBody + "\n";
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
                result += indent();

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
                if (aLink.element() == "ActivityDiagram_ObjectFlow") {
                    addError("Object " + objectType(id) + " with id  " + id.toString() + ". Only Control Edges can have Initial Nodes as source.");
                }
            }
        }

        result += "{\n";
        mIndent++;
    } else if (objectType(id) == "ActivityDiagram_Action") {
        result += indent() + mApi.name(id) + "\n";
    } else if (objectType(id) == "ActivityDiagram_ActivityFinalNode") {
        mIndent--;
        result += indent() + "} /*final node*/   \n"; //TODO: delete /*final node*/!

        //[Superstructure 09-02-02][1] A final node has no outgoing edges.
        IdList outgoingLinks = mApi.outgoingLinks(id);
        if (!outgoingLinks.isEmpty()) {
            addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". A final node has no outgoing edges.");
        }
    } else if (objectType(id) == "ActivityDiagram_DecisionNode") {
        //"if" or "while"?
        IdList incomingLinks = mApi.incomingLinks(id);
        if (incomingLinks.length() == 1) { //"if"
            result += ifStatement(id);
        } else if (incomingLinks.length() == 2) { //"while"
            result += whileDoLoop(id);
        }

        result += indent() + "//end of the decision node\n"; //TODO: delete!
    } else if (objectType(id) == "ActivityDiagram_MergeNode") {
        result += indent() + "//merge node \n"; //TODO: delete!
    }

    return result;
}

QString JavaHandler::serializeUntil(Id &id, Id const &untilElement)
{
    QString result = "";

    if (id == untilElement) {
        return result;
    } else if (id.element() == "ActivityDiagram_ActivityFinalNode") {
        result += indent() + "//return; \n"; //TODO: fill this
        addError("If you want \"return;\" you should write it in the Action before this Final Node.");
    } else {
        result += serializeObject(id);

        Id nextImportaintNode = id;
        if (id.element() == "ActivityDiagram_DecisionNode") {
            //"if" or "while"?
            IdList incomingLinks = mApi.incomingLinks(id);
            if (incomingLinks.length() == 1) { //"if"
                nextImportaintNode = findMergeNode(id);
            } else if (incomingLinks.length() == 2) { //"while"

                //It can fail here. Need to be rewritten
                Id nonBodyLink = findNonBodyLink(id);
                nextImportaintNode = mApi.otherEntityFromLink(nonBodyLink, id);
                result += serializeObject(nextImportaintNode);
            }
        }

        if (nextImportaintNode != Id()) {
            //serialise it's children
            IdList outgoingLinks = mApi.outgoingLinks(nextImportaintNode);
            foreach (Id aLink, outgoingLinks) {
                Id nextElement = mApi.otherEntityFromLink(aLink, nextImportaintNode);
                result += serializeUntil(nextElement, untilElement);
            }
        }
    }

    return result;
}

QString JavaHandler::ifStatement(Id const &id)
{
    QString result = indent() + "";

    Id untilMergeNode = findMergeNode(id);

    int existElse = 0; //for checking that there is no 2 outgoing links with "else" as a guard

    //move "else" link to the end of the list
    IdList outgoingLinks = mApi.outgoingLinks(id);
    foreach (Id aLink, outgoingLinks) {
        //if this link represent "else" case than change it with the last link in the serialization sequence
        if (getFlowGuard(aLink) == "else") {
            if (existElse == 1) {
                addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". There are two objects with \"else\" as guard.");
            }
            existElse = 1;
            outgoingLinks.swap(outgoingLinks.indexOf(aLink), outgoingLinks.length()-1);
        }
    }

    //serialization
    foreach (Id aLink, outgoingLinks) {
        Id caseBody = mApi.otherEntityFromLink(aLink, id);
        QString guard = getFlowGuard(aLink);
        if (guard != "else") {
            result += "if (" + guard + ") ";
        }

        //if it is a straight link from Decision Node to Merge Node
        result += "{\n";
        mIndent++;
        result += serializeUntil(caseBody, untilMergeNode);
        mIndent--;
        result += indent() + "}";

        if (aLink == outgoingLinks.at(outgoingLinks.length()-1)) { // if it is the last link then we should finish "if"-statement
            result += indent() + "/*last link in the decision node*/  \n";
        } else { //if it is not the last link, connected to the Decision Node
            result += " else ";
        }
    }

    //if there is no merge node than we must close the function
    if (untilMergeNode == Id()) {
        mIndent--;
        result += indent() + "}\n";
    }

    return result;
}

QString JavaHandler::whileDoLoop(Id const &id)
{
    QString result = indent() + "";

    //get the "body" link
    Id nonBodyLink = findNonBodyLink(id);
    IdList outgoingLinks = mApi.outgoingLinks(id);
    outgoingLinks.removeAll(nonBodyLink);
    Id bodyLink = outgoingLinks.at(0);

    result += "while (" + getFlowGuard(bodyLink) + ") {\n";
    mIndent++;

    //Serialization of the loop's body
    Id nextElement = mApi.otherEntityFromLink(bodyLink, id);
    result += serializeUntil(nextElement, id);
\
    mIndent--;
    result += indent() + "} /*end of \"while\"*/  \n"; //TODO: delete /*end of "while"*/!

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
        Id realizationDiagram;

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

QString JavaHandler::indent()
{
    QString result;
    for (int i = 0; i < mIndent; ++i)
        result += "\t";
    return result;
}
