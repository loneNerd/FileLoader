#pragma once
#ifndef FILES_LOGGER_H_
#define FILES_LOGGER_H_

#include <chrono>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <mutex>
#include <string>

using std::wofstream;
using std::mutex;
using std::wstring;

namespace Files
{
   class Logger
   {
   public:
      Logger( const Logger& other )            = delete;
      Logger( Logger&& other )                 = delete;
      Logger& operator=( const Logger& other ) = delete;
      Logger& operator=( Logger&& other )      = delete;

      ~Logger();

      static Logger& instance()
      {
         static Logger inst;
         return inst;
      }

      void init( const wstring& fileName );
      void addNote( const wstring& note );

   private:
      Logger() {}

      wofstream m_logFile;
      mutex     m_lock;
   };
}

#endif // FILES_LOGGER_H_
