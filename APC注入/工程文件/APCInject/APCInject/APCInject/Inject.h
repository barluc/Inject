//��Suspend��Ŀ�����
BOOL OpenTargetProcess(LPCWSTR ProcName , STARTUPINFO  &st , PROCESS_INFORMATION &pi );
//��Ŀ����̵�ַ�ռ��з�������ص�DLL·���ռ�
BOOL TagetAlloc(HANDLE	 hTargetProcess,	LPVOID &lpAddr);
//�Ѵ����ص�DLL·��д��Ŀ����̿ռ�
BOOL WriteDLLToTarget(HANDLE hTargetProcess ,	LPVOID lpAddr , LPCWSTR lpBuffer);
