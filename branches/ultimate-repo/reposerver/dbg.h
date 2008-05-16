#pragma once

#ifdef __USE_GNU

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

#else // __USE_GNU

#define dbg

#endif // __USE_GNU
