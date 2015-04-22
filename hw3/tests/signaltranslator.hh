#ifndef __SIGNAL_TRANSLATOR_HH
#define __SIGNAL_TRANSLATOR_HH

#if defined(_NOSIGNALTHROWS)
    #define UNITTEST_THROW_SIGNALS
#elif defined(_WIN32)

  #if !defined (__GNUC__)
    #define UNITTEST_EXTENSION
  #else
    #define UNITTEST_EXTENSION __extension__
  #endif

  #include "signaltranslator-win.hh"
  #define UNITTEST_THROW_SIGNALS \
	SignalTranslator::WinSignalTranslator sig; \
	if (UNITTEST_EXTENSION setjmp(*SignalTranslator::WinSignalTranslator::s_jumpTarget) != 0) \
        throw ("Unhandled system exception");

#else
  #include "signaltranslator-posix.hh"
  #define UNITTEST_THROW_SIGNALS \
    SignalTranslator::PosixSignalTranslator<SignalTranslator::SegmentationFault> segfault_translator; \
    SignalTranslator::PosixSignalTranslator<SignalTranslator::FloatingPointException> fpe_translator;
#endif

#endif //__SIGNAL_TRANSLATOR_HH
