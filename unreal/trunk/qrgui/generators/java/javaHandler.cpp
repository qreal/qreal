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

    IdList allDiagrams = mApi.children(repoId);
    IdList classDiagrams;

    //separate just class diagrams, because they are the main diagrams, others are connected
    foreach (Id const aDiagram, allDiagrams) {
        if (objectType(aDiagram) == "ClassDiagram_ClassDiagramNode") {
            classDiagrams.append(aDiagram);
        }
        if (objectType(aDiagram) == "ActivityDiagram_ActivityDiagramNode") {
            //If there is no connected Class Methods it won't be serialized
            IdList incomingConnections = mApi.incomingConnections(aDiagram);
            if (incomingConnections.isEmpty()) {
                addError("Unable to serialize object " + objectType(aDiagram) + " with id: " + aDiagram.toString() + ". It is not connected to some class method.");
            }
        }
    }

    foreach (Id const classDiagram, classDiagrams) {
        serializeChildren(classDiagram);
    }

    qDebug() << "Done.";
    return mErrorText;
}

Id JavaHandler::findMergeNode(Id const &idDecisionNode)
{
    Id mergeNode = Id();

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
                //Supposing that the first link is represent the loop's body. If not --- .swap
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

//Returns "importaint" nodes between startNode (including) and untilNode (including)
IdList JavaHandler::findIntermediateNodes(Id const &startNode, Id const &untilNode, bool const closesMethod)
{
    IdList children;

    if (startNode != untilNode) {
        children.append(startNode);

        if (startNode.element() == "ActivityDiagram_DecisionNode") {
            int isControlFlow = -1; //for checking in and outgoing links connected to DecisionNode
            if (!mApi.links(startNode).isEmpty()) {
                IdList outgoingLinks = mApi.outgoingLinks(startNode);
                if (!outgoingLinks.isEmpty()) {
                    // Very arched way to check:
                    // [Superstructure 09-02-02.pdf][2] The edges coming into and out of a DecisionNode, other than the decisionInputFlow (if any), must be either all ObjectFlows or all ControlFlows.
                    foreach (Id const aLink, outgoingLinks) {
                        if (aLink.element() == "ActivityDiagram_ControlFlow") {
                            if (isControlFlow == 0) {
                                addError("Unable to serialize object " + objectType(startNode) + " with id: " + startNode.toString() + ". The edges coming out must be either all Object Flows or all Control Flows.");
                            }
                            isControlFlow = 1;
                        } else if (aLink.element() == "ActivityDiagram_ObjectFlow") {
                            if (isControlFlow == 1) {
                                addError("Unable to serialize object " + objectType(startNode) + " with id: " + startNode.toString() + ". The edges coming out must be either all Object Flows or all Control Flows.");
                            }
                            isControlFlow = 0;
                        } //TODO: To check if the link is not Control Flow or Object Flow and fail the generation if it is.
                    }

                    //"if" or "while"?
                    IdList incomingLinks = mApi.incomingLinks(startNode);
                    deleteCommentLinks(incomingLinks);
                    deleteConstraintEdges(incomingLinks);
                    if (incomingLinks.length() == 1) { //"if"
                        Id mergeNode = findMergeNode(startNode);

                        if (mergeNode != Id()) {
                            children.append(findIntermediateNodes(mergeNode, untilNode, closesMethod));
                        }
                    } else if (incomingLinks.length() == 2) { //"while"
                        Id nonBodyLink = findNonBodyLink(startNode);

                        if (nonBodyLink != Id()) {
                            Id firstNonBodyElement = mApi.otherEntityFromLink(nonBodyLink, startNode);
                            if (firstNonBodyElement != Id()) {
                                children.append(findIntermediateNodes(firstNonBodyElement, untilNode, closesMethod));
                            } else { //wrong end of the link
                                addError("Unable to serialize object " + objectType(nonBodyLink) + " with id: " + nonBodyLink.toString() + ". A decision Node has one or two incoming edges.");
                            }
                        }
                    } else { //[Superstructure 09-02-02][1] A decision node has one or two incoming edges.
                        addError("Unable to serialize object " + objectType(startNode) + " with id: " + startNode.toString() + ". A decision Node has one or two incoming edges.");
                    }

                } else {
                    addError("Unable to serialize object " + objectType(startNode) + " with id: " + startNode.toString() + ". Is must have at least one outgoing edge.");
                }
            } else {
                addError("Unable to serialize object " + objectType(startNode) + " with id: " + startNode.toString() + ". Is must have at least one outgoing edge.");
            }
        } else if (startNode.element() == "ActivityDiagram_MergeNode") {
            //TODO: fill this
            IdList outgoingLinks = mApi.outgoingLinks(startNode);
            deleteCommentLinks(outgoingLinks);
            deleteConstraintEdges(outgoingLinks);

            foreach (Id aLink, outgoingLinks) {
                Id nextElement = mApi.otherEntityFromLink(aLink, startNode);
                if (nextElement != Id()) {
                    children.append(findIntermediateNodes(nextElement, untilNode, closesMethod));;
                } else {
                    addError("Unable to serialize object " + objectType(aLink) + " with id: " + aLink.toString() + ". It does not have the target-node.");
                }
            }
        } else if (startNode.element() == "ActivityDiagram_ActivityFinalNode") {
            if (!closesMethod) {
                Id returnNode = children.at(children.length() - 1);
                addError("Node: " + returnNode.toString() + ". If you want \"return;\" you should write it in the Action before this Final Node.");
            }
        } else if (startNode.element() == "ActivityDiagram_Activity") { //"try-catch" or just Activity
            IdList outgoingLinks = mApi.outgoingLinks(startNode);
            deleteCommentLinks(outgoingLinks);
            deleteConstraintEdges(outgoingLinks);

            //Delete all "exception"-links from the list. They will be serialized by the Activity itself.
            foreach (Id aLink, outgoingLinks) {
                if (getFlowGuard(aLink) != "") {
                    outgoingLinks.removeAll(aLink);
                }
            }

            foreach (Id aLink, outgoingLinks) {
                Id nextElement = mApi.otherEntityFromLink(aLink, startNode);
                if (nextElement != Id()) {
                    children.append(findIntermediateNodes(nextElement, untilNode, closesMethod));
                } else {
                    addError("Unable to serialize object " + objectType(aLink) + " with id: " + aLink.toString() + ". It does not have the target-node.");
                }
            }
        } //TODO: add other "importaint" nodes
        else { //Think, that it is Action, Initial, Final.
            IdList outgoingLinks = mApi.outgoingLinks(startNode);
            deleteCommentLinks(outgoingLinks);
            deleteConstraintEdges(outgoingLinks);

            foreach (Id aLink, outgoingLinks) {
                Id nextElement = mApi.otherEntityFromLink(aLink, startNode);
                if (nextElement != Id()) {
                    children.append(findIntermediateNodes(nextElement, untilNode, closesMethod));
                } else {
                    addError("Unable to serialize object " + objectType(aLink) + " with id: " + aLink.toString() + ". It does not have the target-node.");
                }
            }
        }
    }

    return children;
}

IdList JavaHandler::deleteCommentLinks(IdList &idList)
{
    IdList result;

    foreach (Id aLink, idList) {
        if (aLink.element() == "ActivityDiagram_CommentLink") {
            result.append(aLink);
            idList.removeAll(aLink);
        }
    }

    return result;
}

IdList JavaHandler::deleteConstraintEdges(IdList &idList)
{
    IdList result;

    foreach (Id aLink, idList) {
        if (aLink.element() == "ActivityDiagram_ConstraintEdge") {
            result.append(aLink);
            idList.removeAll(aLink);
        }
    }

    return result;
}

IdList JavaHandler::getActivityChildren(Id const &idStartNode, Id const &idUntilNode)
{
    IdList result;

    //if the Final Node, that we will find closes the Method
    bool closesMethod = idStartNode != Id() && idStartNode.element() == "ActivityDiagram_InitialNode";

    result.append(findIntermediateNodes(idStartNode, idUntilNode, closesMethod));
    if (idUntilNode != Id()) {
        result.append(idUntilNode);
    }

    return result;
}

QString JavaHandler::serializeChildren(Id const &idParent)
{
    QString result = "";
    IdList childElems, allChildren = mApi.children(idParent);

    if (objectType(idParent) == "ActivityDiagram_ActivityDiagramNode" || objectType(idParent) == "ActivityDiagram_Activity") {
        IdList startNodes;

        foreach (Id aChild, allChildren) {
            //Search for the nodes that can start control flows
            //We are hoping that there is just one such node on the Activity Diagram
            if (aChild.element() == "ActivityDiagram_InitialNode" || aChild.element() == "ActivityDiagram_ActivityParameterNode"
                || aChild.element() == "ActivityDiagram_AcceptEventAction") {
                startNodes.append(aChild);
            }
        }

        if (startNodes.length() == 0) {
            addError("Unable to serialize object " + objectType(idParent) + " with id: " + idParent.toString() + ". There is no start nodes (Initial Node, Activity Parameter Node, Accept Event Action).");
        } else if (startNodes.length() > 1) { //TODO: to find out if it is possible. If so, write the appropriate serialization
            addError("AAA!!! There are too many start nodes. I don't know which to choose.");
        } else {
            Id startNode = startNodes.at(0);
            childElems = getActivityChildren(startNode, Id());
        }
    } else if (objectType(idParent) == "ClassDiagram_Class") {
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
        childElems = allChildren;
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
        QString imports = getImports(id);
        QString isFinalField = hasModifier(id, "final");
        QString isAbstractField = hasModifier(id, "abstract");
        QString parents = getSuperclass(id);

        if (isAbstractField == "abstract " && isFinalField == "final ") {
            addError("unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". \"abstract final\" declaration doesn't make sense");
        }

        out << imports;
        out << indent() + visibility + isAbstractField + isFinalField + "class " + mApi.name(id) + parents +  " {" + "\n";
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
                    result += " = " + defaultValue;
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

        result += getConstraints(id);
    } else if (objectType(id) == "ActivityDiagram_Action") {
        result += getComments(id);

        IdList outgoingConnections = mApi.outgoingConnections(id);
        IdList activityDiagrams;
        foreach (Id aConnection, outgoingConnections) {
            if (aConnection.element() == "ActivityDiagram_ActivityDiagramNode") {
                activityDiagrams.append(aConnection);
            }
        }

        if (activityDiagrams.length() > 1) {
            addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". There are too many Activity Diagrams connected.");
        }

        QString code = getConstraints(id);

        if (activityDiagrams.isEmpty() && code == "") {
            result += indent() + mApi.name(id).replace("<br/>", "\n" + indent()) + "\n";
        } else {
            if (activityDiagrams.isEmpty() && code != "") {
                result += indent() + code + "\n";
            } else if (!activityDiagrams.isEmpty() && code == "") {
                result += serializeChildren(activityDiagrams.at(0));
            } else {
                addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". You can not set code into the Constraints and Activity Diagram at the same time.");
            }
        }
    } else if (objectType(id) == "ActivityDiagram_ActivityFinalNode") {
        result += getComments(id) + getConstraints(id);

        //[Superstructure 09-02-02][1] A final node has no outgoing edges.
        IdList outgoingLinks = mApi.outgoingLinks(id);
        if (!outgoingLinks.isEmpty()) {
            addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". A final node has no outgoing edges.");
        }
    } else if (objectType(id) == "ActivityDiagram_DecisionNode") {
        result += getComments(id);
        //"if" or "while"?
        IdList incomingLinks = mApi.incomingLinks(id);
        deleteCommentLinks(incomingLinks);
        deleteConstraintEdges(incomingLinks);
        if (incomingLinks.length() == 1) { //"if"
            result += ifStatement(id);
        } else if (incomingLinks.length() == 2) { //"while"
            result += whileDoLoop(id);
        }

        result += indent() + "\n";
    } else if (objectType(id) == "ActivityDiagram_MergeNode") {
        result += getComments(id);
    } else if (objectType(id) == "ActivityDiagram_Activity") {
        result += getComments(id) + getConstraints(id);
        //search for "exception"-links
        IdList outgoingLinks = mApi.outgoingLinks(id);
        IdList exceptions;
        foreach (Id aLink, outgoingLinks) {
            if (getFlowGuard(aLink) != "") {
                exceptions.append(aLink);
            }
        }

        if (exceptions.isEmpty()) { //if it is just an Activity
            result += serializeChildren(id);
        } else { //if it is "try-catch"
            result += tryCatch(id);
        }
    }

    return result;
}

QString JavaHandler::serializeActivity(Id const &idStartNode, Id const &idUntilNode)
{
    QString result = "";

    IdList childElems = getActivityChildren(idStartNode, idUntilNode);

    if (!childElems.isEmpty()) {
        Id last = childElems.takeLast(); //it will be serialized in the upper level activity

        if (last != Id() && (last.element() == "ActivityDiagram_Action" ||
                             last.element() == "ActivityDiagram_Activity")) {
            childElems.append(last);
        }
    }

    foreach (Id id, childElems) {
       if (id != Id()) {
            result += serializeObject(id);
        }
    }

    return result;
}

QString JavaHandler::tryCatch(Id const &id)
{
    QString result = "";
    int existFinally = 0; //for checking that there is no 2 outgoing links with "finally" as a guard

    //search for "exception"-links
    IdList outgoingLinks = mApi.outgoingLinks(id);
    IdList exceptions;
    foreach (Id aLink, outgoingLinks) {
        if (getFlowGuard(aLink) != "") {
            exceptions.append(aLink);
        }
    }

    //move "finally"-link to the end of the list
    foreach (Id aLink, exceptions) {
        //if this link represent "finally" case than change it with the last link in the serialization sequence
        if (getFlowGuard(aLink) == "finally") {
            if (existFinally == 1) {
                addError("Unable to serialize object " + objectType(id) + " with id: " + id.toString() + ". There are two objects with \"finally\" as guard.");
            }
            existFinally = 1;
            outgoingLinks.swap(outgoingLinks.indexOf(aLink), outgoingLinks.length()-1);
        }
    }

    result += indent() + "try {\n";
    mIndent++;
    result += serializeChildren(id);
    mIndent--;
    result += indent() + "}";

    foreach (Id anException, exceptions) {
        if (getFlowGuard(anException) != "finally") {
            result += " catch (" + getFlowGuard(anException) + ") {\n";
        } else {
            result += " finally {\n";
        }
        mIndent++;
        Id exceptionHandler = mApi.otherEntityFromLink(anException, id);
        result += serializeActivity(exceptionHandler, Id());
        mIndent--;
        result += indent() + "}";
    }

    result += "\n";

    return result;
}

QString JavaHandler::ifStatement(Id const &id)
{
    QString result = indent() + "";

    Id untilMergeNode = findMergeNode(id);
    int existElse = 0; //for checking that there is no 2 outgoing links with "else" as a guard

    //move "else" link to the end of the list and delete Constraint Edges.
    IdList outgoingLinks = mApi.outgoingLinks(id);
    foreach (Id aLink, outgoingLinks) {
        if (aLink.element() == "ActivityDiagram_ConstraintEdge") {
            outgoingLinks.removeAll(aLink);
            break;
        }
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

        result += "{\n";
        mIndent++;
        if (caseBody != untilMergeNode) {
            result += serializeActivity(caseBody, untilMergeNode);
        }
        mIndent--;
        result += indent() + "}";

        if (aLink != outgoingLinks.at(outgoingLinks.length()-1)) { //if it is not the last link, connected to the Decision Node
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
    result += serializeActivity(nextElement, id);
\
    mIndent--;
    result += indent() + "}\n";

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
        //TODO: delete it!!! That is just because QReal's sh~
        guard.replace("&lt;", "<");
        guard.replace("&rt;", ">");

        result = guard.simplified(); //delete whitespaces from the start and the end and internal whitespaces replace with a single space

//        if (guard == "") {
//            addError("Object " + objectType(id) + " with id  " + id.toString() + " has empty guard property.");
//        }
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

        result = type + " ";
        if ( !isTypeSuitable(type) && !(objectType(id) == "ClassDiagram_ClassMethod" && type == "void") ) {
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
    QString result = "{\n";
    mIndent++;

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
            result += serializeChildren(realizationDiagram);
        } else if (realizationsCount == 0) { //if there is no realization
            addError("Method " + objectType(id) + " with id " + id.toString() + " will be empty.");
        } else { //if there is more than one ActivityDiagram connected
            addError("Object " + objectType(id) + " with id " + id.toString() + " has too many realizations");
        }

    } else {
        addError("Method " + objectType(id) + " with id " + id.toString() + " will be empty.");
    }

    mIndent--;
    result += indent() + "}\n";
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

        result = operationFactors;
    }

    return result;
}

QString JavaHandler::getDefaultValue(Id const &id)
{
    QString result = "";

    if (mApi.hasProperty(id, "defaultValue")) {
        QString defaultValue = mApi.stringProperty(id, "defaultValue");

        result = defaultValue;
    }

    return result;
}

QString JavaHandler::getImports(Id const &id)
{
    QString result = "";

    if (mApi.hasProperty(id, "elementImport")) {
        QString elementImport = mApi.stringProperty(id, "elementImport");
        if (elementImport != "") {
            QStringList imports = elementImport.split(",");

            foreach (QString anImport, imports) {
                anImport = anImport.simplified();
                if (anImport != "")
                    result += indent() + "import " + anImport + ";\n";
            }
            result += indent() + "\n";
        }
    }

    return result;
}

QString JavaHandler::getConstraints(Id const &id)
{
    QString result = "";

    //if it has the Constraint nodes
    IdList outgoingLinks = mApi.outgoingLinks(id);
    IdList constraints = deleteConstraintEdges(outgoingLinks);

    if (constraints.length() >= 2) {
        addError("Object " + objectType(id) + " with id: " + id.toString() + ". The order of Constraints generalization can not be defined.");
    }

    foreach (Id aConstraint, constraints) {
        Id constraint = mApi.otherEntityFromLink(aConstraint, id);
        QString code = getConstraint(constraint);

        result += code;
    }

    return result;
}

QString JavaHandler::getConstraint(Id const &id)
{
    QString result = "";

    if (mApi.hasProperty(id, "specification")) {
        QString specification = mApi.stringProperty(id, "specification");
        specification.replace("<br/>", "\n" + indent());

        result = specification;
    }

    return result;
}

QString JavaHandler::getComments(Id const &id)
{
    QString result = "";

    IdList outgoingLinks = mApi.outgoingLinks(id);
    IdList comments = deleteCommentLinks(outgoingLinks);

    if (comments.length() >= 2) {
        addError("Object " + objectType(id) + " with id: " + id.toString() + ". The order of Comments generalization can not be defined.");
    }

    foreach (Id aCommentLink, comments) {
        Id comment = mApi.otherEntityFromLink(aCommentLink, id);
        QString body = getComment(comment);

        result += body + "\n";
    }

    return result;
}

QString JavaHandler::getComment(Id const &id)
{
    QString result = "";

    if (mApi.hasProperty(id, "body")) {
        QString body = mApi.stringProperty(id, "body");

        QStringList strings = body.split("<br/>");
        if (strings.length() == 1) {
            result = indent() + "// " + body;
        } else {
            result += indent() + "/*\n";
            foreach (QString aString, strings) {
                result += indent() + "* " + aString + "\n";
            }
            result += indent() + "*/";
        }
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
