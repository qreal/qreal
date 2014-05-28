#include "generator.h"
#include "D:/qrealGen/qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace generators::qUbiqPresentationGenerator;
using namespace qReal;
using namespace utils;

/// Generation target file
QString const templateDir = "./templates/qUbiqPresentationGenerator";
QString const qUbiqApp_config = "qUbiqApp.config";
QString const qUbiqCSProject_csproj = "qUbiqCSProject.csproj";
QString const qUbiqForms_cs = "qUbiqForms.cs";
QString const qUbiqVariables_cs = "qUbiqVariables.cs";


Generator::Generator(QString const &outputDirPath
		, QString const &programName
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QMap<QString, QString> marksProperty
		)
	: AbstractGenerator(templateDir, QString(outputDirPath + "/" + programName).replace("\\", "/"), logicalModel, errorReporter)
	, mProgramName(programName)
{
	foreach (QString key, marksProperty.keys()) {
		mTemplateUtils[key] = marksProperty[key];
	}
}

Generator::~Generator()
{
}

bool Generator::isCorrectedName(QString const &name)
{
	QRegExp patten;
	patten.setPattern("[A-Za-z]+([A-Za-z_0-9]*)");
	return patten.exactMatch(name);
}

void Generator::generate()
{
	mErrorReporter.clear();
	mErrorReporter.clearErrors();

	if (!isCorrectedName(mProgramName)) {
		mErrorReporter.addError("Program name is not correct");
		return;
	}
	mTemplateUtils["@@programName@@"] = mProgramName;
	QString result_Links = "";
	QString res_Links_1 = "";
	foreach (Id const &element_1, mApi.elementsByType("BeginNodeInPresentationEditor")) {
		if (!mApi.isLogicalElement(element_1)) {
			continue;
		}
		IdList list = mApi.outgoingNodes(element_1); // TODO_need only "TransitionLink"
		foreach (Id const &outElement, list) {
			if (!mApi.isLogicalElement(outElement)|| outElement.element() != "Slide") {
				continue;
			}
			QString res_Links_one_1 = mTemplateUtils["@@elements_TransitionLink_1@@"];
			res_Links_one_1.replace("##dst.name##", mApi.property(outElement, "name").toString());
			res_Links_1 += res_Links_one_1 + "\n";
		}
	}
	mTemplateUtils["@@elements_TransitionLink_BeginNodeInPresentationEditor_Slide@@"] = res_Links_1;
	result_Links += res_Links_1;

	QString res_Links_2 = "";
	foreach (Id const &element_2, mApi.elementsByType("Button")) {
		if (!mApi.isLogicalElement(element_2)) {
			continue;
		}
		IdList list = mApi.outgoingNodes(element_2); // TODO_need only "TransitionLink"
		foreach (Id const &outElement, list) {
			if (!mApi.isLogicalElement(outElement)|| outElement.element() != "Slide") {
				continue;
			}
			QString res_Links_one_2 = mTemplateUtils["@@elements_TransitionLink_2@@"];
			res_Links_one_2.replace("##src.name##", mApi.property(element_2, "name").toString());
			res_Links_one_2.replace("##dst.name##", mApi.property(outElement, "name").toString());

			res_Links_2 += res_Links_one_2 + "\n";
		}
	}
	mTemplateUtils["@@elements_TransitionLink_Button_Slide@@"] = res_Links_2;
	result_Links += res_Links_2;


	QString result = "";
	QString res_1 = "";
	foreach (Id const &element_1, mApi.elementsByType("Variable")) {
		if (!mApi.isLogicalElement(element_1)) {
			continue;
		}
		QString res_one_1 = mTemplateUtils["@@oneVariableDeclaration@@"];
		res_one_1.replace("##type##", mApi.property(element_1, "type").toString());
		res_one_1.replace("##name##", mApi.property(element_1, "name").toString());

		res_1 += res_one_1 + "\n";
	}
	mTemplateUtils["@@elements_Variable@@"] = res_1;
	result += res_1;

	QString res_2 = "";
	foreach (Id const &element_2, mApi.elementsByType("Slide")) {
		if (!mApi.isLogicalElement(element_2)) {
			continue;
		}
		mTemplateUtils["@@mainGridFilling@@"] = "";
		QString res_2_1_1 = "";
		foreach (Id const &element_2_1_1, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_1_1) || element_2_1_1.element() != "Button") {
				continue;
			}
			QString res_one_2_1_1 = mTemplateUtils["@@elements_Button_2_1_1@@"];
			res_one_2_1_1.replace("##name##", mApi.property(element_2_1_1, "name").toString());
			res_one_2_1_1.replace("##handler##", mApi.property(element_2_1_1, "handler").toString());
			res_one_2_1_1.replace("##x##", mApi.property(element_2_1_1, "x").toString());
			res_one_2_1_1.replace("##y##", mApi.property(element_2_1_1, "y").toString());

			res_2_1_1 += res_one_2_1_1 + "\n";


		}
		mTemplateUtils["@@mainGridFilling@@"] += res_2_1_1;

		QString res_2_1_2 = "";
		foreach (Id const &element_2_1_2, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_1_2) || element_2_1_2.element() != "Text") {
				continue;
			}
			QString res_one_2_1_2 = mTemplateUtils["@@elements_Text_2_1_2@@"];
			res_one_2_1_2.replace("##name##", mApi.property(element_2_1_2, "name").toString());
			res_one_2_1_2.replace("##text##", mApi.property(element_2_1_2, "text").toString());
			res_one_2_1_2.replace("##x##", mApi.property(element_2_1_2, "x").toString());
			res_one_2_1_2.replace("##y##", mApi.property(element_2_1_2, "y").toString());

			res_2_1_2 += res_one_2_1_2 + "\n";
		}
		mTemplateUtils["@@mainGridFilling@@"] += res_2_1_2;

		QString res_2_1_3 = "";
		foreach (Id const &element_2_1_3, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_1_3) || element_2_1_3.element() != "Image") {
				continue;
			}
			QString res_one_2_1_3 = mTemplateUtils["@@elements_Image_2_1_3@@"];
			res_one_2_1_3.replace("##name##", mApi.property(element_2_1_3, "name").toString());
			res_one_2_1_3.replace("##pathToImage##", mApi.property(element_2_1_3, "pathToImage").toString());
			res_one_2_1_3.replace("##x##", mApi.property(element_2_1_3, "x").toString());
			res_one_2_1_3.replace("##y##", mApi.property(element_2_1_3, "y").toString());

			res_2_1_3 += res_one_2_1_3 + "\n";
		}
		mTemplateUtils["@@mainGridFilling@@"] += res_2_1_3;

		QString res_2_1_4 = "";
		foreach (Id const &element_2_1_4, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_1_4) || element_2_1_4.element() != "List") {
				continue;
			}
			mTemplateUtils["@@listFillings@@"] = "";
			QString res_2_1_4_1_1 = "";
			foreach (Id const &element_2_1_4_1_1, mApi.children(element_2_1_4)) {
				if (!mApi.isLogicalElement(element_2_1_4_1_1) || element_2_1_4_1_1.element() != "Text") {
					continue;
				}
				QString res_one_2_1_4_1_1 = mTemplateUtils["@@elements_Text_2_1_4_1_1@@"];
				res_one_2_1_4_1_1.replace("##name##", mApi.property(element_2_1_4_1_1, "name").toString());
				res_one_2_1_4_1_1.replace("##text##", mApi.property(element_2_1_4_1_1, "text").toString());

				res_2_1_4_1_1 += res_one_2_1_4_1_1 + "\n";
			}
			mTemplateUtils["@@listFillings@@"] += res_2_1_4_1_1;

			QString res_2_1_4_1_2 = "";
			foreach (Id const &element_2_1_4_1_2, mApi.children(element_2_1_4)) {
				if (!mApi.isLogicalElement(element_2_1_4_1_2) || element_2_1_4_1_2.element() != "Image") {
					continue;
				}
				QString res_one_2_1_4_1_2 = mTemplateUtils["@@elements_Image_2_1_4_1_2@@"];
				res_one_2_1_4_1_2.replace("##name##", mApi.property(element_2_1_4_1_2, "name").toString());
				res_one_2_1_4_1_2.replace("##pathToImage##", mApi.property(element_2_1_4_1_2, "pathToImage").toString());

				res_2_1_4_1_2 += res_one_2_1_4_1_2 + "\n";
			}
			mTemplateUtils["@@listFillings@@"] += res_2_1_4_1_2;

			QString res_one_2_1_4 = mTemplateUtils["@@elements_List_2_1_4@@"];
			res_one_2_1_4.replace("@@listFillings@@", mTemplateUtils["@@listFillings@@"]);
			res_one_2_1_4.replace("##x##", mApi.property(element_2_1_4, "x").toString());
			res_one_2_1_4.replace("##y##", mApi.property(element_2_1_4, "y").toString());

			res_2_1_4 += res_one_2_1_4 + "\n";
		}
		mTemplateUtils["@@mainGridFilling@@"] += res_2_1_4;

		QString res_2_1_5 = "";
		foreach (Id const &element_2_1_5, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_1_5) || element_2_1_5.element() != "Grid") {
				continue;
			}
			mTemplateUtils["@@gridItemCreation@@"] = "";
			QString res_2_1_5_1_1 = "";
			foreach (Id const &element_2_1_5_1_1, mApi.children(element_2_1_5)) {
				if (!mApi.isLogicalElement(element_2_1_5_1_1) || element_2_1_5_1_1.element() != "Button") {
					continue;
				}
				QString res_one_2_1_5_1_1 = mTemplateUtils["@@elements_Button_2_1_5_1_1@@"];
				res_one_2_1_5_1_1.replace("##name##", mApi.property(element_2_1_5_1_1, "name").toString());
				res_one_2_1_5_1_1.replace("##handler##", mApi.property(element_2_1_5_1_1, "handler").toString());

				res_2_1_5_1_1 += res_one_2_1_5_1_1 + "\n";
			}
			mTemplateUtils["@@gridItemCreation@@"] += res_2_1_5_1_1;

			QString res_one_2_1_5 = mTemplateUtils["@@elements_Grid_2_1_5@@"];
			res_one_2_1_5.replace("@@gridItemCreation@@", mTemplateUtils["@@gridItemCreation@@"]);
			res_one_2_1_5.replace("##w##", mApi.property(element_2_1_5, "w").toString());
			res_one_2_1_5.replace("##h##", mApi.property(element_2_1_5, "h").toString());
			res_one_2_1_5.replace("##x##", mApi.property(element_2_1_5, "x").toString());
			res_one_2_1_5.replace("##y##", mApi.property(element_2_1_5, "y").toString());

			res_2_1_5 += res_one_2_1_5 + "\n";
		}
		mTemplateUtils["@@mainGridFilling@@"] += res_2_1_5;

		QString res_2_1_6 = "";
		foreach (Id const &element_2_1_6, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_1_6) || element_2_1_6.element() != "ExitButton") {
				continue;
			}
			QString res_one_2_1_6 = mTemplateUtils["@@elements_ExitButton_2_1_6@@"];
			res_one_2_1_6.replace("##name##", mApi.property(element_2_1_6, "name").toString());
			res_one_2_1_6.replace("##x##", mApi.property(element_2_1_6, "x").toString());
			res_one_2_1_6.replace("##y##", mApi.property(element_2_1_6, "y").toString());

			res_2_1_6 += res_one_2_1_6 + "\n";
		}
		mTemplateUtils["@@mainGridFilling@@"] += res_2_1_6;

		mTemplateUtils["@@handlerDescriptions@@"] = "";
		QString res_2_2_1 = "";
		foreach (Id const &element_2_2_1, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_2_1) || element_2_2_1.element() != "ExitButton") {
				continue;
			}
			QString res_one_2_2_1 = mTemplateUtils["@@elements_ExitButton_2_2_1@@"];
			res_one_2_2_1.replace("##name##", mApi.property(element_2_2_1, "name").toString());

			res_2_2_1 += res_one_2_2_1 + "\n";
		}
		mTemplateUtils["@@handlerDescriptions@@"] += res_2_2_1;

		QString res_2_2_2 = "";
		foreach (Id const &element_2_2_2, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_2_2) || element_2_2_2.element() != "Button") {
				continue;
			}
			QString res_one_2_2_2 = mTemplateUtils["@@elements_Button_2_2_2@@"];
			res_one_2_2_2.replace("##handler##", mApi.property(element_2_2_2, "handler").toString());

			res_2_2_2 += res_one_2_2_2 + "\n";
		}
		mTemplateUtils["@@handlerDescriptions@@"] += res_2_2_2;

		QString res_2_2_3 = "";
		foreach (Id const &element_2_2_3, mApi.children(element_2)) {
			if (!mApi.isLogicalElement(element_2_2_3) || element_2_2_3.element() != "Grid") {
				continue;
			}
			mTemplateUtils["@@gridButtonsHandler@@"] = "";
			QString res_2_2_3_1_1 = "";
			foreach (Id const &element_2_2_3_1_1, mApi.children(element_2_2_3)) {
				if (!mApi.isLogicalElement(element_2_2_3_1_1) || element_2_2_3_1_1.element() != "Button") {
					continue;
				}
				QString res_one_2_2_3_1_1 = mTemplateUtils["@@elements_Button_2_2_3_1_1@@"];
				res_one_2_2_3_1_1.replace("##handler##", mApi.property(element_2_2_3_1_1, "handler").toString());

				res_2_2_3_1_1 += res_one_2_2_3_1_1 + "\n";
			}
			mTemplateUtils["@@gridButtonsHandler@@"] += res_2_2_3_1_1;

			QString res_one_2_2_3 = mTemplateUtils["@@elements_Grid_2_2_3@@"];
			res_one_2_2_3.replace("@@gridButtonsHandler@@", mTemplateUtils["@@gridButtonsHandler@@"]);

			res_2_2_3 += res_one_2_2_3 + "\n";
		}
		mTemplateUtils["@@handlerDescriptions@@"] += res_2_2_3;

		QString res_one_2 = mTemplateUtils["@@elements_Slide_2@@"];
		res_one_2.replace("@@mainGridFilling@@", mTemplateUtils["@@mainGridFilling@@"]);
		res_one_2.replace("@@handlerDescriptions@@", mTemplateUtils["@@handlerDescriptions@@"]);
		res_one_2.replace("##name##", mApi.property(element_2, "name").toString());
		res_one_2.replace("##width##", mApi.property(element_2, "width").toString());
		res_one_2.replace("##height##", mApi.property(element_2, "height").toString());

		res_2 += res_one_2 + "\n";
	}
	mTemplateUtils["@@elements_Slide@@"] = res_2;
	result += res_2;

	mTemplateUtils["@@elements_ALL@@"] = result + "\n" + result_Links;

	QString result_qUbiqVariables_cs = "";
	loadTemplateFromFile(qUbiqVariables_cs, result_qUbiqVariables_cs);
	result_qUbiqVariables_cs.replace("@@programName@@", mTemplateUtils["@@programName@@"]);
	result_qUbiqVariables_cs.replace("@@elements_Variable@@", mTemplateUtils["@@elements_Variable@@"]);
	saveOutputFile("qUbiqVariables.cs", result_qUbiqVariables_cs);

	QString result_qUbiqApp_config = "";
	loadTemplateFromFile(qUbiqApp_config, result_qUbiqApp_config);
	saveOutputFile("qUbiqApp.config", result_qUbiqApp_config);

	QString result_qUbiqCSProject_csproj = "";
	loadTemplateFromFile(qUbiqCSProject_csproj, result_qUbiqCSProject_csproj);
	result_qUbiqCSProject_csproj.replace("@@programName@@", mTemplateUtils["@@programName@@"]);
	saveOutputFile("qUbiqCSProject.csproj", result_qUbiqCSProject_csproj);

	QString result_qUbiqForms_cs = "";
	loadTemplateFromFile(qUbiqForms_cs, result_qUbiqForms_cs);
	result_qUbiqForms_cs.replace("@@programName@@", mTemplateUtils["@@programName@@"]);
	result_qUbiqForms_cs.replace("@@elements_TransitionLink_BeginNodeInPresentationEditor_Slide@@", mTemplateUtils["@@elements_TransitionLink_BeginNodeInPresentationEditor_Slide@@"]);
	result_qUbiqForms_cs.replace("@@elements_TransitionLink_Button_Slide@@", mTemplateUtils["@@elements_TransitionLink_Button_Slide@@"]);
	result_qUbiqForms_cs.replace("@@elements_Slide@@", mTemplateUtils["@@elements_Slide@@"]);
	saveOutputFile("qUbiqForms.cs", result_qUbiqForms_cs);
}
