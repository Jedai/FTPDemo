#include "stdafx.h"
#include "CFTPWorker.h"


FTPWorker::FTPWorker()
{
	wcscpy(wszAgentName, L"FTPDemoClient");
	wcscpy(wszServer, L"");
	wcscpy(wszUser, L"anonymous");
	wcscpy(wszPassword, L"");
	wcscpy(wszFileName, L"");
}

FTPWorker::~FTPWorker()
{
	if (hFTPConnection)
		InternetCloseHandle(hFTPConnection);
	if (hInetConnection)
		InternetCloseHandle(hInetConnection);
}

bool FTPWorker::ConnectServer(wchar_t *pwszServer, wchar_t *pwszUser, wchar_t *pwszPassword)
{
	if (pwszServer)
		wcscpy(wszServer, pwszServer);
	if (pwszUser)
		wcscpy(wszUser, pwszUser);
	if (pwszPassword)
		wcscpy(wszPassword, pwszPassword);


	hInetConnection = InternetOpen(wszAgentName, 0, 0, 0, 0);

	if (hInetConnection)
	{
		hFTPConnection = InternetConnect(hInetConnection, wszServer, INTERNET_DEFAULT_FTP_PORT, wszUser, wszPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_EXISTING_CONNECT, NULL);

		if (hFTPConnection)
		{
			bConnected = true;
			return true;
		}
		else
		{
			dwErrorCode = GetLastError();
			InternetCloseHandle(hInetConnection);
		}
	}
	else
		dwErrorCode = GetLastError();

	return false;
}


bool FTPWorker::EnumerateFiles(bool bFirst)
{
	bool bResult = false;
	WIN32_FIND_DATA findData = { 0 };
	static HINTERNET hFindHandle = 0;

	if (bFirst)
	{
		hFindHandle = FtpFindFirstFile(hFTPConnection, nullptr, &findData, INTERNET_FLAG_NEED_FILE, 0);
		if (hFindHandle)
		{
			wcscpy(wszFileName, findData.cFileName);
			bResult = true;
		}
		else
			dwErrorCode = GetLastError();
	}
	else
	{
		if (hFindHandle)
		{
			if (InternetFindNextFile(hFindHandle, &findData))
			{
				wcscpy(wszFileName, findData.cFileName);
				bResult = true;
			}
			else
				dwErrorCode = GetLastError();
		}
	}

	return bResult;
}


bool FTPWorker::OpenFile(wchar_t *pFileName, wchar_t *pNewFileName)
{
	if (FtpGetFile(hFTPConnection, pFileName, pNewFileName, false, 0, FTP_TRANSFER_TYPE_BINARY, 0))
	{
		return true;
	}

	return false;
}


bool FTPWorker::UpdateFile(wchar_t *pFileName, wchar_t *pLocalFileName)
{
	if (FtpPutFile(hFTPConnection, pLocalFileName, pFileName, FTP_TRANSFER_TYPE_BINARY, 0))
	{
		return true;
	}
	return false;
}


wchar_t* FTPWorker::GetCurrentFileName()
{
	return wszFileName;
}


DWORD FTPWorker::GetErrorCode()
{
	return dwErrorCode;
}

bool FTPWorker::IsConnected()
{
	return bConnected;
}
