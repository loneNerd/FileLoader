#pragma once
#ifndef WINDOWS_MAIN_WINDOW_H_
#define WINDOWS_MAIN_WINDOW_H_

#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace Windows
{
   enum EWindowElementID
   {
      eWEID_ListViewID = 1,
   };

   using std::mutex;
   using std::wstring;
   using std::vector;
   using std::pair;

   class MainWindow
   {
   public:
      MainWindow( HINSTANCE hInstance, int nCmdShow );

      MainWindow( const MainWindow& other )            = delete;
      MainWindow( MainWindow&& other )                 = delete;
      MainWindow& operator=( const MainWindow& other ) = delete;
      MainWindow& operator=( MainWindow&& other )      = delete;

      HWND getHandler() const { return m_windowHandler; }

      unsigned addFileStatus( const wstring& name, const wstring& status, const wstring& size );
      void updateFileSize( unsigned id, const wstring& size );
      void updateFileStatus( unsigned id, const wstring& status, COLORREF color );

      ~MainWindow();

   private:
      static LRESULT CALLBACK s_processes( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
      LRESULT CALLBACK processes( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

      WNDCLASSEXW m_mainWindow;
      HWND        m_windowHandler;
      HWND        m_listView;
      mutex       m_lock;
   };
}

#endif // WINDOWS_MAIN_WINDOW_H_
