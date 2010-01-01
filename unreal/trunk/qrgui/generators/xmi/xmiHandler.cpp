#include "xmiHandler.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/definitions.h"
#include "../../../qrrepo/repoApi.h"

#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;

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
	Id repoId = ROOT_ID;
	IdList rootDiagrams = mApi.children(repoId);

	foreach (Id const typeDiagram, rootDiagrams) {
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

void XmiHandler::serializeLinks(QDomElement &parent, bool direction, Id const &idParent)
{
	IdList links = direction ? mApi.outcomingLinks(idParent) : mApi.incomingLinks(idParent);

	foreach (Id const id, links) {
		QDomElement link = direction ? serializeOutcomingLink(id) : serializeIncomingLink(id);
		if (!link.isNull())
			parent.appendChild(link);
	}
}

void XmiHandler::serializeChildren(QDomElement &parent, Id const &idParent)
{
	IdList childElems = mApi.children(idParent);

	foreach (Id const id, childElems)
		serializeObject(parent, id, idParent);

	if (idParent != ROOT_ID) {
		serializeLinks(parent, true, idParent);
		serializeLinks(parent, false, idParent);
	}
}

void XmiHandler::serializeObject(QDomElement &parent, Id const &id, Id const &parentId)
{
	typedef QPair<QString, QString> StringPair;

	QString typeOfElem = "";
	QString typeOfTag = "";
	QList<StringPair> additionalParams;

	QString const objectType = mApi.typeName(id);
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
			addError("unable to serrialize object " + objectType + " with id: " + id.toString() + ". Move it inside some cnClass");
	} else if (objectType == "cnClassField") {
		if (parentType == "cnClass"){
			typeOfTag = "ownedAttribute";
			typeOfElem = "uml:Property";
		} else
			addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". Move it inside some cnClass");
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
	if (mApi.hasProperty(id, "visibility")) {
		QString visibility = mApi.stringProperty(id, "visibility");

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
				addError("object " + objectType + " with id  " + id.toString() + " has invalid visibility property: " + visibility);
		}
	}

	if (mApi.hasProperty(id, "type")) {
		QString type = mApi.stringProperty(id, "type");

		if (!type.isEmpty()) {
			if (isTypeSuitable(type))
				additionalParams << StringPair("type", type);
			else
				addError("object " + objectType + " with id " + id.toString() + " has invalid type: " + type);
		}
	}

	if (!typeOfElem.isEmpty() && !typeOfTag.isEmpty()) {
		QDomElement object = createDomElement(typeOfTag, id.toString(), typeOfElem);
		object.setAttribute("xmi:name", mApi.name(id));

		foreach (StringPair nameValuePair, additionalParams)
			object.setAttribute(nameValuePair.first, nameValuePair.second);

		serializeChildren(object, id);

		parent.appendChild(object);
		serializeLinkBodies(parent, id);
	}
}

QDomElement XmiHandler::serializeOutcomingLink(Id const &id)
{
	QString linkType = mApi.typeName(id);
	QDomElement result;

	// kernel diagram
	if (linkType == "krnePackageImport") {
		result = createDomElement("packageImport", id.toString(), "uml:PackageImport");
		result.setAttribute("importedPackage", mApi.to(id).toString());
	} else if (linkType == "krneElementImport") {
		result = createDomElement("elementImport", id.toString(), "uml:ElementImport");
		result.setAttribute("importedElement", mApi.to(id).toString());
	} else if (linkType == "krneGeneralization") {
		result = createDomElement("generalization", id.toString(), "uml:Generalization");
		result.setAttribute("general", mApi.to(id).toString());
	} else if (linkType == "krneDirRelationship") {
		result = createDomElement("ownedAttribute", id.toString(), "uml:Property");
		result.setAttribute("visibility", "protected");
		result.setAttribute("type", mApi.to(id).toString());

		QString toMult = mApi.stringProperty(id, "toMultiplicity");
		serializeMultiplicity(result, id, toMult);

		createDomElementWithIdRef(result, "association", id.toString());
	}

	// class diagram
	else if (linkType == "ceDependency")
		result = createDomElementWithIdRef("clientDependency",  id.toString());

	// use case diagram
	else if (linkType == "uscaExtend") {
		result = createDomElement("extend", id.toString(), "uml:Extend");
		result.setAttribute("extendedCase", mApi.to(id).toString());

		createDomElementWithIdRef(result, "extension",  mApi.from(id).toString());
	} else if (linkType == "uscaInclude") {
		result = createDomElement("include", id.toString(), "uml:Include");
		result.setAttribute("addition", mApi.to(id).toString());
	}

	return result;
}

QDomElement XmiHandler::serializeIncomingLink(Id const &id)
{
	QDomElement result;
	if (mApi.typeName(id) == "ceDependency")
		result = createDomElementWithIdRef("supplierDependency", id.toString());
	return result;
}

QDomElement XmiHandler::createOwnedEnd(QString const &direction, Id const &id, Id const &target)
{
	QDomElement result = createDomElement("ownedEnd", direction + "End" + id.toString(), "uml:Property");
	result.setAttribute("visibility", "protected");
	result.setAttribute("type", target.toString());

	QDomElement endAssociation = createDomElementWithIdRef("association", id.toString());

	QString multiplicityDirection = direction;
	multiplicityDirection[0] = multiplicityDirection[0].toLower();

	QString mult = mApi.stringProperty(id, multiplicityDirection + "Multiplicity");
	serializeMultiplicity(endAssociation, id, mult);
	result.appendChild(endAssociation);

	createDomElementWithIdRef(result, "association", id.toString());
	return result;
}

QDomElement XmiHandler::serializeLink(Id const &id)
{
	QString visibility = "";
	QString linkType = mApi.typeName(id);

	QDomElement result = createDomElement("ownedMember", id.toString());

	if (!mApi.stringProperty(id, "visibility").isEmpty())
		visibility = mApi.property(id, "visibility").toString();

	if (linkType == "ceComposition" || linkType == "ceAggregation"
		|| linkType == "krneRelationship" || linkType == "ceRelation"
		|| linkType == "krneDirRelationship")
	{
		result.setAttribute("xmi:type", "uml:Association");
		if (!visibility.isEmpty())
			result.setAttribute("visibility", visibility);

		// FromEnd
		createDomElementWithIdRef(result, "memberEnd", "FromEnd" + id.toString());

		QDomElement fromOwnedEnd = createOwnedEnd("From", id, mApi.from(id));
		result.appendChild(fromOwnedEnd);

		// ToEnd
		createDomElementWithIdRef(result, "memberEnd", "ToEnd" + id.toString());

		if (linkType != "krneDirRelationship") {
			QDomElement toOwnedEnd = createOwnedEnd("To", id, mApi.to(id));

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

		createDomElementWithIdRef(result, "supplier", mApi.to(id).toString());
		createDomElementWithIdRef(result, "client", mApi.from(id).toString());
	} else
		result = QDomElement();

	return result;
}

void XmiHandler::serializeLinkBodies(QDomElement &parent, Id const &id)
{
	foreach (Id const id, mApi.incomingLinks(id)) {
		QDomElement link = serializeLink(id);
		if (!link.isNull())
			parent.appendChild(link);
	}
}

QDomElement XmiHandler::createMultiplicityNode(QString const &tagName, Id const &id, QString const &value)
{
	QDomElement result = createDomElement(tagName, tagName + "To" + id.toString(), "uml:LiteralString");
	result.setAttribute("visibility", "public");
	result.setAttribute("value", value);
	return result;
}

void XmiHandler::serializeMultiplicity(QDomElement &parent, Id const &id, QString const &multiplicity)
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
			parent.appendChild(createMultiplicityNode("lowerValue", id, valueLower));
			parent.appendChild(createMultiplicityNode("upperValue", id, valueUpper));
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

QDomElement XmiHandler::createDomElement(QString const &tagName, QString const &id)
{
	QDomElement result = mDocument.createElement(tagName);
	result.setAttribute("xmi:id", id);
	result.setAttribute("xmi:uuid", id);
	return result;
}

QDomElement XmiHandler::createDomElement(QString const &tagName, QString const &id, QString const &type)
{
	QDomElement result = createDomElement(tagName, id);
	result.setAttribute("xmi:type", type);
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

bool XmiHandler::isTypeSuitable(QString const &type) const
{
	return type == "int" || type == "float" || type == "double" || type == "boolean" || type == "char" || type == "byte";
}

bool XmiHandler::isVisibilitySuitable(QString const &visibility) const
{
	return visibility == "public" || visibility == "private" || visibility == "protected" || visibility == "package";
}

void XmiHandler::addError(QString const &errorText)
{
	mErrorText += errorText + "\n";
}
