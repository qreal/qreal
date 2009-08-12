#include "xmiHandler.h"
#include "realrepoclient.h"
#include "dbg.h"

#include <QtGui>

XmiHandler::XmiHandler (QString const &addr, const int port)
{
	client = new RealRepoClient(addr, port, 0);
}

QString XmiHandler::exportToXmi(QString const &pathToFile)
{
	errorText = "";

	QFile file(pathToFile);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return "";

	QTextStream out(&file);

	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";
	out << "<xmi:XMI xmlns:xmi=\"http://schema.omg.org/spec/XMI/2.1\" xmlns:uml=\"http://schema.omg.org/spec/UML/2.0\" xmi:version=\"2.1\">" << "\n";

	out << "\n";
	out << "\n";

	//  --------------  header --------------- //
	out << "<xmi:Documentation exporter=\"QReal\" />" << "\n";

	out << "\n";
	out << "\n";

	//  --------------  end of header --------------- //

	//  --------------  content --------------- //
	QString repoId = "repoRoot";

	out << "<uml:Package xmi:id=\"" + repoId + "\" xmi:uuid=\"" + repoId + "\" name=\"RootDiagram\">" << "\n";

	out << this->initPrimitiveTypes();

	qRealTypes::RealObject root = client->getObjectById(repoId);
	qRealTypes::IdTypeList rootDiagrams = root.getChildElements();

	foreach (const QString typeDiagram, rootDiagrams) {
		out << this->serializeChildren(typeDiagram);
	}

	out << "</uml:Package>" << "\n";
	//  --------------  end of content  --------------- //

	out << "</xmi:XMI>" << "\n";
	qDebug() << "Done.";
	return errorText;
}

QString XmiHandler::serializeChildren(QString const &idParent)
{
	QString result = "";
	qRealTypes::RealObject object = client->getObjectById(idParent);
	qRealTypes::IdTypeList childElems = object.getChildElements();

	if (childElems.size() > 0) {
		foreach (const QString id, childElems) {
			result = result + this->serializeObject(id, idParent);
		}
	}

	qRealTypes::IdTypeList linksOut = object.getOutcomingLinks();

	if (linksOut.size() > 0) {
		foreach (const QString id, linksOut) {
			result = result + this->serializeOutcomingLink(id);
		}
	}

	qRealTypes::IdTypeList linksIn = object.getIncomingLinks();

	if (linksIn.size() > 0) {
		foreach (const QString id, linksIn) {
			result = result + this->serializeIncomingLink(id);
		}
	}

	return result;
}

QString XmiHandler::serializeObject(QString const &id, QString const &parentId)
{
	QString result = "";
	qRealTypes::RealObject object = client->getObjectById(id);
	qRealTypes::RealObject parent = client->getObjectById(parentId);

	QString typeOfElem = "";
	QString typeOfTag = "";
	QString additionalParams = "";

	if (object.getTypeId() == "krnnDiagram") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Package";
	}

	// class diagramm

	else if (object.getTypeId() == "cnClass") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Class";
	} else if (object.getTypeId() == "cnClassView") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Package";
	}else if (object.getTypeId() == "cnClassMethod") {
		if (parent.getTypeId() == "cnClass") {
			typeOfTag = "ownedOperation";
			typeOfElem = "uml:Operation";
		} else {
			this->addError("unable to serrialize object " + object.getTypeId() + " with id: " + object.getId() + ". Move it inside some cnClass");
		}
	} else if (object.getTypeId() == "cnClassField") {
		if (parent.getTypeId() == "cnClass"){
			typeOfTag = "ownedAttribute";
			typeOfElem = "uml:Property";
		} else {
			this->addError("unable to serrialize object " + object.getTypeId() + " with id: " + object.getId() + ". Move it inside some cnClass");
		}
	}

	//use case diagram

	else if (object.getTypeId() == "uscnActor") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Actor";
	} else if (object.getTypeId() == "uscnUseCase") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:UseCase";
	}

	// sequence diagramm

	else if (object.getTypeId() == "sqnnInteraction") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Interaction";
	} else if (object.getTypeId() == "sqnnSLifeline") {
		typeOfTag = "lifeline";
		typeOfElem = "uml:Lifeline";
	}

	// additional params
	if (object.getProperty("visibility").size() > 0) {
		QString visibility = object.getProperty("visibility");
		if (visibility == "+"){
		    visibility = "public";
		} else if (visibility == "-"){
		    visibility = "private";
		} else if (visibility == "#"){
		    visibility = "protected";
		} else if (visibility == "~"){
		    visibility = "package";
		}

		if (this->isVisibilitySuitable(visibility)) {
			additionalParams = additionalParams + "visibility=\"" + visibility + "\"";
		} else {
			this->addError("object " + object.getTypeId() + " with id  " + object.getId() + " has invalid visibility property: " + visibility);
		}
	}

	if (object.getProperty("type").size() > 0){
		QString type = object.getProperty("type");

		if (this->isTypeSuitable(type)) {
			additionalParams = additionalParams + "type=\"" + type + "\"";
		}else {
			this->addError("object " + object.getTypeId() + " with id " + object.getId() + " has invalid type: " + type);
		}
	}

	if (typeOfElem.size() != 0 && typeOfTag.size() != 0) {
		result = result + "<" + typeOfTag + " xmi:type=\"" + typeOfElem + "\" xmi:id=\""
				 + id + "\" xmi:uuid=\"" + id + "\" name=\"" + object.getName()
				 + "\" " + additionalParams + ">" + "\n";
		result = result + this->serializeChildren(id);
		result = result + "</" + typeOfTag + ">" + "\n";
		result = result + this->serializeLinkBodies(id);
	}
	return result;
}

QString XmiHandler::serializeOutcomingLink(QString const &id)
{
	QString result = "";
	try {
		qRealTypes::RealLink link = client->getLinkById(id);

		// kernel diagram
		if (link.getTypeId() == "krnePackageImport") {
			result = result + "<packageImport xmi:type=\"uml:PackageImport\" xmi:id=\""
					 + link.getId() + "\" xmi:uuid=\"" + link.getId()
					 + "\"  importedPackage=\"" + link.getToId() + "\" />";
		} else if (link.getTypeId() == "krneElementImport") {
			result = result + "<elementImport xmi:type=\"uml:ElementImport\" xmi:id=\""
					 + link.getId() + "\" xmi:uuid=\"" + link.getId()
					 + "\"  importedElement=\"" + link.getToId() + "\" />";
		} else if (link.getTypeId() == "krneGeneralization") {
			result = result + "<generalization xmi:type=\"uml:Generalization\" xmi:id=\""
					 + link.getId() + "\" xmi:uuid=\"" + link.getId()
					 + "\" general=\"" + link.getToId() +  "\"/>";
		} else if (link.getTypeId() == "krneDirRelationship") {
			result = result + "<ownedAttribute xmi:type=\"uml:Property\" xmi:id=\""
					 + "ToEnd" + id + "\" xmi:uuid=\"" + "ToEnd" + id
					 + "\" visibility=\"protected\" type=\"" + link.getToId() + "\">" + "\n";

			QString toMult = link.getProperty("toMultiplicity");
			result = result + serializeMultiplicity(id, toMult);

			result = result + "<association xmi:idref=\"" + id +  "\"/>" + "\n";
			result = result + "</ownedAttribute>" + "\n";
		}

		// class diagram

		else if (link.getTypeId() == "ceDependency") {
			result = result +
					 "<clientDependency xmi:idref=\"" + link.getId()  + "\"/>" + "\n";
		}

		// use case diagram

		else if (link.getTypeId() == "uscaExtend") {
			result = result + "<extend xmi:type=\"uml:Extend\" xmi:id=\"" + id
					 + "\" xmi:uuid=\"" + id + "\" extendedCase=\"" + link.getToId() + "\">" + "\n";
			result = result +  "<extension xmi:idref=\"" + link.getFromId() + "\"/>" + "\n";
			result = result + "</extend>" + "\n";
		} else if (link.getTypeId() == "uscaInclude"){
			result = result + "<include xmi:type=\"uml:Include\" xmi:id=\"" + id +
					 "\" xmi:uuid=\"" + id + "\" addition=\"" + link.getToId() + "\"/>" + "\n";
		}
	}
	catch (...) {

	}

	return result;
}

QString XmiHandler::serializeIncomingLink(QString const &id)
{
	QString result = "";
	try {
		qRealTypes::RealLink link = client->getLinkById(id);

		if (link.getTypeId() == "ceDependency") {
			result = result +
					 "<supplierDependency xmi:idref=\"" + link.getId()  + "\"/>";
		}
	}
	catch (...) {

	}

	return result;
}

QString XmiHandler::serializeLink(QString const &id)
{
	QString result = "";
	try {
		qRealTypes::RealLink link = client->getLinkById(id);
		QString additionalParams = "";

		if (link.getProperty("visibility").size() > 0) {
			additionalParams = additionalParams + "visibility=\"" + link.getProperty("visibility") + "\"";
		}

		if (link.getTypeId() == "ceComposition" || link.getTypeId() == "ceAggregation"
			|| link.getTypeId() == "krneRelationship" || link.getTypeId() == "ceRelation"
			|| link.getTypeId() == "krneDirRelationship")
		{
			QString aggregation = "";

			if (link.getTypeId() == "ceComposition") {
				aggregation = "aggregation=\"composite\"";
			} else if (link.getTypeId() == "ceAggregation") {
				aggregation = "aggregation=\"shared\"";
			}

			result = result + "<ownedMember xmi:type=\"uml:Association\" xmi:id=\""
					 + id + "\" xmi:uuid=\"" + id + "\" " + additionalParams + ">" + "\n";

			// FromEnd

			result = result + "<memberEnd xmi:idref=\"" + "FromEnd" + id + "\"/>"  + "\n";
			result = result + "<ownedEnd xmi:type=\"uml:Property\" xmi:id=\""
					 + "FromEnd" + id + "\" xmi:uuid=\"" + "FromEnd" + id
					 + "\" visibility=\"protected\" type=\"" + link.getFromId() + "\">" + "\n";
			result = result + "<association xmi:idref=\"" + id +  "\"/>" + "\n";

			QString fromMult = link.getProperty("fromMultiplicity");
			result = result + this->serializeMultiplicity(id, fromMult);

			result = result + "<association xmi:idref=\"" + id +  "\"/>" + "\n";
			result = result + "</ownedEnd>" + "\n";

			// ToEnd

			result = result + "<memberEnd xmi:idref=\"" + "ToEnd" + id + "\"/>"  + "\n";

			if (link.getTypeId() != "krneDirRelationship") {

				result = result + "<ownedEnd xmi:type=\"uml:Property\" xmi:id=\"" + "ToEnd"
						 + id + "\" xmi:uuid=\"" + "ToEnd" + id + "\" visibility=\"protected\" "
						 + aggregation + " type=\"" + link.getToId() + "\">" + "\n";

				QString toMult = link.getProperty("toMultiplicity");
				result = result + this->serializeMultiplicity(id, toMult);

				result = result + "<association xmi:idref=\"" + id +  "\"/>" + "\n";
				result = result + "</ownedEnd>" + "\n";
			}

			result = result + "</ownedMember>" + "\n";
		} else if (link.getTypeId() == "ceDependency"){
			result = result + "<ownedMember xmi:type=\"uml:Dependency\" xmi:id=\""
					 + id + "\" xmi:uuid=\"" + id + "\" " + additionalParams + ">" + "\n";
			result = result + "<supplier xmi:idref=\"" + link.getToId() + "\"/>" + "\n";
			result = result + "<client xmi:idref=\"" + link.getFromId() + "\"/>" + "\n";
			result = result + "</ownedMember>" + "\n";
		}
	}
	catch (...) {

	}

	return result;
}

QString XmiHandler::serializeLinkBodies(QString const &id)
{
	QString result = "";
	qRealTypes::RealObject object = client->getObjectById(id);

	qRealTypes::IdTypeList links = object.getIncomingLinks();

	if (links.size() > 0) {
		foreach (const QString id, links) {
			result = result + this->serializeLink(id);
		}
	}
	return result;
}

QString XmiHandler::serializeMultiplicity(QString const &id, QString const &multiplicity) const
{
	QString result = "";
	if (multiplicity.size() > 0){
		QString valueLover = "";
		QString valueUpper = "";

		if (multiplicity == "1..*"){
			valueLover = "1";
			valueUpper = "*";
		} else if (multiplicity == "0..1") {
			valueLover = "0";
			valueUpper = "1";
		} else if (multiplicity == "1") {
			valueLover = "1";
			valueUpper = "1";
		} else if (multiplicity == "*") {
			valueLover = "*";
			valueUpper = "*";
		}

		if (valueLover.size() > 0 && valueUpper.size() > 0){
			result = result + "<lowerValue xmi:type=\"uml:LiteralString\" xmi:id=\""
					 + "loverValueTo" + id + "\" xmi:uuid=\"" + "loverValueTo" + id
					 + "\" visibility=\"public\" value=\"" + valueLover + "\"/>" + "\n";
			result = result + "<upperValue xmi:type=\"uml:LiteralString\" xmi:id=\""
					 + "upperValueTo" + id + "\" xmi:uuid=\"" + "upperValueTo" + id
					 + "\" visibility=\"public\" value=\"" + valueUpper + "\"/>" + "\n";
		}
	}

	return result;
}

QString XmiHandler::initPrimitiveTypes() const
{
	QString result = "";

	result = result + "<ownedMember xmi:type=\"uml:PrimitiveType\" xmi:id=\"int\" xmi:uuid=\"int\" name=\"int\" visibility=\"public\"/>" + "\n";
	result = result + "<ownedMember xmi:type=\"uml:PrimitiveType\" xmi:id=\"float\" xmi:uuid=\"float\" name=\"float\" visibility=\"public\"/>" + "\n";
	result = result + "<ownedMember xmi:type=\"uml:PrimitiveType\" xmi:id=\"double\" xmi:uuid=\"double\" name=\"double\" visibility=\"public\"/>" + "\n";

	result = result + "<ownedMember xmi:type=\"uml:PrimitiveType\" xmi:id=\"char\" xmi:uuid=\"char\" name=\"char\" visibility=\"public\"/>" + "\n";
	result = result + "<ownedMember xmi:type=\"uml:PrimitiveType\" xmi:id=\"boolean\" xmi:uuid=\"boolean\" name=\"boolean\" visibility=\"public\"/>" + "\n";
	result = result + "<ownedMember xmi:type=\"uml:PrimitiveType\" xmi:id=\"byte\" xmi:uuid=\"byte\" name=\"byte\" visibility=\"public\"/>" + "\n";

	return result;
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
	this->errorText = this->errorText + errorText + "\n";
}
