#include "JavaHandler.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/definitions.h"
#include "../../client/repoApi.h"

using namespace qReal;
using namespace generators;

JavaHandler::JavaHandler(client::RepoApi const &api)
	: mApi(api)
{
}

QString JavaHandler::exportToJava(QString const &pathToFile)
{
	mErrorText = "";

	QFile file(pathToFile);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return "";

	QTextStream out(&file);

	Id repoId = ROOT_ID;

	out << serializeChildren(repoId);

	IdList rootDiagrams = mApi.children(repoId);

	foreach (Id const typeDiagram, rootDiagrams) {
		out << serializeChildren(typeDiagram);
	}

	qDebug() << "Done.";
	return mErrorText;
}

QString JavaHandler::serializeChildren(Id const &idParent)
{
	QString result = "";
	IdList childElems = mApi.children(idParent);

	foreach (Id const id, childElems) {
		result += serializeObject(id, idParent);
	}

	if (idParent != ROOT_ID) {
		IdList linksOut = mApi.outcomingLinks(idParent);

		foreach (Id const id, linksOut) {
			result += serializeOutcomingLink(id);
		}

		IdList linksIn = mApi.incomingLinks(idParent);

		foreach (Id const id, linksIn) {
			result += serializeIncomingLink(id);
		}
	}

	return result;
}

QString JavaHandler::serializeObject(Id const &id, Id const &parentId)
{
	QString result = "";

	QString additionalParams = "";

	QString const objectType = mApi.typeName(id);
	QString const parentType = mApi.typeName(parentId);

	if (objectType == "krnnDiagram") {
//	    result += serializeChildren(id);
	}

	// class diagramm

	else if (objectType == "cnClass") {
	    QString visibility = getVisibility(id);
	    result += visibility + "class " + mApi.name(id) + " {" + "\n";
	    result += serializeChildren(id);
	    result += "}\n";
	} else if (objectType == "cnClassView") {
//	    to do someting
	} else if (objectType == "cnClassMethod") {
		if (parentType == "cnClass") {
		    QString visibility = getVisibility(id);
		    QString type = getType(id);
//		    QString operationFactors = getOperationFactors(id);
//		    todo: исправить генерацию параметров
		    result += visibility + type  + mApi.name(id) + "();" + "\n";
		} else {
			this->addError("unable to serrialize object " + objectType + " with id: " + id.toString() + ". Move it inside some cnClass");
		}
	} else if (objectType == "cnClassField") {
		if (parentType == "cnClass"){
		    QString visibility = getVisibility(id);
		    QString type = getType(id);
		    QString defaultValue = getDefaultValue(id);
		    result += visibility + type + mApi.name(id);
		    if (defaultValue != ""){
			result += " " + defaultValue;
		    }
		    result += ";\n";
		} else {
			addError("unable to serialize object " + objectType + " with id: " + id.toString() + ". Move it inside some cnClass");
		}
	}

//	//use case diagram
//
//	else if (objectType == "uscnActor") {
//		typeOfTag = "ownedMember";
//		typeOfElem = "uml:Actor";
//	} else if (objectType == "uscnUseCase") {
//		typeOfTag = "ownedMember";
//		typeOfElem = "uml:UseCase";
//	}
//
//	// sequence diagramm
//
//	else if (objectType == "sqnnInteraction") {
//		typeOfTag = "ownedMember";
//		typeOfElem = "uml:Interaction";
//	} else if (objectType == "sqnnSLifeline") {
//		typeOfTag = "lifeline";
//		typeOfElem = "uml:Lifeline";
//	}


//	result += "<" + typeOfTag + " xmi:type=\"" + typeOfElem + "\" xmi:id=\""
//		+ id.toString() + "\" xmi:uuid=\"" + id.toString() + "\" name=\"" + mApi.name(id)
//		+ "\" " + additionalParams + ">" + "\n";
//	result += serializeChildren(id);
//	result += "</" + typeOfTag + ">" + "\n";
//	result += serializeLinkBodies(id);

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

QString JavaHandler::getType(Id const &id)
{
    QString result = "";

    QString const objectType = mApi.typeName(id);

    if (mApi.hasProperty(id, "type")) {
	QString type = mApi.stringProperty(id, "type");

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

QString JavaHandler::getOperationFactors(Id const &id)
{
    QString result = "";

    QString const objectType = mApi.typeName(id);

//    todo

    return result;
}

QString JavaHandler::getDefaultValue(Id const &id)
{
    QString result = "";

    QString const objectType = mApi.typeName(id);

    if (mApi.hasProperty(id, "defaultValue")) {
	QString defaultValue = mApi.stringProperty(id, "defaultValue");

//	if (isTypeSuitable(defaultValue)) {
//	проверка, что типы default и поля совпадают
		result = defaultValue;
//	} else {
//		addError("object " + objectType + " with id " + id.toString() + " has invalid default value: " + defaultValue);
//    	}
    }

    return result;
}

QString JavaHandler::serializeOutcomingLink(Id const &id)
{
	QString result = "";
	QString linkType = mApi.typeName(id);

	// kernel diagram
	// TODO: � љ� ѕС€� ј� °СЂ� Ѕ� °СЏ � є� ѕ� ї� ё� ї� °СЃС‚� ° СЃ � ё� ґС€� Ѕ� ё� є� °� ј� ё
	if (linkType == "krnePackageImport") {
		result += "<packageImport xmi:type=\"uml:PackageImport\" xmi:id=\""
			+ id.toString() + "\" xmi:uuid=\"" + id.toString()
			+ "\"  importedPackage=\"" + mApi.to(id).toString() + "\" />";
	} else if (linkType == "krneElementImport") {
		result += "<elementImport xmi:type=\"uml:ElementImport\" xmi:id=\""
			+ id.toString() + "\" xmi:uuid=\"" + id.toString()
			+ "\"  importedElement=\"" + mApi.to(id).toString() + "\" />";
	} else if (linkType == "krneGeneralization") {
		result += "<generalization xmi:type=\"uml:Generalization\" xmi:id=\""
			+ id.toString() + "\" xmi:uuid=\"" + id.toString()
			+ "\" general=\"" + mApi.to(id).toString() +  "\"/>";
	} else if (linkType == "krneDirRelationship") {
		result = result + "<ownedAttribute xmi:type=\"uml:Property\" xmi:id=\""
			+ "ToEnd" + id.toString() + "\" xmi:uuid=\"" + "ToEnd" + id.toString()
			+ "\" visibility=\"protected\" type=\"" + mApi.to(id).toString() + "\">" + "\n";

		QString toMult = mApi.stringProperty(id, "toMultiplicity");
		result = result + serializeMultiplicity(id, toMult);

		result = result + "<association xmi:idref=\"" + id.toString() +  "\"/>" + "\n";
		result = result + "</ownedAttribute>" + "\n";
	}

	// class diagram

	else if (linkType == "ceDependency") {
		result += "<clientDependency xmi:idref=\"" + id.toString() + "\"/>" + "\n";
	}

	// use case diagram

	else if (linkType == "uscaExtend") {
		result += "<extend xmi:type=\"uml:Extend\" xmi:id=\"" + id.toString()
			+ "\" xmi:uuid=\"" + id.toString() + "\" extendedCase=\""
			+ mApi.to(id).toString() + "\">" + "\n";
		result += "<extension xmi:idref=\"" + mApi.from(id).toString() + "\"/>" + "\n";
		result += "</extend>\n";
	} else if (linkType == "uscaInclude") {
		result += "<include xmi:type=\"uml:Include\" xmi:id=\"" + id.toString() +
			"\" xmi:uuid=\"" + id.toString() + "\" addition=\"" + mApi.to(id).toString() + "\"/>" + "\n";
	}

	return result;
}

QString JavaHandler::serializeIncomingLink(Id const &id)
{
	QString result = "";

	if (mApi.typeName(id) == "ceDependency") {
		result += "<supplierDependency xmi:idref=\"" + id.toString()  + "\"/>";
	}

	return result;
}

QString JavaHandler::serializeLink(Id const &id)
{
	QString result = "";
	QString additionalParams = "";
	QString linkType = mApi.typeName(id);

	if (!mApi.stringProperty(id, "visibility").isEmpty()) {
		additionalParams += "visibility=\"" + mApi.property(id, "visibility").toString() + "\"";
	}

	if (linkType == "ceComposition" || linkType == "ceAggregation"
		|| linkType == "krneRelationship" || linkType == "ceRelation"
		|| linkType == "krneDirRelationship")
	{
		QString aggregation = "";

		if (linkType == "ceComposition") {
			aggregation = "aggregation=\"composite\"";
		} else if (linkType == "ceAggregation") {
			aggregation = "aggregation=\"shared\"";
		}

		result += "<ownedMember xmi:type=\"uml:Association\" xmi:id=\""
			+ id.toString() + "\" xmi:uuid=\"" + id.toString() + "\" " + additionalParams + ">" + "\n";

		// FromEnd

		result += "<memberEnd xmi:idref=\"FromEnd" + id.toString() + "\"/>\n";
		result += "<ownedEnd xmi:type=\"uml:Property\" xmi:id=\""
			+ QString("FromEnd") + id.toString() + "\" xmi:uuid=\"FromEnd" + id.toString()
			+ "\" visibility=\"protected\" type=\"" + mApi.from(id).toString() + "\">\n";
		result += "<association xmi:idref=\"" + id.toString() +  "\"/>\n";

		QString fromMult = mApi.stringProperty(id, "fromMultiplicity");
		result += serializeMultiplicity(id, fromMult);

		result += "<association xmi:idref=\"" + id.toString() +  "\"/>\n";
		result += "</ownedEnd>\n";

		// ToEnd

		result += "<memberEnd xmi:idref=\"ToEnd" + id.toString() + "\"/>\n";

		if (linkType != "krneDirRelationship") {

			result += "<ownedEnd xmi:type=\"uml:Property\" xmi:id=\"ToEnd"
				+ id.toString() + "\" xmi:uuid=\"ToEnd" + id.toString()
				+ "\" visibility=\"protected\" " + aggregation + " type=\""
				+ mApi.to(id).toString() + "\">" + "\n";

			QString toMult = mApi.stringProperty(id, "toMultiplicity");
			result += serializeMultiplicity(id, toMult);

			result += "<association xmi:idref=\"" + id.toString() +  "\"/>" + "\n";
			result += "</ownedEnd>\n";
		}

		result += "</ownedMember>\n";
	} else if (linkType == "ceDependency"){
		result += "<ownedMember xmi:type=\"uml:Dependency\" xmi:id=\""
			+ id.toString() + "\" xmi:uuid=\"" + id.toString() + "\" "
			+ additionalParams + ">" + "\n";
		result = "<supplier xmi:idref=\"" + mApi.to(id).toString() + "\"/>\n";
		result = "<client xmi:idref=\"" + mApi.from(id).toString() + "\"/>\n";
		result = "</ownedMember>\n";
	}

	return result;
}

QString JavaHandler::serializeLinkBodies(Id const &id)
{
	QString result = "";

	foreach (Id const id, mApi.incomingLinks(id)) {
		result += serializeLink(id);
	}

	return result;
}

QString JavaHandler::serializeMultiplicity(Id const &id, QString const &multiplicity) const
{
	QString result = "";
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
			result += QString("<lowerValue xmi:type=\"uml:LiteralString\" xmi:id=\"")
					 + "loverValueTo" + id.toString() + "\" xmi:uuid=\"loverValueTo" + id.toString()
					 + "\" visibility=\"public\" value=\"" + valueLower + "\"/>\n";
			result = QString("<upperValue xmi:type=\"uml:LiteralString\" xmi:id=\"")
					 + "upperValueTo" + id.toString() + "\" xmi:uuid=\"" + "upperValueTo" + id.toString()
					 + "\" visibility=\"public\" value=\"" + valueUpper + "\"/>\n";
		}
	}

	return result;
}

bool JavaHandler::isTypeSuitable(QString const &type) const
{
//    Добавить в рассмотрение все классы-оболочки и пользователем созданные классы
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
