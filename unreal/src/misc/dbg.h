#ifndef VERBOSE_DEBUG_H
#define VERBOSE_DEBUG_H

#ifdef _LONG_DEBUG

class __tepkom_debugger{
      public:
          __tepkom_debugger(const char *name) { qDebug(name); };
	  ~__tepkom_debugger() { qDebug("\\---  exit"); };
};

#define dbg \
    __tepkom_debugger __a_tepkom_debugger(__PRETTY_FUNCTION__)

#else

#define dbg

#endif

#endif
