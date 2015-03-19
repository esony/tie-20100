#ifndef __SIGNAL_TRANSLATOR_POSIX_HH
#define __SIGNAL_TRANSLATOR_POSIX_HH

#include <exception>
#include <csignal>

// Adapter from http://www.ibm.com/developerworks/library/l-cppexcep/
// TODO: Use .cc file for implementation

namespace SignalTranslator
{
    template <class SignalExceptionClass> class PosixSignalTranslator
    {
    private:
        class SingleTonTranslator
        {
        public:
            SingleTonTranslator()
            {
                signal(SignalExceptionClass::GetSignalNumber(), SignalHandler);
            }

            static void SignalHandler(int)
            {
                throw SignalExceptionClass();
            }
        };

    public:
        PosixSignalTranslator()
        {
            static SingleTonTranslator s_objTranslator;
        }
    };

    class SegmentationFault: public std::exception
    {
    public:
        virtual const char* what() const throw()
        {
            return "Segmentation Fault";
        }
        static int GetSignalNumber() {return SIGSEGV;}

    };

    class FloatingPointException : public std::exception
    {
    public:
        virtual const char* what() const throw()
        {
            return "Floating Point Exception";
        }
        static int GetSignalNumber() {return SIGFPE;}
    };
}


#endif
