#include <windows.h>
#include <memory>
#include <list>
#include <sstream>
#include <string>
#include <thread>

#include "windows/main_window.h"
#include "files/new_file.h"
#include "files/logger.h"

using std::unique_ptr;
using std::list;
using std::wstringstream;
using std::wstring;
using std::thread;

int APIENTRY wWinMain( _In_     HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_     LPWSTR    lpCmdLine,
                       _In_     int       nCmdShow )
{
   Windows::MainWindow mainWindow( hInstance, nCmdShow );

   list< thread > threadPool;
   list< unique_ptr< Files::NewFile > >files;
   wstring url;
   wstringstream buffer( lpCmdLine );

   while( buffer >> url )
   {
      threadPool.push_back( thread{ &Files::NewFile::upload, Files::NewFile( url ), std::ref( mainWindow ) } );
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

   return static_cast< int >( msg.wParam );
}
