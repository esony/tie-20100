#include "ioredirector.hh"

void remove_newlines(std::string& line)
{
    if( !line.empty() && line.at(line.length()-1) == '\n' )
    {
        line.erase(line.end() - 1);
    }
    if( !line.empty() && line.at(line.length()-1) == '\r' )
    {
        line.erase(line.end() - 1);
    }
//   line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
//   line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
}


IORedirector::IORedirector()
{
    sbuf_out_ = std::cout.rdbuf();
}

IORedirector::~IORedirector()
{   
    std::cout.rdbuf(sbuf_out_);
}

void IORedirector::start()
{
    reset();
    std::cout.rdbuf(buffer_out_.rdbuf());
}

std::string IORedirector::stop()
{
    std::string output = buffer_out_.str();
    remove_newlines(output);
    reset();
    std::cout.rdbuf(sbuf_out_);   
    return output;
}

void IORedirector::reset()
{
    buffer_out_.str("");
}
