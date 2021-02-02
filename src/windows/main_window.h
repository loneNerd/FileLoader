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

   class CMainWindow
   {
   public:
      CMainWindow( HINSTANCE hInstance, int nCmdShow );

      CMainWindow( const CMainWindow& other )            = delete;
      CMainWindow( CMainWindow&& other )                 = delete;
      CMainWindow& operator=( const CMainWindow& other ) = delete;
      CMainWindow& operator=( CMainWindow&& other )      = delete;

      HWND getHandler() const { return m_windowHandler; }

      unsigned addFileStatus( const wstring& name, const wstring& status, const wstring& size );
      void updateFileSize( unsigned id, const wstring& size );
      void updateFileStatus( unsigned id, const wstring& status, COLORREF color );

      ~CMainWindow();

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
