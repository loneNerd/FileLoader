#include "logger.h"

using namespace Files;

wofstream Logger::m_logFile;
mutex Logger::m_lock;

void Logger::Init()
{
   m_logFile.open( L"Log.txt", std::ios::app );
}

void Logger::AddNote( const wstring& note )
{
   m_lock.lock();

   if ( !m_logFile.is_open() )
   {
      m_lock.unlock();
      return;
   }

   auto now = std::chrono::system_clock::now();
   auto in_time_t = std::chrono::system_clock::to_time_t( now );

   tm time{ 0 };
   localtime_s( &time, &in_time_t );

   m_logFile << std::put_time( &time, L"%Y-%m-%d %X" );

   m_logFile<< L": " << note << std::endl;

   m_lock.unlock();
}

void Logger::Destroy()
{
   if ( m_logFile.is_open() )
      m_logFile.close();
}
