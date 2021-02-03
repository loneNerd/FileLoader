#pragma once
#ifndef FILES_NEW_FILE_H_
#define FILES_NEW_FILE_H_

#include <ctime>
#include <fstream>
#include <string>
#include <regex>
#include <Windows.h>
#include <winhttp.h>

#include "../windows/main_window.h"
#include "logger.h"

using std::ofstream;
using std::wstring;
using std::wregex;
using std::wsmatch;
using Windows::MainWindow;

namespace Files
{
   class NewFile
   {
   public:
      NewFile( wstring name );

      NewFile( const NewFile& other )            = default;
      NewFile( NewFile&& other )                 = default;
      NewFile& operator=( const NewFile& other ) = default;
      NewFile& operator=( NewFile&& other )      = default;

      ~NewFile() {};

      void upload( MainWindow& window );

   private:
      wstring m_domain;
      wstring m_urlPath;
      wstring m_filePath;

      long long m_downloaded = 0;
   };
}

#endif //FILES_NEW_FILE_H_
