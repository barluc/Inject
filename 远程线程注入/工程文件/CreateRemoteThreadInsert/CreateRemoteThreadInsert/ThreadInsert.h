//��������Ȩ��
BOOL AdjustProcessTokenPrivilege();
//�򿪴�ע��Ľ���
HANDLE OpenTargetProcess(DWORD dpid);
//���ݽ������Ʋ���pid
DWORD GetProcessIDByName(const wchar_t* pName);
//�ڴ�ע��Ľ���������ռ�
BOOL TagetAlloc(HANDLE	 hTargetProcess,	LPVOID &lpAddr);
//��DLL·��д������Ŀռ�
BOOL WriteDLLToTarget(HANDLE hTargetProcess,	LPVOID lpAddr, LPCWSTR lpBuffer);
//��Ŀ������п����߳�
BOOL CreateThreadInTarget(HANDLE hTargetProcess,PTHREAD_START_ROUTINE pnfStartAddr,LPVOID lpAddr);
