#ifndef IOREDIRECTOR_HH
#define IOREDIRECTOR_HH

#include <sstream>
#include <iostream>
#include <string>

std::string trim_multiline_string(const std::string& line);

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
