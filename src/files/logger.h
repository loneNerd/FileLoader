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
      Logger()                                  = delete;
      Logger( const Logger& other )            = delete;
      Logger( Logger&& other )                 = delete;
      Logger& operator=( const Logger& other ) = delete;
      Logger& operator=( Logger&& other )      = delete;
      ~Logger()                                 = delete;

      static void Init();
      static void Destroy();
      static void AddNote( const wstring& note );

   private:
      static wofstream m_logFile;
      static mutex     m_lock;
   };
}

#endif // FILES_LOGGER_H_
