#pragma once

class FTPWorker
{
	wchar_t wszAgentName[20];
	wchar_t wszServer[128];
	wchar_t wszUser[20];
	wchar_t wszPassword[20];
	wchar_t wszFileName[128];

	DWORD dwErrorCode = 0;

	HINTERNET hInetConnection = 0;
	HINTERNET hFTPConnection = 0;

	bool bConnected = false;

public:

	FTPWorker();
	~FTPWorker();

	wchar_t* GetCurrentFileName();
	DWORD GetErrorCode();
	bool IsConnected();

<<<<<<< Updated upstream
=======
	void SetFileInfo(DWORD, PFILE_INFO);

	int GetIndexByName(wchar_t*);
	void SetItemReceived(DWORD, bool);
	bool IsItemReceived(DWORD);

>>>>>>> Stashed changes
	bool ConnectServer(wchar_t*, wchar_t*, wchar_t*);
	bool EnumerateFiles(bool);
	bool OpenFile(wchar_t*, wchar_t*);
	bool UpdateFile(wchar_t*, wchar_t*);
};
