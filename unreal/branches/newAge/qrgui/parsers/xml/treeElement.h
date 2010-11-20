#pragma once

#include <QtCore/QList>
#include "../../kernel/ids.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	class EditorManager;

	namespace parsers {

		class TreeElement
		{
		public:
			TreeElement(const Id &id);
			void addChild(TreeElement *const child);

		private:
			QList<TreeElement*> mChildren;
			Id mId;
		};
	}
}

