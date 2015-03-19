#ifndef __SIGNAL_TRANSLATOR_WIN_HH
#define __SIGNAL_TRANSLATOR_WIN_HH

#include <setjmp.h>
#include <signal.h>

namespace SignalTranslator {

    class WinSignalTranslator
    {
    public:
        WinSignalTranslator();
        ~WinSignalTranslator();

        static jmp_buf* s_jumpTarget;

    private:
        jmp_buf m_currentJumpTarget;
        jmp_buf* m_oldJumpTarget;

    };
}


#endif
