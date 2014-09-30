#include "stdafx.h"
#include "CFTPWorker.h"


FTPWorker::FTPWorker()
{
	wcscpy(wszAgentName, L"FTPDemoClient");
	wcscpy(wszServer, L"");
	wcscpy(wszUser,L"anonymous");
	wcscpy(wszPassword, L"anonymous");
	wcscpy(wszFileName, L"");
}

FTPWorker::~FTPWorker()
{
	pFileList.clear();

	if (hFTPConnection)
		InternetCloseHandle(hFTPConnection);
	if (hInetConnection)
		InternetCloseHandle(hInetConnection);
}

bool FTPWorker::ConnectServer(wchar_t *pwszServer, wchar_t *pwszUser, wchar_t *pwszPassword)
{
	if (pwszServer)
		wcscpy_s(wszServer, COUNTOFWCHAR(wszServer) - 1, pwszServer);
	if (pwszUser)
		wcscpy_s(wszUser, COUNTOFWCHAR(wszUser) - 1, pwszUser);
	if (pwszPassword)
		wcscpy_s(wszPassword, COUNTOFWCHAR(wszPassword) - 1, pwszPassword);


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
			//wcscpy(wszFileName, findData.cFileName);
			bResult = FtpGetFileInfo(findData.cFileName);
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
				//wcscpy(wszFileName, findData.cFileName);
				bResult = FtpGetFileInfo(findData.cFileName);
			}
			else
				dwErrorCode = GetLastError();
		}
	}
	
	return bResult;
}


void FTPWorker::SetFileInfo(DWORD index, PFILE_INFO pInfo)
{
	if (index < pFileList.size())
	{
		pFileList[index] = *pInfo;
	}
}


bool FTPWorker::FtpGetFileInfo(wchar_t *wszFile)
{
	DWORD dwSizeLow = 0;
	DWORD dwSizeHigh = 0;
	LONG size = 0;

	wcscpy(wszFileName, wszFile);
	wcscpy(wszFileDate, L"");
	wcscpy(wszFileSize, L"");

	HINTERNET hFile = FtpOpenFile(hFTPConnection, wszFile, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, 0);
	if (hFile)
	{
		dwSizeLow = FtpGetFileSize(hFile, &dwSizeHigh);
		size = (dwSizeHigh << 16) | dwSizeLow;
		_i64tow(size, wszFileSize, 10);

		wchar_t wszCommand[256] = L"MDTM ";
		wcscat(wszCommand, wszFileName);
		if (FtpCommand(hFTPConnection, false, FTP_TRANSFER_TYPE_BINARY, wszCommand, 0, 0))
			wcscpy(wszFileDate, wszCommand);

		InternetCloseHandle(hFile);
		//return true;
	}
	return true;
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

wchar_t* FTPWorker::GetCurrentFileDate()
{
	return wszFileDate;
}

wchar_t* FTPWorker::GetCurrentFileSize()
{
	return wszFileSize;
}

DWORD FTPWorker::GetErrorCode()
{
	return dwErrorCode;
}

bool FTPWorker::IsConnected()
{
	return bConnected;
}

void FTPWorker::SetItemReceived(DWORD nItem, bool bReceived)
{
	if (nItem < pFileList.size())
		pFileList[nItem].bReceived = bReceived;
}

bool FTPWorker::IsItemReceived(DWORD nItem)
{
	if (nItem < pFileList.size())
		return pFileList[nItem].bReceived;

	return false;
}