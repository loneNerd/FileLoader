#include <windows.h>

#include "windows/main_window.h"
#include "files/new_file.h"
#include "files/logger.h"

#include <list>
#include <sstream>
#include <string>
#include <thread>

using std::list;
using std::wstringstream;
using std::wstring;
using std::thread;

int APIENTRY wWinMain( _In_     HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_     LPWSTR    lpCmdLine,
                       _In_     int       nCmdShow )
{
   Windows::CMainWindow mainWindow( hInstance, nCmdShow );
   Files::CLogger::Init();

   list< thread > threadPool;
   wstring url;
   wstringstream buffer( lpCmdLine );

   while( buffer >> url )
   {
      threadPool.push_back( thread{ &Files::CNewFile::upload,
                                    Files::CNewFile( url ),
                                    std::ref( mainWindow ) } );
   }

   MSG msg;

   while ( GetMessage( &msg, nullptr, 0, 0 ) )
   {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
   }

   for ( auto& elem : threadPool )
   {
      elem.join();
   }

   Files::CLogger::Destroy();
   return static_cast< int >( msg.wParam );
}
