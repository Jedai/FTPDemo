#pragma once

class FTPWorker
{
	wchar_t wszAgentName[20];
	wchar_t wszServer[128];
	wchar_t wszUser[20];
	wchar_t wszPassword[20];

	wchar_t wszFileName[128];
	wchar_t wszFileDate[32];
	wchar_t wszFileSize[32];

	DWORD dwErrorCode = 0;

	HINTERNET hInetConnection = 0;
	HINTERNET hFTPConnection = 0;

	BOOL bConnected = FALSE;

	std::vector<FILE_INFO> pFileList;

public:

	FTPWorker();
	~FTPWorker();

	wchar_t* GetCurrentFileName();
	wchar_t* GetCurrentFileSize();
	wchar_t* GetCurrentFileDate();

	DWORD GetErrorCode();
	BOOL IsConnected();

	void SetFileInfo(DWORD, PFILE_INFO);

	int GetItemCount();
	int GetIndexByName(wchar_t*);
	FILE_INFO* GetFileInfoByIndex(DWORD);
	void SetItemReceived(DWORD, BOOL);
	BOOL IsItemReceived(DWORD);

	BOOL FtpGetFileInfo(wchar_t*);

	BOOL ConnectServer(wchar_t*, wchar_t*, wchar_t*);
	BOOL ReconnectServer();
	BOOL EnumerateFiles(BOOL);
	BOOL OpenFile(wchar_t*, wchar_t*);
	BOOL UpdateFile(wchar_t*, wchar_t*);
};
