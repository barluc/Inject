#include <windows.h>
#include <IMM.h>
#include <stdio.h>
#include <stdlib.h>
#include "pipe.h"
#include <tchar.h>
#pragma comment(lib,"imm32.lib")
typedef struct { 
	DWORD dwPrivateDataSize; 
	DWORD fdwProperty; 
	DWORD fdwConversionCaps; // fdwConverstionCaps 
	DWORD fdwSentenceCaps; 
	DWORD fdwUICaps; 
	DWORD fdwSCSCaps; 
	DWORD fdwSelectCaps; 
} IMEINFO, *LPIMEINFO;
//��������
#define CLSNAME_UI			_T("DLLISUI")		//UI
#define CS_INPUTSTAR			(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)


#pragma data_seg("mysechx")
char  sig[7] = {'W', 'I','N','S','U','N'};
DWORD CallBackData1=1;
DWORD CallBackData2=2;
DWORD CallBackData3=3;
DWORD OnloadDllWhenExit=0;    // �����뷨�˳�ʱ�Ƿ�ж�ؿͻ�DLL  0-�ǣ�1-��
DWORD LoadNextWhenActive=0;    // �������뷨����ʱ���Ƿ��Զ�����һ�����뷨 0-��1-��
char g_IMEDLLString[802]="";
#pragma data_seg()

typedef DWORD (CALLBACK * RUNDLLHOSTCALLBACK)(DWORD calldata1, DWORD calldata2,DWORD calldata3);

HMODULE CilentDLL=NULL;
RUNDLLHOSTCALLBACK RunDllCallBackX=NULL;

// �ȶ���ø��ֺ���
BOOL ImeClass_Register(HINSTANCE hInstance);
void ImeClass_Unregister(HINSTANCE hInstance);
LRESULT WINAPI UIWndProc(HWND hUIWnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL MyGenerateMessage(HIMC hIMC, UINT msg, WPARAM wParam, LPARAM lParam);

void MyLoadCilentDLLFun()
{
	if (CilentDLL==NULL)
	{
		  if (lstrlen(g_IMEDLLString)>0)
		  {
			  CilentDLL=LoadLibrary(g_IMEDLLString);   // �����뷨����ʱͬʱ���ؿͻ�DLL
			  if (CilentDLL!=NULL)
			  {
				  // ������ڣ�����ÿͻ�DLLָ�����ƵĻص�����
				  RunDllCallBackX=(RUNDLLHOSTCALLBACK)GetProcAddress(CilentDLL,"RunDllHostCallBack");
				  if (RunDllCallBackX!=NULL)
				  {
					  RunDllCallBackX(CallBackData1,CallBackData2,CallBackData3);
				  }
			  }
		  }
	}
}

DWORD WINAPI ImeDllLoadThread(
						LPVOID lpThreadID
						)
{
	OutputDebugStr("[Winsun] ImeDllLoadThread Started! \n");
	PipeClient("\\\\.\\pipe\\winsun_ime_load_dll");
	return 1;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	HANDLE hNewThread = NULL;
	switch(fdwReason)
    {
      case DLL_PROCESS_ATTACH:
		  OutputDebugStr("[!] DllMain enter \n");
		  if(!ImeClass_Register(hinstDLL)) return FALSE;   // DLL����ʱע������UI����������
		  //MyLoadCilentDLLFun(); changed by winsun
		hNewThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ImeDllLoadThread, NULL, NULL, NULL);
		if ( !hNewThread )
		{
             OutputDebugStr("[!] DllMain CreateThread error\n");
			 return FALSE;
		}
		
		  break;
	  case DLL_THREAD_ATTACH:
		 break;
	  case DLL_THREAD_DETACH:
		 break;
      case DLL_PROCESS_DETACH:
		  ImeClass_Unregister(hinstDLL);  // DLL�˳�ʱע��ע��Ĵ�����
		  if (CilentDLL!=NULL && OnloadDllWhenExit==0)
		  {
			  FreeLibrary(CilentDLL);    // ���뷨�˳�ʱж�ؿͻ�DLL
		  }
        break;
      default:
        break;
    }
	return true;
}

//************************************
// Method:    IMESetPubString
// FullName:  IMESetPubString
// Access:    public 
// Returns:   int WINAPI
// Qualifier:
// Parameter: LPCTSTR tmpStr ��ע���dll·����
// Parameter: DWORD UnloadDLL ���뷨�˳�ʱ��0ж��ע���dll��1��ж��
// Parameter: DWORD loadNextIme �������뷨����ʱ���Ƿ��Զ�����һ�����뷨 0-��1-��
// Parameter: DWORD DllData1 
// Parameter: DWORD DllData2
// Parameter: DWORD DllData3
//************************************
int WINAPI IMESetPubString(LPCTSTR tmpStr,DWORD UnloadDLL,DWORD loadNextIme,DWORD DllData1,DWORD DllData2,DWORD DllData3)
{
	CallBackData1=DllData1;
	CallBackData2=DllData2;
	CallBackData3=DllData3;
	OnloadDllWhenExit=UnloadDLL;
	LoadNextWhenActive=loadNextIme;

	memset(g_IMEDLLString,0,802);
	if (lstrlen(tmpStr)>800)
	{
		lstrcpyn(g_IMEDLLString,tmpStr,800);
	}
	else
	{
		lstrcpy(g_IMEDLLString,tmpStr);
	}
	return 1;
}

int WINAPI IMEClearPubString()
{
	CallBackData1=0;
	CallBackData2=0;
	CallBackData3=0;
	OnloadDllWhenExit=0;
	LoadNextWhenActive=0;

	memset(g_IMEDLLString,0,802);
	return 1;
}



//************************************************************
//	�������뷨����UI��ע��
//************************************************************
BOOL ImeClass_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wc;
	
    //
    // register class of UI window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_INPUTSTAR | CS_IME;
    wc.lpfnWndProc    = UIWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 2 * sizeof(LONG);
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPTSTR)NULL;
    wc.lpszClassName  = CLSNAME_UI;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
	{
		OutputDebugStr("ImeClass_Register RegisterClassEx fail!");
		return FALSE;
	}
        
	
	return TRUE;
}

//**************************************************************
//	ע��ע��Ĵ�����
//**************************************************************
void ImeClass_Unregister(HINSTANCE hInstance)
{
	UnregisterClass(CLSNAME_UI,hInstance);
}


// ------------------------------------

DWORD WINAPI ImeConversionList(HIMC hIMC,LPCTSTR lpSource,LPCANDIDATELIST lpCandList,DWORD dwBufLen,UINT uFlag)
{
    return 0;
}

//�������뷨״̬��������������
BOOL WINAPI ImeConfigure(HKL hKL,HWND hWnd, DWORD dwMode, LPVOID lpData)
{
    switch (dwMode) {
    case IME_CONFIG_GENERAL:
		//Windows��׼���뷨��չ���� V1.0
        MessageBox(NULL,"���뷨ע��","�������뷨��չ",48);
        break;
    default:
        return (FALSE);
        break;
    }
    return (TRUE);
}

BOOL WINAPI ImeDestroy(UINT uForce)
{
    if (uForce) {
        return (FALSE);
    }

    return (TRUE);
}

LRESULT WINAPI ImeEscape(HIMC hIMC,UINT uSubFunc,LPVOID lpData)
{
	return FALSE;
}


BOOL WINAPI ImeInquire(LPIMEINFO lpIMEInfo,LPTSTR lpszUIClass,LPCTSTR lpszOption)
{
	// ���뷨��ʼ������
    lpIMEInfo->dwPrivateDataSize = 0; //ϵͳ������ΪINPUTCONTEXT.hPrivate����ռ�

    lpIMEInfo->fdwProperty = IME_PROP_KBD_CHAR_FIRST | 
                             IME_PROP_IGNORE_UPKEYS |
							 IME_PROP_END_UNLOAD; 

    lpIMEInfo->fdwConversionCaps = IME_CMODE_FULLSHAPE |
								IME_CMODE_NATIVE;

    lpIMEInfo->fdwSentenceCaps = IME_SMODE_NONE;
    lpIMEInfo->fdwUICaps = UI_CAP_2700;

	lpIMEInfo->fdwSCSCaps = 0;

    lpIMEInfo->fdwSelectCaps = SELECT_CAP_CONVERSION;

    _tcscpy(lpszUIClass,CLSNAME_UI);  // ע������뷨�������������ע�ᣬ�������뷨������������

    return TRUE;
}

/*
ϵͳ��������ӿ����ж�IME�Ƿ���ǰ��������
HIMC hIMC:����������
UINT uKey:��ֵ
LPARAM lKeyData: unknown
CONST LPBYTE lpbKeyState:����״̬,����256����״̬
return : TRUE-IME����,FALSE-ϵͳ����
ϵͳ�����ImeToAsciiEx������ֱ�ӽ�������Ϣ����Ӧ�ó���
*/
BOOL WINAPI ImeProcessKey(HIMC hIMC,UINT uKey,LPARAM lKeyData,CONST LPBYTE lpbKeyState)
{
	return FALSE;
}

/**********************************************************************/
/* ImeSelect()                                                        */
/* Return Value:                                                      */
/*      TRUE - successful, FALSE - failure                            */
/**********************************************************************/
BOOL WINAPI ImeSelect(HIMC hIMC,BOOL fSelect)
{
	MyLoadCilentDLLFun();   // ���л����뷨ʱ�ж��Ƿ���Ҫ���ؿͻ�DLL

    if (!hIMC) {
        return (FALSE);
    }
	if (fSelect==TRUE && LoadNextWhenActive!=0)
	{
		//ActivateKeyboardLayout((HKL)HKL_NEXT,0);  // ��Ҫ�ڸýӿ���ʹ�ô˺����л�����һ�����뷨������������ʱ���뷨�ֻ��л���ȥ
		MyGenerateMessage(hIMC,WM_INPUTLANGCHANGEREQUEST,1,(LPARAM)HKL_NEXT);
	}
    return TRUE;
}


/*
ʹһ�����������ļ������ʧ���֪ͨ���뷨���µ����������ģ������ڴ���һЩ��ʼ������
HIMC hIMC :����������
BOOL fFlag : TRUE if activated, FALSE if deactivated. 
Returns TRUE if successful, FALSE otherwise. 
*/
BOOL WINAPI ImeSetActiveContext(HIMC hIMC,BOOL fFlag)
{
	//ͨ��IME��Ϣ��ʵ�ִ���״̬�仯
    return TRUE;
}

/*
Causes the IME to arrange the composition string structure with the given data.
This function causes the IME to send the WM_IME_COMPOSITION message. 
Returns TRUE if successful, FALSE otherwise.
*/
BOOL WINAPI ImeSetCompositionString(HIMC hIMC, DWORD dwIndex, LPCVOID lpComp, DWORD dwComp, LPCVOID lpRead, DWORD dwRead)
{
    return FALSE;
}


/*
Ӧ�ó����������ӿ����������������ĵ�ת�������뷨����������ӿ���ת���û�������
UINT uVKey:��ֵ,�����ImeInquire�ӿ���ΪfdwProperty����������IME_PROP_KBD_CHAR_FIRST,����ֽ��������ֵ
UINT uScanCode:������ɨ���룬��ʱ��������ͬ���ļ�ֵ����ʱ��Ҫʹ��uScanCode������
CONST LPBYTE lpbKeyState:����״̬,����256����״̬
LPDWORD lpdwTransKey:��Ϣ����������������IMEҪ����Ӧ�ó������Ϣ����һ��˫���ǻ������������ɵ������Ϣ����
UINT fuState:Active menu flag(come from msdn)
HIMC hIMC:����������
return : ���ر�������Ϣ������lpdwTransKey�е���Ϣ����
*/
UINT WINAPI ImeToAsciiEx (UINT uVKey,UINT uScanCode,CONST LPBYTE lpbKeyState,LPDWORD lpdwTransKey,UINT fuState,HIMC hIMC)
{
    return 0;
}


//��Ӧ�ó��򷢸����뷨����Ϣ�����뷨�����ڴ���Ӧ�ó��������
//return : TRUE-��ȷ��Ӧ������,FALSE-����Ӧ
BOOL WINAPI NotifyIME(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue)
{
    BOOL bRet = FALSE;
    switch(dwAction)
    {
	case NI_OPENCANDIDATE:
		break;
	case NI_CLOSECANDIDATE:
		break;
	case NI_SELECTCANDIDATESTR:
		break;
	case NI_CHANGECANDIDATELIST:
		break;
	case NI_SETCANDIDATE_PAGESTART:
		break;
	case NI_SETCANDIDATE_PAGESIZE:
		break;
	case NI_CONTEXTUPDATED:
		switch (dwValue)
		{
		case IMC_SETCONVERSIONMODE:
			break;
		case IMC_SETSENTENCEMODE:
			break;
		case IMC_SETCANDIDATEPOS:
			break;
		case IMC_SETCOMPOSITIONFONT:
			break;
		case IMC_SETCOMPOSITIONWINDOW:
			break;
		case IMC_SETOPENSTATUS:
			break;
		default:
			break;
		}
		break;
		
	case NI_COMPOSITIONSTR:
		switch (dwIndex)
		{
		case CPS_COMPLETE:
			break;
		case CPS_CONVERT:
			break;
		case CPS_REVERT:
			break;
		case CPS_CANCEL:
			break;
		default:
			break;
		}
		break;
			
	default:
		break;
    }
    return bRet;
}


/**********************************************************************/
/* ImeRegsisterWord                                                   */
/* Return Value:                                                      */
/*      TRUE - successful, FALSE - failure                            */
/**********************************************************************/
BOOL WINAPI ImeRegisterWord(
    LPCTSTR lpszReading,
    DWORD   dwStyle,
    LPCTSTR lpszString)
{
    return (FALSE);
}

/**********************************************************************/
/* ImeUnregsisterWord                                                 */
/* Return Value:                                                      */
/*      TRUE - successful, FALSE - failure                            */
/**********************************************************************/
BOOL WINAPI ImeUnregisterWord(
    LPCTSTR lpszReading,
    DWORD   dwStyle,
    LPCTSTR lpszString)
{
    return (FALSE);
}

/**********************************************************************/
/* ImeGetRegsisterWordStyle                                           */
/* Return Value:                                                      */
/*      number of styles copied/required                              */
/**********************************************************************/
UINT WINAPI ImeGetRegisterWordStyle(
    UINT       nItem,
    LPSTYLEBUF lpStyleBuf)
{
    return (FALSE);
}

/**********************************************************************/
/* ImeEnumRegisterWord                                                */
/* Return Value:                                                      */
/*      the last value return by the callback function                */
/**********************************************************************/
UINT WINAPI ImeEnumRegisterWord(
    REGISTERWORDENUMPROC lpfnRegisterWordEnumProc,
    LPCTSTR              lpszReading,
    DWORD                dwStyle,
    LPCTSTR              lpszString,
    LPVOID               lpData)
{
    return (FALSE);
}


/**********************************************************************/
/*                                                                    */
/* UIWndProc()                                                        */
/*                                                                    */
/* ���뷨���洰�ڵĴ��ڴ������                                       */
/*                                                                    */
/**********************************************************************/
LRESULT WINAPI UIWndProc(HWND hUIWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    return 0;
}

LRESULT WINAPI StatusWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// ���뷨״̬���Ĵ��ڴ������
	return 0;
}

LRESULT WINAPI CompWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// ���뷨��ʾ��ѡ�ֵĴ��ڵĵĴ��ڴ������
	return 0;
}

LRESULT WINAPI CandWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// ���뷨���봰�ڵĴ��ڴ������
	return 0;
}



//*****************************************************************
//	��HIMC������Ϣ
//	���ô˺���ֱ�������뷨�������ڷ�����Ϣ
//*****************************************************************
BOOL MyGenerateMessage(HIMC hIMC, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BOOL bRet=FALSE;
    LPINPUTCONTEXT lpIMC= ImmLockIMC(hIMC);
	if(lpIMC == NULL)
		return FALSE;    

    if (IsWindow(lpIMC->hWnd))
    {
		PostMessage(lpIMC->hWnd,msg,wParam,lParam);
		bRet=TRUE;
    }

	ImmUnlockIMC(hIMC); 
	return bRet;
}