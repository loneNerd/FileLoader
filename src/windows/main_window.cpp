#include "main_window.h"

using namespace Windows;

MainWindow::MainWindow( HINSTANCE hInstance, int nCmdShow )
{
   m_mainWindow.cbSize        = sizeof( WNDCLASSEX );
   m_mainWindow.style         = CS_HREDRAW | CS_VREDRAW;
   m_mainWindow.lpfnWndProc   = MainWindow::s_processes;
   m_mainWindow.hInstance     = hInstance;
   m_mainWindow.hCursor       = LoadCursor( nullptr, IDC_ARROW );
   m_mainWindow.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
   m_mainWindow.lpszClassName = L"Main Window";

   RegisterClassExW( &m_mainWindow );

   m_windowHandler = CreateWindow( L"Main Window",
                                   L"File Downloader", 
                                   WS_OVERLAPPEDWINDOW | WS_VSCROLL,
                                   CW_USEDEFAULT, 0, 800, 600, 
                                   nullptr, 
                                   nullptr, 
                                   hInstance, 
                                   this );

   SetScrollRange( m_windowHandler, SB_VERT, 0, 0, FALSE );

   ShowWindow( m_windowHandler, nCmdShow );
   UpdateWindow( m_windowHandler );

   RECT rc{};
   GetClientRect(m_windowHandler, &rc);
   m_listView = CreateWindowEx( LVS_EX_DOUBLEBUFFER, 
                                WC_LISTVIEW,
                                L"",
                                WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | WS_OVERLAPPED,
                                0, 0,
                                rc.right - rc.left, rc.bottom - rc.top,
                                m_windowHandler,
                                ( HMENU )eWEID_ListViewID,
                                hInstance,
                                this );

   LV_COLUMN lvC;

   lvC.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
   lvC.fmt      = LVCFMT_LEFT;
   lvC.cx       = 150;
   lvC.pszText  = ( LPWSTR )L"Name";
   lvC.iSubItem = 0;

   SendMessage( m_listView, LVM_INSERTCOLUMN, 0, ( LPARAM )&lvC );

   lvC.cx       = 100;
   lvC.pszText  = ( LPWSTR )L"Status";
   lvC.iSubItem = 1;

   SendMessage( m_listView, LVM_INSERTCOLUMN, 1, ( LPARAM )&lvC );

   lvC.cx       = 500;
   lvC.pszText  = ( LPWSTR )L"Progress";
   lvC.iSubItem = 2;

   SendMessage( m_listView, LVM_INSERTCOLUMN, 2, ( LPARAM )&lvC );
}

MainWindow::~MainWindow()
{
   DestroyWindow( m_windowHandler );
   DestroyWindow( m_listView );
}

unsigned MainWindow::addFileStatus( const wstring& name, const wstring& status, const wstring& size )
{
   m_lock.lock();

   int iItem = SendMessage( m_listView, LVM_GETITEMCOUNT, 0, 0 );

   LV_ITEM lvi;

   lvi.mask     = LVIF_TEXT | LVIF_PARAM;
   lvi.iItem    = iItem;
   lvi.iSubItem = 0;
   lvi.pszText  = ( LPWSTR )name.c_str();

   ListView_InsertItem( m_listView, &lvi );
   ListView_SetItemText( m_listView, iItem, 1, ( LPWSTR )status.c_str() );
   ListView_SetItemText( m_listView, iItem, 2, ( LPWSTR )size.c_str() );

   m_lock.unlock();

   return iItem;
}

void MainWindow::updateFileStatus( unsigned id, const wstring& status, COLORREF color )
{
   ListView_SetItemText( m_listView, id, 1, ( LPWSTR )status.c_str() );
   
}

void MainWindow::updateFileSize( unsigned id, const wstring& progress )
{
   ListView_SetItemText( m_listView, id, 2, ( LPWSTR )progress.c_str() );
}

LRESULT MainWindow::s_processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   MainWindow* pThis;

   if ( uMsg == WM_NCCREATE )
   {
      pThis = static_cast< MainWindow* >( reinterpret_cast< CREATESTRUCT* >( lParam )->lpCreateParams );

      SetLastError(0);
      if ( !SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( pThis ) ) )
      {
         if ( GetLastError() != 0 )
            return FALSE;
      }
   }
   else
   {
      pThis = reinterpret_cast< MainWindow* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
   }

   if ( pThis )
   {
      return pThis->processes( hWnd, uMsg, wParam, lParam );
   }

   return FALSE;
}

LRESULT CALLBACK MainWindow::processes( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
   switch ( message )
   {
      case WM_SIZE:
      {
         RECT rc{};
         GetWindowRect( m_windowHandler, &rc );
         SetWindowPos( m_listView, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);

         break;
      }
      case WM_DESTROY:
      {
         m_isClose = true;
         PostQuitMessage( 0 );
         break;
      }
   }

   return DefWindowProc( hWnd, message, wParam, lParam );
}
