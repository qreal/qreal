#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../kernel/ids.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	class EditorManager;

	namespace parsers {

		class HascolParser
		{
		public:
			explicit HascolParser(qrRepo::RepoApi &api, EditorManager const &editorManager);

			void parse(QStringList const &files);
		private:
			Id mImportedDiagramId;

			qrRepo::RepoApi &mApi;
			EditorManager const &mEditorManager;

			Id addElement(Id const &parent, Id const &elementType, QString const &name);
			void parseFile(QString const &fileName);
			void parseProcess(QDomElement const &element);
			void parsePorts(QDomNodeList const &ports, QString const &direction, Id const &parent);

			void doLayout();
			void doPortsLayout(Id const &parent);
			void doLayoutForPortsType(Id const &parent, unsigned margin, QString const &direction, unsigned count);
		};
	}

}
