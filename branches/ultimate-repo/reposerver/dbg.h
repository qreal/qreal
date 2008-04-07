#pragma once

#ifdef _LONG_DEBUG

namespace qreal_debug
{
  class TepkomDebugger
  {
    public:
      TepkomDebugger(const char *name) 
      { 
        qDebug(name); 
      };
	    ~TepkomDebugger() 
      { 
        qDebug("\\---  exit"); 
      };
  };
}

#define dbg \
  qreal_debug::TepkomDebugger tepkomDebugger(__PRETTY_FUNCTION__)

#else
#define dbg
#endif

