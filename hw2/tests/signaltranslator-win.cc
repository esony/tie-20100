#include "signaltranslator-win.hh"

namespace SignalTranslator {

    jmp_buf* WinSignalTranslator::s_jumpTarget = 0;

    namespace {

        void SignalHandler(int sig)
        {
            longjmp(*WinSignalTranslator::s_jumpTarget, sig );
        }

    }

    WinSignalTranslator::WinSignalTranslator()
    {
        m_oldJumpTarget = s_jumpTarget;
        s_jumpTarget = &m_currentJumpTarget;

        signal(SIGSEGV, SignalHandler);
        signal(SIGFPE, SignalHandler);
    }

    WinSignalTranslator::~WinSignalTranslator()
    {
        s_jumpTarget = m_oldJumpTarget;
    }
}
