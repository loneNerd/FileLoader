#include "new_file.h"

using namespace Files;

NewFile::NewFile( wstring url )
{
   wregex protocolPattern{ L"http(s)?://" };
   wsmatch matches;

   if ( regex_search( url, matches, protocolPattern ) )
      url.erase( url.cbegin(), url.cbegin() + matches[ 0 ].str().size() );

   size_t bSlash = url.find_first_of( L'/' );

   m_domain.assign( url.cbegin(), url.cbegin() + bSlash );

   m_urlPath.assign( url.cbegin() + bSlash, url.cend() );

   bSlash = m_urlPath.find_last_of( L'/' );
   m_filePath.assign( m_urlPath.cbegin() + bSlash + 1, m_urlPath.cend() );
}

void NewFile::upload( MainWindow& window )
{
   DWORD dwSize       = 0;
   DWORD dwDownloaded = 0;

   char* pszOutBuffer;

   BOOL bResults      = FALSE;
   HINTERNET hSession = NULL,
             hConnect = NULL,
             hRequest = NULL;

   clock_t start = clock();
   unsigned id = window.addFileStatus( m_filePath, L"Connecting", L"<NONE>" );

   hSession = WinHttpOpen( L"File Loader/1.0",
                           WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                           WINHTTP_NO_PROXY_NAME,
                           WINHTTP_NO_PROXY_BYPASS, 0 );

   if ( hSession )
   {
      hConnect = WinHttpConnect( hSession,
                                 m_domain.c_str(),
                                 INTERNET_DEFAULT_HTTP_PORT,
                                 0 );
   }

   if ( hConnect )
   {
      hRequest = WinHttpOpenRequest( hConnect,
                                     L"GET",
                                     m_urlPath.c_str(),
                                     NULL,
                                     WINHTTP_NO_REFERER,
                                     NULL,
                                     NULL );
   }

   if ( hRequest )
   {
      bResults = WinHttpSendRequest( hRequest,
                                     WINHTTP_NO_ADDITIONAL_HEADERS,
                                     0,
                                     WINHTTP_NO_REQUEST_DATA,
                                     0, 0, 0 );
   }

   if ( bResults )
   {
      bResults = WinHttpReceiveResponse( hRequest, NULL );
   }

   LPVOID lpOutBuffer = NULL;

   if ( bResults )
   {
      WinHttpQueryHeaders( hRequest, 
                           WINHTTP_QUERY_RAW_HEADERS_CRLF,
                           WINHTTP_HEADER_NAME_BY_INDEX, NULL,
                           &dwSize, 
                           WINHTTP_NO_HEADER_INDEX );

      if ( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
      {
         lpOutBuffer = new WCHAR[ dwSize / sizeof( WCHAR ) ];

         bResults = WinHttpQueryHeaders( hRequest,
                                         WINHTTP_QUERY_RAW_HEADERS_CRLF,
                                         WINHTTP_HEADER_NAME_BY_INDEX,
                                         lpOutBuffer, 
                                         &dwSize,
                                         WINHTTP_NO_HEADER_INDEX );
      }
   }

   unsigned fileSize = 0;

   if ( bResults )
   {
      wregex lengthPattern{ LR"(Content-Length: \d+)" };
      wsmatch matches;
      wstring str( ( LPCWSTR )lpOutBuffer );

      if ( regex_search( str, matches, lengthPattern ) )
      {
         str = matches.str();
         wstring temp( str.cbegin() + 16, str.cend() );
         fileSize = std::stoi( temp );
      }
   }

   delete[] lpOutBuffer;

   ofstream newFile( m_filePath, std::ios::binary | std::ofstream::trunc );

   if ( !newFile.is_open() )
   {
      window.updateFileStatus( id, L"File create failed", RGB( 255, 0, 0 ) );
      bResults = false;
   }

   if ( bResults )
   {
      window.updateFileStatus( id, L"Loading", RGB( 50, 205, 50 ) );
      window.updateFileSize( id, L"0%" );

      do
      {
         dwSize = 0;

         if ( !WinHttpQueryDataAvailable( hRequest, &dwSize ) )
         {
            window.updateFileStatus( id, L"Download Failed", RGB( 255, 0, 0 ) );
            bResults = false;
            break;
         }

         dwSize += 1;
         pszOutBuffer = new char[ dwSize ];

         if ( !pszOutBuffer )
         {
            window.updateFileStatus( id, L"Out of memory", RGB( 255, 0, 0 ) );
            bResults = false;
            break;
         }

         if ( !WinHttpReadData( hRequest, ( LPVOID )pszOutBuffer, dwSize - 1, &dwDownloaded ) )
         {
            window.updateFileStatus( id, L"Read file failed", RGB( 255, 0, 0 ) );
            delete[] pszOutBuffer;
            bResults = false;
            break;
         }

         newFile.write( pszOutBuffer, ( size_t )dwDownloaded );

         delete[] pszOutBuffer;

         m_downloaded += dwDownloaded;

         if ( start + 500 < clock() )
         {
            window.updateFileSize( id, std::to_wstring( m_downloaded * 100 / fileSize ) + L"%" );
            start = clock();
         }

      }
      while ( dwSize - 1 > 0 );
   }
   else
   {
      window.updateFileStatus(id, L"Server failed", RGB(255, 0, 0));
      Logger::AddNote(L"Download of " + m_filePath + L" failed");
   }

   if ( bResults )
   {
      window.updateFileStatus( id, L"Done", RGB( 50, 205, 50 ) );
      window.updateFileSize( id, L"100%" );
      Logger::AddNote( L"Download of " + m_filePath + L" complete" );
   }

   if ( newFile.is_open() )
      newFile.close();

   if ( hRequest )
      WinHttpCloseHandle( hRequest );

   if ( hConnect )
      WinHttpCloseHandle( hConnect );

   if ( hSession )
      WinHttpCloseHandle( hSession );
}
