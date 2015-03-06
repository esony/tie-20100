#ifndef IOREDIRECTOR_HH
#define IOREDIRECTOR_HH

#include <sstream>
#include <iostream>
#include <string>

void remove_newlines(std::string& line);

class IORedirector
{
public:
    IORedirector();
    ~IORedirector();    
    
    void start();
    std::string stop();

    void reset();    
    
private:
    std::streambuf* sbuf_out_;
    std::stringstream buffer_out_;
};

#endif
