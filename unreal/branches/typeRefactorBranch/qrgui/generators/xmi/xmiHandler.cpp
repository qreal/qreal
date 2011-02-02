#include "xmiHandler.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/roles.h"
#include "../../../qrrepo/repoApi.h"

#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;
using namespace utils;

XmiHandler::XmiHandler(qrRepo::RepoApi const &api)
	: mApi(api)
{
}

QString XmiHandler::exportToXmi(QString const &pathToFile)
{
	mErrorText = "";

	QDomElement header = mDocument.createElement("xmi:XMI");
	header.setAttribute("xmlns:xmi", "http://schema.omg.org/spec/XMI/2.1");
	header.setAttribute("xmlns:uml", "http://schema.omg.org/spec/UML/2.0");
	header.setAttribute("xmi:version", "2.1");
	mDocument.appendChild(header);

	QDomElement documentation = mDocument.createElement("xmi:Documentation");
	documentation.setAttribute("exporter", "QReal");
	mDocument.appendChild(documentation);

	//  --------------  content --------------- //
        NewType repoId = ROOT_ID;
        TypeList rootDiagrams = mApi.children(repoId);

        foreach (NewType const typeDiagram, rootDiagrams) {
		QDomElement package = createDomElement("uml:Package", typeDiagram.toString());
		package.setAttribute("name", mApi.name(typeDiagram));
		mDocument.appendChild(package);

		initPrimitiveTypes(package);
		serializeChildren(package, typeDiagram);
	}

	OutFile out(pathToFile);
	out() << "<?xml version='1.0' encoding='utf-8'?>\n";
	out() << mDocument.toString(4);

	qDebug() << "XMI generation done.";

	return mErrorText;
}

void XmiHandler::serializeLinks(QDomElement &parent, bool direction, NewType const &idParent)
{
        TypeList links = direction ? mApi.outgoingLinks(idParent) : mApi.incomingLinks(idParent);

        foreach (NewType const type, links) {
                QDomElement link = direction ? serializeOutcomingLink(type) : serializeIncomingLink(type);
		if (!link.isNull())
			parent.appendChild(link);
	}
}

void XmiHandler::serializeChildren(QDomElement &parent, NewType const &idParent)
{
        TypeList childElems = mApi.children(idParent);

        foreach (NewType const type, childElems)
                serializeObject(parent, type, idParent);

	if (idParent != ROOT_ID) {
		serializeLinks(parent, true, idParent);
		serializeLinks(parent, false, idParent);
	}
}

void XmiHandler::serializeObject(QDomElement &parent, NewType const &type, NewType const &parentId)
{
	typedef QPair<QString, QString> StringPair;

	QString typeOfElem = "";
	QString typeOfTag = "";
	QList<StringPair> additionalParams;

        QString const objectType = mApi.typeName(type);
	QString const parentType = mApi.typeName(parentId);

	if (objectType == "krnnDiagram") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Package";
	}

	// class diagramm

	else if (objectType == "cnClass") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Class";
	} else if (objectType == "cnClassView") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Package";
	}else if (objectType == "cnClassMethod") {
		if (parentType == "cnClass") {
			typeOfTag = "ownedOperation";
			typeOfElem = "uml:Operation";
		} else
                        addError("unable to serrialize object " + objectType + " with type: " + type.toString() + ". Move it inside some cnClass");
	} else if (objectType == "cnClassField") {
		if (parentType == "cnClass"){
			typeOfTag = "ownedAttribute";
			typeOfElem = "uml:Property";
		} else
                        addError("unable to serialize object " + objectType + " with type: " + type.toString() + ". Move it inside some cnClass");
	}

	//use case diagram

	else if (objectType == "uscnActor") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Actor";
	} else if (objectType == "uscnUseCase") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:UseCase";
	}

	// sequence diagramm

	else if (objectType == "sqnnInteraction") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Interaction";
	} else if (objectType == "sqnnSLifeline") {
		typeOfTag = "lifeline";
		typeOfElem = "uml:Lifeline";
	}

	// additional params
        if (mApi.hasProperty(type, "visibility")) {
                QString visibility = mApi.stringProperty(type, "visibility");

		if (!visibility.isEmpty()) {

			if (visibility == "+")
				visibility = "public";
			else if (visibility == "-")
				visibility = "private";
			else if (visibility == "#")
				visibility = "protected";
			else if (visibility == "~")
				visibility = "package";

			if (isVisibilitySuitable(visibility))
				additionalParams << StringPair("visibility", visibility);
			else
                                addError("object " + objectType + " with type  " + type.toString() + " has invalid visibility property: " + visibility);
		}
	}

        if (mApi.hasProperty(type, "type")) {
                QString typeStr = mApi.stringProperty(type, "type");

                if (!typeStr.isEmpty()) {
                        if (isTypeSuitable(typeStr))
                                additionalParams << StringPair("type", typeStr);
			else
                                addError("object " + objectType + " with type " + type.toString() + " has invalid type: " + typeStr);
		}
	}

	if (!typeOfElem.isEmpty() && !typeOfTag.isEmpty()) {
                QDomElement object = createDomElement(typeOfTag, type.toString(), typeOfElem);
                object.setAttribute("xmi:name", mApi.name(type));

		foreach (StringPair nameValuePair, additionalParams)
			object.setAttribute(nameValuePair.first, nameValuePair.second);

                serializeChildren(object, type);

		parent.appendChild(object);
                serializeLinkBodies(parent, type);
	}
}

QDomElement XmiHandler::serializeOutcomingLink(NewType const &type)
{
        QString linkType = mApi.typeName(type);
	QDomElement result;

	// kernel diagram
	if (linkType == "krnePackageImport") {
                result = createDomElement("packageImport", type.toString(), "uml:PackageImport");
                result.setAttribute("importedPackage", mApi.to(type).toString());
	} else if (linkType == "krneElementImport") {
                result = createDomElement("elementImport", type.toString(), "uml:ElementImport");
                result.setAttribute("importedElement", mApi.to(type).toString());
	} else if (linkType == "krneGeneralization") {
                result = createDomElement("generalization", type.toString(), "uml:Generalization");
                result.setAttribute("general", mApi.to(type).toString());
	} else if (linkType == "krneDirRelationship") {
                result = createDomElement("ownedAttribute", type.toString(), "uml:Property");
		result.setAttribute("visibility", "protected");
                result.setAttribute("type", mApi.to(type).toString());

                QString toMult = mApi.stringProperty(type, "toMultiplicity");
                serializeMultiplicity(result, type, toMult);

                createDomElementWithIdRef(result, "association", type.toString());
	}

	// class diagram
	else if (linkType == "ceDependency")
                result = createDomElementWithIdRef("clientDependency",  type.toString());

	// use case diagram
	else if (linkType == "uscaExtend") {
                result = createDomElement("extend", type.toString(), "uml:Extend");
                result.setAttribute("extendedCase", mApi.to(type).toString());

                createDomElementWithIdRef(result, "extension",  mApi.from(type).toString());
	} else if (linkType == "uscaInclude") {
                result = createDomElement("include", type.toString(), "uml:Include");
                result.setAttribute("addition", mApi.to(type).toString());
	}

	return result;
}

QDomElement XmiHandler::serializeIncomingLink(NewType const &type)
{
	QDomElement result;
        if (mApi.typeName(type) == "ceDependency")
                result = createDomElementWithIdRef("supplierDependency", type.toString());
	return result;
}

QDomElement XmiHandler::createOwnedEnd(QString const &direction, NewType const &type, NewType const &target)
{
        QDomElement result = createDomElement("ownedEnd", direction + "End" + type.toString(), "uml:Property");
	result.setAttribute("visibility", "protected");
	result.setAttribute("type", target.toString());

        QDomElement endAssociation = createDomElementWithIdRef("association", type.toString());

	QString multiplicityDirection = direction;
	multiplicityDirection[0] = multiplicityDirection[0].toLower();

        QString mult = mApi.stringProperty(type, multiplicityDirection + "Multiplicity");
        serializeMultiplicity(endAssociation, type, mult);
	result.appendChild(endAssociation);

        createDomElementWithIdRef(result, "association", type.toString());
	return result;
}

QDomElement XmiHandler::serializeLink(NewType const &type)
{
	QString visibility = "";
        QString linkType = mApi.typeName(type);

        QDomElement result = createDomElement("ownedMember", type.toString());

        if (!mApi.stringProperty(type, "visibility").isEmpty())
                visibility = mApi.property(type, "visibility").toString();

	if (linkType == "ceComposition" || linkType == "ceAggregation"
		|| linkType == "krneRelationship" || linkType == "ceRelation"
		|| linkType == "krneDirRelationship")
	{
		result.setAttribute("xmi:type", "uml:Association");
		if (!visibility.isEmpty())
			result.setAttribute("visibility", visibility);

		// FromEnd
                createDomElementWithIdRef(result, "memberEnd", "FromEnd" + type.toString());

                QDomElement fromOwnedEnd = createOwnedEnd("From", type, mApi.from(type));
		result.appendChild(fromOwnedEnd);

		// ToEnd
                createDomElementWithIdRef(result, "memberEnd", "ToEnd" + type.toString());

		if (linkType != "krneDirRelationship") {
                        QDomElement toOwnedEnd = createOwnedEnd("To", type, mApi.to(type));

			if (linkType == "ceComposition")
				toOwnedEnd.setAttribute("aggregation", "composite");
			else if (linkType == "ceAggregation")
				toOwnedEnd.setAttribute("aggregation", "shared");

			result.appendChild(toOwnedEnd);
		}
	} else if (linkType == "ceDependency"){
		result.setAttribute("xmi:type", "uml:Dependency");

		if (!visibility.isEmpty())
			result.setAttribute("visibility", visibility);

                createDomElementWithIdRef(result, "supplier", mApi.to(type).toString());
                createDomElementWithIdRef(result, "client", mApi.from(type).toString());
	} else
		result = QDomElement();

	return result;
}

void XmiHandler::serializeLinkBodies(QDomElement &parent, NewType const &type)
{
        foreach (NewType const type, mApi.incomingLinks(type)) {
                QDomElement link = serializeLink(type);
		if (!link.isNull())
			parent.appendChild(link);
	}
}

QDomElement XmiHandler::createMultiplicityNode(QString const &tagName, NewType const &type, QString const &value)
{
        QDomElement result = createDomElement(tagName, tagName + "To" + type.toString(), "uml:LiteralString");
	result.setAttribute("visibility", "public");
	result.setAttribute("value", value);
	return result;
}

void XmiHandler::serializeMultiplicity(QDomElement &parent, NewType const &type, QString const &multiplicity)
{
	if (!multiplicity.isEmpty()) {
		QString valueLower;
		QString valueUpper;

		if (multiplicity == "1..*") {
			valueLower = "1";
			valueUpper = "*";
		} else if (multiplicity == "0..1") {
			valueLower = "0";
			valueUpper = "1";
		} else if (multiplicity == "1") {
			valueLower = "1";
			valueUpper = "1";
		} else if (multiplicity == "*") {
			valueLower = "*";
			valueUpper = "*";
		}

		if (!valueLower.isEmpty() && !valueUpper.isEmpty()) {
                        parent.appendChild(createMultiplicityNode("lowerValue", type, valueLower));
                        parent.appendChild(createMultiplicityNode("upperValue", type, valueUpper));
		}
	}
}

QDomElement XmiHandler::createPrimitiveType(QString const &typeName)
{
	QDomElement result = createDomElement("ownedMember", typeName, "uml:PrimitiveType");
	result.setAttribute("xmi:name", typeName);
	result.setAttribute("xmi:visibility", "public");
	return result;
}

void XmiHandler::initPrimitiveTypes(QDomElement &parent)
{
	parent.appendChild(createPrimitiveType("int"));
	parent.appendChild(createPrimitiveType("float"));
	parent.appendChild(createPrimitiveType("double"));

	parent.appendChild(createPrimitiveType("char"));
	parent.appendChild(createPrimitiveType("boolean"));
	parent.appendChild(createPrimitiveType("byte"));
}

QDomElement XmiHandler::createDomElement(QString const &tagName, QString const &typeString)
{
	QDomElement result = mDocument.createElement(tagName);
        result.setAttribute("xmi:type", typeString);
        result.setAttribute("xmi:uuid", typeString);
	return result;
}

QDomElement XmiHandler::createDomElement(QString const &tagName, QString const &idString, QString const &typeString)
{
        QDomElement result = createDomElement(tagName, idString);
        result.setAttribute("xmi:type", typeString);
	return result;
}

QDomElement XmiHandler::createDomElementWithIdRef(QString const &tagName, QString const &idRef)
{
	QDomElement result = mDocument.createElement(tagName);
	result.setAttribute("xmi:idref", idRef);
	return result;
}

void XmiHandler::createDomElementWithIdRef(QDomElement &parent, QString const &tagName, QString const &idRef)
{
	QDomElement result = createDomElementWithIdRef(tagName, idRef);
	parent.appendChild(result);
}

bool XmiHandler::isTypeSuitable(QString const &typeString) const
{
        return typeString == "int" || typeString == "float" || typeString == "double" || typeString == "boolean" || typeString == "char" || typeString == "byte";
}

bool XmiHandler::isVisibilitySuitable(QString const &visibility) const
{
	return visibility == "public" || visibility == "private" || visibility == "protected" || visibility == "package";
}

void XmiHandler::addError(QString const &errorText)
{
	mErrorText += errorText + "\n";
}
