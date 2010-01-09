#include "csharpHandler.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/definitions.h"
#include "../../client/repoApi.h"

using namespace qReal;
using namespace generators;

CsharpHandler::CsharpHandler(client::RepoApi const &api)
		: mApi(api)
{
}

Interfac::Interfac(){
    name =" ";
    visibility = "";
    numVariables = 0;
    numMethods = 0;
}
QString Interfac::Print(void){
    QString result = "";

    if(visibility != "")
        result += visibility + " ";
    result +="interface "+ name;
    bool key = false;
    foreach (QString str,interf ){
        if (key == false){
            result +=": "+ str;
            key = true;
        }else{
            result += ",  " + str;
        }
    }
    result+="{\n";

    foreach (Variable var,variables ){
          result += "  " + var.Print();
    }

    Method met;
    for(int i = 0; i < numMethods; i++){
         met = methods.at(i);
         result += "  " + met.Print();
    }


    result +="}\n\n";
    variables.clear();
    methods.clear();
    return result;
}

Variable::Variable(){
    isStatic = false;
    isFinal = false;
}
QString Variable::Print(){
    QString result = "";
    if(visibility != "")
        result += visibility + " ";
    result += type + " ";
    if(isStatic)
            result += "static ";
    result += name;
    if(defaultVal != "")
        result += " = "+defaultVal;
    result += ";\n";
    return result;
}

Method::Method(){
    isStatic = false;
    isFinal = false;
}
QString Method::Print(){
    QString result = "";
    if(visibility != "")
        result += visibility + " ";
    if(isStatic)
        result += "static ";
    result += type + " "+ name + "()";

    result += "{}\n";
    return result;
}

ClassSet::ClassSet(){
    name =" ";
    property.visibility = "";
    property.isLeaf = false;
    property.isAbstract = false;

    numVariables = 0;
    numMethods = 0;
}

QString ClassSet::Print(void){
    QString result = "";

    if(property.visibility != "")
        result += property.visibility + " ";
    if(property.isAbstract)
        result +="abstract ";
    if(property.isLeaf)
        result +="sealed ";
    result +="class "+ name;
    if(cl != ""){
        result += ": "+cl;
        foreach (QString str,interf ){
            result += ",  " + str;
         }
    }else{
        bool key = false;
        foreach (QString str,interf ){
            if (key == false){
                result +=": "+ str;
                key = true;
            }else{
                result += ",  " + str;
            }
        }
    }
    result+="{\n";

    foreach (Variable var,variables ){
        result += "  " + var.Print();
    }

    Method met;
    for(int i = 0; i < numMethods; i++){
         met = methods.at(i);
         result += "  " + met.Print();
    }

    result +="}\n\n";
    variables.clear();
    methods.clear();
    return result;
}

QString CsharpHandler::generateToCsharp(QString const &pathToFile)
{
	mErrorText = "";
        number = 0;
        numberInterface = 0;

        QFile file(pathToFile+".txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return "";

	QTextStream out(&file);
 out << "using System;\n";
        Id repoId = ROOT_ID;

       serializeChildren(repoId);

        IdList rootDiagrams = mApi.children(repoId);

        foreach (Id const typeDiagram, rootDiagrams) {
            IdList childElems = mApi.children(typeDiagram);
            foreach (Id const id, childElems) {
                CreateSet(id);
            }
        }

        foreach (Id const typeDiagram, rootDiagrams) {
          //  out<<mApi.name(typeDiagram);
            //out<< mApi.typeName(typeDiagram);
            out << serializeChildren(typeDiagram);
        }

       Interfac newInterface;
        for(int i = 0; i < numberInterface; i++){
            newInterface = interfaces.at(i);
            out << newInterface.Print();

        }


        ClassSet newClass;
        for(int i = 0; i<number; i++){
            newClass = classes.at(i);
            out << newClass.Print();

        }

        classes.clear();
        interfaces.clear();
	qDebug() << "Done.";
        return mErrorText;
}

QString CsharpHandler::serializeChildren(Id const &idParent)
{
	QString result = "";
	IdList childElems = mApi.children(idParent);

	foreach (Id const id, childElems) {
		result += serializeObject(id, idParent);
	}

	return result;
}

void CsharpHandler::CreateSet(Id const &id){
    QString const objectType = mApi.typeName(id);
    if (objectType == "cnClass") {
        nameClass << mApi.name(id);
    }else if(objectType == "cnInterface"){
        nameInterface << mApi.name(id);
    }
}

bool CsharpHandler::isClassExist(QString const &name) const {
  return nameClass.contains(name);
}

bool CsharpHandler::isInterfaceExist(QString const &name){
  return nameInterface.contains(name);
}

QString CsharpHandler::serializeObject(Id const &id, Id const &parentId)
{
        QString result = "";

	QString additionalParams = "";

	QString const objectType = mApi.typeName(id);
	QString const parentType = mApi.typeName(parentId);

	if (objectType == "krnnDiagram") {
	}

	// class diagramm

	else if (objectType == "cnClass") {
            number++;
            ClassSet newClass;

            newClass.property.isAbstract = getAbstract(id);
            newClass.property.isLeaf = getFinal(id);
            if ( newClass.property.isAbstract && newClass.property.isLeaf){
                addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". \"abstract final\" declaration doesn't make sence");
            }

            newClass.cl = getParent(id);
            newClass.property.visibility = getVisibility(id);
            newClass.name = mApi.name(id);
            newClass.interf = getParents(id);
            newClass.variables = getObjectVariable(id);
            classes << newClass;
            serializeChildren(id);
	} else if (objectType == "cnClassView") {
		//	    to do someting
	} else if (objectType == "cnClassMethod") {
		if (parentType == "cnClass") {
                        classes[number-1].numMethods++;
                        Method method;
                        method.visibility = getVisibility(id);
                        method.type = getType(id);
                        method.isFinal= getFinal(id);
                        method.isStatic = getStatic(id);
                        method.name = mApi.name(id);

                        classes[number-1].methods << method;

                    } else if(parentType == "cnInterface"){
                        interfaces[numberInterface-1].numMethods++;
                        Method method;
                        method.type = getType(id);
                        method.name = mApi.name(id);
                        interfaces[numberInterface-1].methods << method;
                    } else {
			this->addError("unable to serrialize object " + objectType + " with id: " + id.toString() + ". Move it inside some cnClass");
		}
	} else if (objectType == "cnClassField") {
                if (parentType == "cnClass"){
                        Variable var;
                        var.visibility = getVisibility(id);
                        var.type = getType(id);
                        var.isFinal= getFinal(id);
                        var.isStatic = getStatic(id);
                        var.defaultVal = getDefaultValue(id);
                        var.name = mApi.name(id);
                        classes[number-1].variables << var;
                    } else if(parentType == "cnInterface"){
                        interfaces[numberInterface-1].numVariables++;
                        Variable var;
                        var.type = getType(id);
                        var.name = mApi.name(id);
                        interfaces[numberInterface-1].variables << var;
                    } else{
			addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". Move it inside some cnClass");
		}
        }else if (objectType == "cnInterface"){
 //           result +="inter";
            numberInterface++;
            Interfac newInterface;
            newInterface.visibility = getVisibility(id);
            newInterface.name = mApi.name(id);
            newInterface.interf = getParents(id);
            newInterface.variables = getObjectVariable(id);
            interfaces << newInterface;
            serializeChildren(id);
        }

	return result;

}

QList<Variable> CsharpHandler::getObjectVariable(Id const id){

    QList<Variable> result;
        Variable var;
        QString const objectType = mApi.typeName(id);
        if (!mApi.links(id).isEmpty()) {
                IdList links = mApi.outcomingLinks(id);

                foreach (Id const aLink, links){
                    if ((aLink.element() == "ceAggregation")&&(mApi.typeName(mApi.otherEntityFromLink(aLink, id))=="cnClass")) {
                        var.type = mApi.name(mApi.otherEntityFromLink(aLink, id));
                        var.name = "class" + var.type;
                        result << var;
                    }


                 }
                }



      return result;
}

QString CsharpHandler::getVisibility(Id const &id)
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
                        visibility = "internal";

		if (isVisibilitySuitable(visibility)) {
			result = visibility;
                } else {
			addError("object " + objectType + " with id  " + id.toString() + " has invalid visibility property: " + visibility);
		}
	}

	return result;
}


bool CsharpHandler::getAbstract(Id const &id){

    QString const objectType = mApi.typeName(id);

        if (mApi.hasProperty(id, "isAbstract")) {
                QString isAbstract = mApi.stringProperty(id, "isAbstract");

                if (isAbstract == "true")
                        return true;
                else return false;
        }
        return false;
}

bool CsharpHandler::getFinal(Id const &id){

    QString const objectType = mApi.typeName(id);

        if (mApi.hasProperty(id, "isLeaf")) {
                QString isAbstract = mApi.stringProperty(id, "isLeaf");

                if (isAbstract == "true")
                        return true;
                else return false;
        }
        return false;
}
bool CsharpHandler::getStatic(Id const &id){

    QString const objectType = mApi.typeName(id);

        if (mApi.hasProperty(id, "isStatic")) {
                QString isStatic = mApi.stringProperty(id, "isStatic");

                if (isStatic == "true")
                        return true;
                else return false;
        }
        return false;
}
QString CsharpHandler::getType(Id const &id)
{
	QString result = "";

	QString const objectType = mApi.typeName(id);

	if (mApi.hasProperty(id, "type")) {
                QString type = mApi.stringProperty(id, "type");
                result = type;
            if (isTypeSuitable(type) || (objectType == "cnClassMethod" && type == "void")) {
			result = type;
			if (result != "")
				result += " ";
		} else {
                   addError("object " + objectType + " with id " + id.toString() + " has invalid type: " + type);
                }
	}

	return result;
}

QString CsharpHandler::getParent(Id const &id)
{
	QString result = "";
bool hasParentClass = false;

	QString const objectType = mApi.typeName(id);

	if (!mApi.links(id).isEmpty()) {
		IdList links = mApi.outcomingLinks(id);

		foreach (Id const aLink, links){
                    if ((aLink.element() == "krneGeneralization")&&(mApi.typeName(mApi.otherEntityFromLink(aLink, id))=="cnClass")) {
				if (hasParentClass == false) {
					hasParentClass = true;
					if (id == mApi.otherEntityFromLink(aLink, id)) {
						addError("object " + objectType + " with id " + id.toString() + " can not be his own superclass");
					} else {
						QString parentClass = mApi.name(mApi.otherEntityFromLink(aLink, id));
                                                result += parentClass;
					}
				} else {
					addError("object " + objectType + " with id " + id.toString() + " has too much superclasses");
				}
			}
		}

	}

	return result;
}

QList<QString> CsharpHandler::getParents(Id const &id)
{
        QList<QString> result;
        QString interf="";
        QString const objectType = mApi.typeName(id);
        if (!mApi.links(id).isEmpty()) {
                IdList links = mApi.outcomingLinks(id);

                foreach (Id const aLink, links){
                    if ((aLink.element() == "krneGeneralization")&&(mApi.typeName(mApi.otherEntityFromLink(aLink, id))=="cnInterface")) {
                        if (id == mApi.otherEntityFromLink(aLink, id)) {
                                addError("object " + objectType + " with id " + id.toString() + " can not be his own superclass");
                        } else {
                            interf = mApi.name(mApi.otherEntityFromLink(aLink, id));

                            }

                           result << interf;
                           interf = "";
                        }
                }

        }

      return result;
}
/*не понимаю, что делает*/
QString CsharpHandler::hasModifier(Id const &id, QString const &modifier)
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

QString CsharpHandler::getOperationFactors(Id const &id)
{
	QString result = "";

	QString const objectType = mApi.typeName(id);

	if (mApi.hasProperty(id, "type")) {
		QString operationFactors = mApi.stringProperty(id, "operationFactors");

		//	to check for the corract data
		//	if (isTypeSuitable(type) || (objectType == "cnClassMethod" && type == "void")) {
		result = operationFactors;
		//	} else {
		//		addError("object " + objectType + " with id " + id.toString() + " has invalid type: " + type);
		//    	}
	}

	return result;
}

QString CsharpHandler::getDefaultValue(Id const &id)
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

bool CsharpHandler::isTypeSuitable(QString const &type) const
{
	//  todo: check class-type for correctness
	return type == "int" || type == "float" || type == "double" || type == "boolean"
                        || type == "char" || type == "byte" || type == "long" || type == "short" || isClassExist(type);
}

bool CsharpHandler::isVisibilitySuitable(QString const &visibility) const
{
        return visibility == "public" || visibility == "private" || visibility == "protected" || visibility == "internal"|| visibility == "";
}

void CsharpHandler::addError(QString const &errorText)
{
	mErrorText += errorText + "\n";
}
