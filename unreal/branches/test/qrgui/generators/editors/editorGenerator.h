#pragma once

#include <QDomDocument>

#include "../../../qrrepo/repoApi.h"

namespace qReal {

	namespace generators {

		class EditorGenerator {
		public:
			EditorGenerator(qrRepo::RepoApi const &api);
			void generate(Id const &editor);
		private:
			QDomElement createNode(QDomDocument doc, Id const &nodeId, QString const &prefix);
			void createMetaEditor(Id const &editor);
			qrRepo::RepoApi const &mApi;
		};

	}

}
