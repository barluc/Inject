//��Suspend��Ŀ�����
BOOL OpenTargetProcess(LPCWSTR ProcName , STARTUPINFO  &st , PROCESS_INFORMATION &pi );
//��������Ȩ��
BOOL AdjustProcessTokenPrivilege();
//�ڴ�ע��Ľ���������ռ�
BOOL TagetAlloc(HANDLE	 hTargetProcess,	LPVOID &lpAddr);
//��DLL·��д������Ŀռ�
BOOL WriteDLLToTarget(HANDLE hTargetProcess ,	LPVOID lpAddr , LPCWSTR lpBuffer);
//��Ŀ������п����߳�
BOOL CreateThreadInTarget(HANDLE hTargetProcess , PTHREAD_START_ROUTINE pnfStartAddr , LPVOID lpAddr);
