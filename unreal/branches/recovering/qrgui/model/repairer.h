#pragma once

#include "message.h"
#include "../editorManager/editorManager.h"

namespace qReal {
		class Repairer {
			public:
				Repairer(const EditorManager &editorManager);

				Id getCorrectId(Id const target);
			private:
				const EditorManager &mEditorManager;
				QMultiHash<Id, QList<Message>* > mLogs;

				void readLog(Id const editor);
		};
}
