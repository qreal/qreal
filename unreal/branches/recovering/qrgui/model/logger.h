#pragma once

#include <QString>

#include "model.h"

namespace qReal {
	namespace model {
		class Model;

		class Logger
		{
			public:
				Logger(Model *model);
				void enable();
				void disable();

				enum action
				{
					setData,
					addElement,
					removeElement,
					createDiagram,
					destroyDiagram
				};

				enum flag
				{
					editors,
					diagrams,
					uselessMessages,
					invalidMessages,
				};

				void setFlag(flag name, bool arg);

				void log(action performed,
						Id const scene);
				void log(action performed,
						Id const scene, Id const target);
				void log(action performed,
						Id const scene, Id const target,
							QVariant const data, QString const additional);

			private:
				Model *mModel;

				bool enabled;
				bool flagsEnabled[4];

				QString getDataString(QVariant const data) const;
				void write(QString const message, Id const scene);
		};
	}
}
