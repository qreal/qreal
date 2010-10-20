#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QHash>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	namespace generators {

		class EditorGenerator
		{
		public:
			explicit EditorGenerator(qrRepo::RepoApi const &api);

			QHash<Id, QString> getMetamodelList();
			gui::ErrorReporter generateEditor(Id const metamodelId, QString const &pathToFile);
		private:
			void serializeObjects(QDomElement &parent, Id const &idParent);
			void createImport(QDomElement &parent, Id const &id);
			void createNode(QDomElement &parent, Id const &id);
			void createEdge(QDomElement &parent, Id const &id);
			void createEnum(QDomElement &parent, Id const &id);
			void setProperties(QDomElement &parent, Id const &id);
			void setContextMenuFields(QDomElement &parent, Id const &id);
			void setValues(QDomElement &parent, Id const &id);
			void setAssotiations(QDomElement &parent, Id const &id);
			void setUsages(QDomElement &parent, Id const &id);
			void setConnections(QDomElement &parent, Id const &id);
			void setGeneralization(QDomElement &parent, Id const &id);
			void setPossibleEdges(QDomElement &parent, Id const &id);
			void setPin(QDomElement &parent, Id const &id);
			void setStatusElement(QDomElement &parent, Id const &id, QString const &tagName, QString const &propertyName);
			void setAction(QDomElement &parent, Id const &id);
			void createDiagrams(QDomElement &parent, Id const &id);
			void setContainer(QDomElement &parent, Id const &id);
			void setContainerProperties(QDomElement &parent, Id const &id);
			void newSetConnections(QDomElement &parent, const Id &id,
					QString const &commonTagName, QString const &internalTagName, QString const &typeName);
			void ensureCorrectness (Id const &id, QDomElement element, QString const &tagName, QString const &value);
			void setBoolValuesForContainer (QString const &propertyName, QDomElement &properties, Id const &id);
			void setSizesForContainer (QString const &propertyName, QDomElement &properties, Id const &id);

			qrRepo::RepoApi const &mApi;
			QDomDocument mDocument;
			QString mErrorText;
			IdList mElements;
			QString mDiagramName;
			gui::ErrorReporter mErrorReporter;
		};
	}
}
