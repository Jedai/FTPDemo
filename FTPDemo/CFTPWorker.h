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

	bool bConnected = false;

	std::vector<FILE_INFO> pFileList;

public:

	FTPWorker();
	~FTPWorker();

	wchar_t* GetCurrentFileName();
	wchar_t* GetCurrentFileSize();
	wchar_t* GetCurrentFileDate();

	bool FtpGetFileInfo(wchar_t*);
	DWORD GetErrorCode();
	bool IsConnected();

	void SetFileInfo(DWORD, PFILE_INFO);

	void SetItemReceived(DWORD, bool);
	bool IsItemReceived(DWORD);

	bool ConnectServer(wchar_t*, wchar_t*, wchar_t*);
	bool EnumerateFiles(bool);
	bool OpenFile(wchar_t*, wchar_t*);
	bool UpdateFile(wchar_t*, wchar_t*);
};

