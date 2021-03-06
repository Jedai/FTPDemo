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

BOOL FTPWorker::ReconnectServer()
{
	if (hFTPConnection)
		InternetCloseHandle(hFTPConnection);
	if (hInetConnection)
		InternetCloseHandle(hInetConnection);

	return ConnectServer(wszServer, wszUser, wszPassword);
}

BOOL FTPWorker::ConnectServer(wchar_t *pwszServer, wchar_t *pwszUser, wchar_t *pwszPassword)
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
		hFTPConnection = InternetConnect(hInetConnection, wszServer, INTERNET_DEFAULT_FTP_PORT, wszUser, wszPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_EXISTING_CONNECT /*| INTERNET_FLAG_PASSIVE*/, NULL);

		if (hFTPConnection)
		{
			/*DWORD dwTimeout = 60000;

			InternetSetOption(hFTPConnection, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, &dwTimeout, sizeof(DWORD));
			InternetSetOption(hFTPConnection, INTERNET_OPTION_DATA_SEND_TIMEOUT, &dwTimeout, sizeof(DWORD));
			InternetSetOption(hFTPConnection, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwTimeout, sizeof(DWORD));
			InternetSetOption(hFTPConnection, INTERNET_OPTION_SEND_TIMEOUT, &dwTimeout, sizeof(DWORD));*/

			bConnected = TRUE;
			return TRUE;
		}
		else
		{
			dwErrorCode = GetLastError();
			InternetCloseHandle(hInetConnection);
		}
	}
	else
		dwErrorCode = GetLastError();

	return FALSE;
}


BOOL FTPWorker::EnumerateFiles(BOOL bFirst)
{
	BOOL bResult = FALSE;
	LONG size = 0;
	WIN32_FIND_DATA findData = { 0 };
	SYSTEMTIME sysTime = { 0 };
	wchar_t buff[20];

	static HINTERNET hFindHandle = 0;

	if (bFirst)
	{
		hFindHandle = FtpFindFirstFile(hFTPConnection, nullptr, &findData, INTERNET_FLAG_NEED_FILE, 0);
		if (hFindHandle)
		{
			wcscpy(wszFileName, findData.cFileName);

			size = (findData.nFileSizeHigh << 16) | findData.nFileSizeLow;
			_i64tow(size, wszFileSize, 10);

			FileTimeToSystemTime(&findData.ftLastWriteTime, &sysTime);

			_itow(sysTime.wDay, buff, 10);
			wcscpy(wszFileDate, buff);
			wcscat(wszFileDate, L"/");
			_itow(sysTime.wMonth, buff, 10);
			wcscat(wszFileDate, buff);
			wcscat(wszFileDate, L"/");
			_itow(sysTime.wYear, buff, 10);
			wcscat(wszFileDate, buff);
			wcscat(wszFileDate, L"<->");
			_itow(sysTime.wHour, buff, 10);
			wcscat(wszFileDate, buff);
			wcscat(wszFileDate, L":");
			_itow(sysTime.wMinute, buff, 10);
			wcscat(wszFileDate, buff);

			bResult = TRUE; 
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

				size = (findData.nFileSizeHigh << 16) | findData.nFileSizeLow;
				_i64tow(size, wszFileSize, 10);
								
				FileTimeToSystemTime(&findData.ftLastWriteTime, &sysTime);

				_itow(sysTime.wDay, buff, 10);
				wcscpy(wszFileDate, buff);
				wcscat(wszFileDate, L"/");
				_itow(sysTime.wMonth, buff, 10);
				wcscat(wszFileDate, buff);
				wcscat(wszFileDate, L"/");
				_itow(sysTime.wYear, buff, 10);
				wcscat(wszFileDate, buff);
				wcscat(wszFileDate, L"<->");
				_itow(sysTime.wHour, buff, 10);
				wcscat(wszFileDate, buff);
				wcscat(wszFileDate, L":");
				_itow(sysTime.wMinute, buff, 10);
				wcscat(wszFileDate, buff);

				bResult = TRUE; 
			}
			else
				dwErrorCode = GetLastError();
		}
	}
	
	if (!bResult)
		InternetCloseHandle(hFindHandle);

	return bResult;
}


void FTPWorker::SetFileInfo(DWORD index, PFILE_INFO pInfo)
{
	if (pFileList.size() == 0 || pFileList.size() == index)
	{
		FILE_INFO fInfo = { 0 };
		pFileList.push_back(fInfo);
	}

	if (index < pFileList.size())
	{
		pFileList[index] = *pInfo;
	}
}

int FTPWorker::GetItemCount()
{
	return pFileList.size();
}



BOOL FTPWorker::OpenFile(wchar_t *pFileName, wchar_t *pNewFileName)
{
	BOOL bResult = FALSE;

	if (FtpGetFile(hFTPConnection, pFileName, pNewFileName, FALSE, 0, INTERNET_FLAG_TRANSFER_BINARY, 0))
	{
		bResult = TRUE;
	}
	else
	{
		dwErrorCode = GetLastError();
		bResult = FALSE;
	}

	return bResult;
}


BOOL FTPWorker::UpdateFile(wchar_t *pFileName, wchar_t *pLocalFileName)
{
	BOOL bResult = FALSE;

	if (FtpPutFile(hFTPConnection, pLocalFileName, pFileName, FTP_TRANSFER_TYPE_BINARY, 0))
	{
		bResult = TRUE;
	}
	else
	{
		dwErrorCode = GetLastError();
		bResult = FALSE;
	}

	return bResult;
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

BOOL FTPWorker::IsConnected()
{
	return bConnected;
}

void FTPWorker::SetItemReceived(DWORD nItem, BOOL bReceived)
{
	if (nItem < pFileList.size())
		pFileList[nItem].bReceived = bReceived;
}

BOOL FTPWorker::IsItemReceived(DWORD nItem)
{
	if (nItem < pFileList.size())
		return pFileList[nItem].bReceived;

	return FALSE;
}

void FTPWorker::SetItemChanged(DWORD nItem, BOOL bReceived)
{
	if (nItem < pFileList.size())
		pFileList[nItem].bChanged = bReceived;
}

BOOL FTPWorker::IsItemChanged(DWORD nItem)
{
	if (nItem < pFileList.size())
		return pFileList[nItem].bChanged;

	return FALSE;
}

int FTPWorker::GetIndexByName(wchar_t *pFileNameToFind)
{
	int index = -1;
	int i = 0;

	for each (auto var in pFileList)
	{
		if (!wcscmp(var.wszFileName, pFileNameToFind))
		{
			index = i;
			break;
		}
		i++;
	}
	return index;
}

FILE_INFO* FTPWorker::GetFileInfoByIndex(DWORD dwIndex)
{
	if (dwIndex < pFileList.size())
		return &pFileList[dwIndex];
	return nullptr;
}


void FTPWorker::ClearFileList()
{
	pFileList.clear();
}