#pragma once

#include <QThread>
#include <QProcess>

namespace qReal {
	class DebuggerThread : QThread
	{
		public:
			DebuggerThread();
			void run();
		private:
			QProcess *process;
	};
}
