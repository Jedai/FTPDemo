#pragma once

class FTPWorker
{
	wchar_t wszAgentName[20];
	wchar_t wszServer[128];
	wchar_t wszUser[20];
	wchar_t wszPassword[20];
	wchar_t wszFileName[128];

	wchar_t Current_path_for_files[256];

	DWORD dwErrorCode ;

	HINTERNET hInetConnection;
	HINTERNET hFTPConnection ;
	bool Download_file;

	bool bConnected;


public:

	FTPWorker();
	~FTPWorker();

	wchar_t* GetCurrentFileName();
	DWORD GetErrorCode();
	bool IsConnected();

	bool ConnectServer(wchar_t*, wchar_t*, wchar_t*);
	bool EnumerateFiles(bool);
	bool OpenFile(wchar_t*, wchar_t*);
	bool UpdateFile(wchar_t*, wchar_t*);
	bool GetDownloadState();
	bool SetDownloadState(bool Download_state);
};
