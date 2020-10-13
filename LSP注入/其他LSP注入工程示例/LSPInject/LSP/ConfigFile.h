// ConfigFile.h: interface for the ConfigFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGFILE_H__7D6BD22C_657B_4F3E_8752_4061EFB73101__INCLUDED_)
#define AFX_CONFIGFILE_H__7D6BD22C_657B_4F3E_8752_4061EFB73101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <vector>
#include <string>
using namespace std;
class ConfigFile  
{public:
	ConfigFile();
    ConfigFile(const char* sPath);
	virtual ~ConfigFile();
	
	void  LoadFile(const char* sPath ); //�ļ�·��
	
	void  GetSectionNames(vector<string> &ar);/*�õ����ж�*/	
	
	void  GetKeyNames(const char* lpSection/*����*/, vector<string> &ar);//�õ�ĳ��������key����
	DWORD GetValue(const char *lpSection/*����*/, const char *sKey/*Keyֵ*/, string& key);//�õ�����ֵ
	
	void  GetKey_Value(const char* lpSection/*����*/, vector<string> &ar );/*�õ��ö��µ�����key-value*/
	
	
	BOOL  Write(const char* lpSection, const char* lpKey, const char* lpValue);//д���ֵ
	BOOL  DeleteSection(const char* lpSection);//ɾ����
	BOOL  DeleteKey(const char* lpSection, const char* lpKey);//ɾ��key
	
protected:
	DWORD GetSectionNames(char* lpBuffer, DWORD dwBufSize);//�����ַ�����
	DWORD GetKey_Value(const char* lpSection, char* lpBuffer, DWORD dwBufSize);//�����ַ�����
	
	BOOL  ParseString( char* buff, vector<string> &ar );//�������Ρ���ֵ
	
	
protected:
	string m_sPath;

};

#endif // !defined(AFX_CONFIGFILE_H__7D6BD22C_657B_4F3E_8752_4061EFB73101__INCLUDED_)
