#include <sstream>

#include "ioredirector.hh"

std::string trim_multiline_string(const std::string& line)
{

   std::istringstream ifs{line};
   std::string tmp;
   std::ostringstream ofs;

   while(getline(ifs, tmp ))
   {
      tmp.erase(tmp.find_last_not_of(" \n\n\t") + 1 );
      ofs << tmp << std::endl;
   }

   std::string full_line{ofs.str()};

   if( !full_line.empty() && full_line.at(full_line.length()-1) == '\n' )
    {
        full_line.erase(full_line.end() - 1);
    }
    if( !full_line.empty() && full_line.at(full_line.length()-1) == '\r' )
    {
        full_line.erase(full_line.end() - 1);
    }
    return full_line;
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
   std::string output = trim_multiline_string(buffer_out_.str());
//    remove_newlines(output);
    reset();
    std::cout.rdbuf(sbuf_out_);
    return output;
}

void IORedirector::reset()
{
    buffer_out_.str("");
}
