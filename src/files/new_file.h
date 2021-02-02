#pragma once
#ifndef FILES_NEW_FILE_H_
#define FILES_NEW_FILE_H_

#include <fstream>
#include <memory>
#include <string>
#include <regex>
#include <Windows.h>
#include <winhttp.h>

#include "../windows/main_window.h"
#include "logger.h"

using std::ofstream;
using std::shared_ptr;
using std::wstring;
using std::wregex;
using std::wsmatch;
using Windows::CMainWindow;

namespace Files
{
   class CNewFile
   {
   public:
      CNewFile( wstring name );

      CNewFile( const CNewFile& other )            = default;
      CNewFile( CNewFile&& other )                 = default;
      CNewFile& operator=( const CNewFile& other ) = default;
      CNewFile& operator=( CNewFile&& other )      = default;

      ~CNewFile() {};

      void upload( CMainWindow& window );

   private:
      wstring m_domain;
      wstring m_urlPath;
      wstring m_filePath;

      long long m_downloaded = 0;
   };
}

#endif //FILES_NEW_FILE_H_
