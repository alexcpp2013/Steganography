#include "stdafx.h"
#include "resource.h"
#include "The_David.h"
#include <stdio.h>
#include <commctrl.h>
#include <commdlg.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];								//Текст заголовка вікна
TCHAR szWindowClass[MAX_LOADSTRING];								// Текст заголовка вікна
HWND hWnd,hBut1,hBut2,hBut3,hBut4,hBut5,hBut6,hBut10,hBut15,
	hEdit1,hEdit2,hEdit3,hEdit4,
	hStatic1,hStatic2,hStatic3,hStatic4,hStatic5,hStatic6,hTrackbar1,hGroup1,hGroup2,hGroup3;

char *filename1,*filename2,*filename3;
char waste[2048];
char WAVfile=0; //0 або 1
char N=1;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void PaintMainWindow(HWND);
void SelectFile(char);
char VerifyAndHide();
char VerifyAndUnhide();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

    // Ініціалізація глобальних змінних
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_The_David_TOOLS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_The_David_TOOLS);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_The_David_TOOLS);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE-1);
	wcex.lpszMenuName	= (LPCSTR)IDC_The_David_TOOLS;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	RECT rt;
	hInst = hInstance;
	int w=0,h=0;

	filename1=(char*)malloc(2048);
	filename2=(char*)malloc(2048);
	filename3=(char*)malloc(2048);
	strncpy(filename1,"\0",1);
	strncpy(filename2,"\0",1);
	strncpy(filename3,"\0",1);
	
	w=GetSystemMetrics(SM_CXSCREEN);
	h=GetSystemMetrics(SM_CYSCREEN);
 
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,szWindowClass, szTitle,WS_OVERLAPPED|WS_MINIMIZEBOX|WS_CAPTION|WS_SYSMENU,260, 200, w-520, 340, NULL, NULL, hInstance, NULL);
	GetClientRect(hWnd,&rt);

	hGroup3=CreateWindow("BUTTON","Вибір файлів для обробки",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,10,2,rt.right-20,145,hWnd,(HMENU)IDC_BUTTON6,hInstance,NULL);
	
	hEdit1=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL,10,30,rt.right-214,25,hGroup3,(HMENU)IDC_EDIT1,hInstance,NULL);
	hEdit2=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL,10,70,rt.right-214,25,hGroup3,(HMENU)IDC_EDIT2,hInstance,NULL);
	hEdit3=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL,10,110,rt.right-214,25,hGroup3,(HMENU)IDC_EDIT3,hInstance,NULL);
  
	hBut1=CreateWindow("BUTTON","Вибрати контейнер...",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON,rt.right-190,30,170,25,hWnd,(HMENU)IDC_BUTTON1,hInstance,NULL);
	hBut2=CreateWindow("BUTTON","Вибрати повідомлення...",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON,rt.right-190,70,170,25,hWnd,(HMENU)IDC_BUTTON2,hInstance,NULL);
	hBut3=CreateWindow("BUTTON","Вибрати результат...",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON,rt.right-190,110,170,25,hWnd,(HMENU)IDC_BUTTON3,hInstance,NULL);
	
	
	
	hBut10=CreateWindow("BUTTON","Вихід",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON,rt.right-188,254,84,25,hWnd,(HMENU) IDM_EXIT,hInstance,NULL);
	hBut15=CreateWindow("BUTTON","Інформація",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON,rt.right-100,254,84,25,hWnd,(HMENU) IDM_ABOUT,hInstance,NULL);


	
	hGroup1=CreateWindow("BUTTON","Вибір кількості біт",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,10,150,rt.right/2+46,130,hWnd,(HMENU)IDC_BUTTON4,hInstance,NULL);

	hTrackbar1=CreateWindowEx(0,TRACKBAR_CLASS,"",WS_CHILD|WS_VISIBLE|WS_TABSTOP|TBS_AUTOTICKS|TBS_ENABLESELRANGE|TBS_HORZ,10,54,rt.right/2+28,46,hGroup1,(HMENU)IDC_TRACKBAR1,hInstance,NULL);	
	SendMessage(hTrackbar1,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(1,5)); 

	hStatic1=CreateWindow("STATIC","Кількість біт для заміни:",WS_CHILD|WS_VISIBLE,10,30,180,25,hGroup1,(HMENU)IDC_STATIC7,hInstance,NULL);
	hStatic2=CreateWindow("STATIC","1 біт",WS_CHILD|WS_VISIBLE|SS_LEFT,10,100,30,25,hGroup1,(HMENU)IDC_STATIC1,hInstance,NULL);
	hStatic3=CreateWindow("STATIC","2 біта",WS_CHILD|WS_VISIBLE|SS_LEFT,100,100,38,25,hGroup1,(HMENU)IDC_STATIC2,hInstance,NULL);
	hStatic4=CreateWindow("STATIC","3 біта",WS_CHILD|WS_VISIBLE|SS_LEFT,190,100,38,25,hGroup1,(HMENU)IDC_STATIC3,hInstance,NULL);
	hStatic5=CreateWindow("STATIC","4 біта",WS_CHILD|WS_VISIBLE|SS_LEFT,280,100,38,25,hGroup1,(HMENU)IDC_STATIC4,hInstance,NULL);
	hStatic6=CreateWindow("STATIC","6 біт",WS_CHILD|WS_VISIBLE|SS_LEFT,rt.right/2-0,100,38,25,hGroup1,(HMENU)IDC_STATIC6,hInstance,NULL);
	


	hBut5=CreateWindow("BUTTON","Сховати!",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,rt.right/2+58,158,rt.right/2-74,25,hWnd,(HMENU)IDC_BUTTON8,hInstance,NULL);
	hBut6=CreateWindow("BUTTON","Витягнути!",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,rt.right/2+58,200,rt.right/2-74,25,hWnd,(HMENU)IDC_BUTTON9,hInstance,NULL);

	if(!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	char numError=0,ErrorMessage[128];
		
	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam);
					
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
				    break;
				case IDM_OPENFILE1:
				case IDC_BUTTON1:
					if(wmEvent==BN_CLICKED)
						SelectFile(0);					
					SetWindowText(hEdit1,filename1);
					break;
				case IDM_OPENFILE2:
				case IDC_BUTTON2:
					if(wmEvent==BN_CLICKED)
						SelectFile(1);
					SetWindowText(hEdit2,filename2);
					break;
				case IDM_OPENFILE3:
				case IDC_BUTTON3:
					if(wmEvent==BN_CLICKED)
						SelectFile(2);
					SetWindowText(hEdit3,filename3);
					break;
				case IDM_STARTHIDE:
				case IDC_BUTTON8:
					if(wmEvent==BN_CLICKED)
					{
						numError=VerifyAndHide();
						switch(numError)
							{
							case 0:
								strcpy(ErrorMessage,"Дані успішно приховані!");
								break;
							case -1:
								strcpy(ErrorMessage,"Типи файлів \"Контейнера\" і \"Результата\" не співпадають!");
								break;							
							case 1:
								strcpy(ErrorMessage,"Файл \"Контейнер\" не досяжний!");
								break;
							case 2:
								strcpy(ErrorMessage,"Файл \"Повідомлення\" не досяжний!");
								break;
							case 3:
								strcpy(ErrorMessage,"Файл \"Результат\" не досяжний!");
								break;
							case 4:
								strcpy(ErrorMessage,"Файл не підтримується!");
								break;
							case 5:
								strcpy(ErrorMessage,"Мультимедійний файл не волновий!");
								break;
							case 6:
								strcpy(ErrorMessage,"Зіпсований формат файла!");
								break;
							case 7:
								strcpy(ErrorMessage,"Програма працює тільки з незжатими файлами!");
								break;
						
							default:
								strcpy(ErrorMessage,"Невідома Помилка");
							}
						MessageBox(hWnd,ErrorMessage,"Інформація.",0);
					}								
					break;
				case IDM_STARTUNHIDE:
				case IDC_BUTTON9:
					if(wmEvent==BN_CLICKED)
					{
						numError=VerifyAndUnhide();
						switch(numError)
							{
							case 0:
								strcpy(ErrorMessage,"Дані витягнуті успішно!");
								break;
							case 1:
								strcpy(ErrorMessage,"Файл \"Контейнер\" не досяжний!");
								break;
							case 2:
								strcpy(ErrorMessage,"Файл \"Повідомлення\" не досяжний!");
								break;
							case 4:
								strcpy(ErrorMessage,"Файл не підтримується!");
								break;
							case 5:
								strcpy(ErrorMessage,"Мультимедійний файл не волновий!");
								break;
							case 6:
								strcpy(ErrorMessage,"Зіпсований формат файла!");
								break;
							case 7:
								strcpy(ErrorMessage,"Програма працює тільки з незжатими файлами!");
								break;
						
							default:
								strcpy(ErrorMessage,"Невідома Помилка");
							}
						MessageBox(hWnd,ErrorMessage,"Інформація.",0);
					}								
					break;

				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			PaintMainWindow(hWnd);
			break;
		case WM_DESTROY:
			free(filename1);
			free(filename2);
			free(filename3);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

void PaintMainWindow(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	
	GetClientRect(hWnd, &rt);
	hdc = BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
}

void SelectFile(char opentype)
{
	char *defext="wav\0";
	char *filterstr1="Wave Files\0*.wav\0\0";
	
	char *filterstr2="All Files\0*.*\0\0";
	OPENFILENAME openfname;
	memset(&openfname,0,sizeof(OPENFILENAME));
	openfname.lStructSize=sizeof(OPENFILENAME);
	openfname.hwndOwner=hWnd;
	openfname.hInstance=NULL;
	openfname.lpstrCustomFilter=NULL;
	openfname.nMaxCustFilter=0;
	openfname.nMaxFile=2048;
	
	switch(opentype)
	{
	case 0:
		openfname.lpstrFilter=filterstr1;
		openfname.lpstrFile=filename1;
		break;
	case 1:
		openfname.lpstrFilter=filterstr2;
		openfname.lpstrFile=filename2;
		break;
	case 2:
		openfname.lpstrFilter=filterstr1;
		openfname.lpstrFile=filename3;
		break;
	}
	openfname.lpstrFileTitle=NULL;
	openfname.lpstrDefExt=defext;
	openfname.lpstrInitialDir=NULL;
	openfname.Flags=OFN_HIDEREADONLY;
	openfname.nFileOffset=0;
	openfname.nFileExtension=0;
	openfname.lCustData=0;
	openfname.lpfnHook=NULL;
	openfname.lpTemplateName=NULL;
	GetOpenFileName(&openfname);
	
	return;
}

char VerifyAndHide()
{
	TitleWave tw;
	
	FILE *file1,*file2,*file3;
	char ext1[4],ext2[4],*n;
	int file2handle;
	unsigned char *source,i,alreadyhidden=0;
	unsigned char mask1=1,mask2=0,bitsN;
	unsigned long bits24ton;

	GetWindowText(hEdit1,filename1,2048);
	GetWindowText(hEdit2,filename2,2048);
	GetWindowText(hEdit3,filename3,2048);
		
	N=SendMessage(hTrackbar1,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
	if(N==5)N=6;

	n=strchr(filename1,'\0');
	strncpy(ext1,n-3,4);
	n=strchr(filename3,'\0');
	strncpy(ext2,n-3,4);
	if(strncmp(ext1,ext2,3))
		return -1;
 WAVfile=0;

	if((file2handle=open(filename2,O_BINARY))==-1)
		return 2;
	if((file1=fopen(filename1,"rb"))==NULL)
		return 1;
	if((file2=fdopen(file2handle,"rb"))==NULL)
		return 2;
	if((file3=fopen(filename3,"wb"))==NULL)
		return 3;
	if(!WAVfile)
	{		
		fread(&tw,sizeof(TitleWave),1,file1);
		if(strncmp(tw.id_riff,"RIFF",4)!=0)return 4;
		if(strncmp(tw.id_chunk,"WAVE",4)!=0)return 5;
		if(strncmp(tw.fmt,"fmt ",4)!=0)return 6;
		if(tw.type!=1)return 7;
		fwrite(&tw,sizeof(TitleWave),1,file3);    
		source=(unsigned char*)malloc(tw.align);
	}
	
	for(i=0;i<N;i++)
		mask1*=2;
	mask1-=1;    
	mask2-=mask1+1;    
         
	i=24/N-1;
	srand(time(0));
    bits24ton=filelength(file2handle);
	
	alreadyhidden=0;
	if(!WAVfile)
	{
		while(!feof(file1))
		{
			fread(source,tw.align,1,file1);
			*(source+tw.align-1)&=mask2;
			bitsN=/*(unsigned char)*/(bits24ton>>(N*i));
			bitsN&=mask1;
			*(source+tw.align-1)|=bitsN;
			if(!i)
			{
				if(feof(file2))alreadyhidden=1;
				if(!alreadyhidden)fread(&bits24ton,3,1,file2);
				else bits24ton=(unsigned long)rand();
				i=24/N;
			}
			i--;
			fwrite(source,tw.align,1,file3);
		}
	} else
	{
		while(!feof(file1))
		{
			fread(source,1,1,file1);
			*source&=mask2;
			bitsN=(bits24ton>>(N*i));
			bitsN&=mask1;
			*source|=bitsN;
			if(!i)
			{
				if(feof(file2))alreadyhidden=1;
				if(!alreadyhidden)fread(&bits24ton,3,1,file2);
				else bits24ton=(unsigned long)rand();
				i=24/N;
			}
			i--;
			fwrite(source,1,1,file3);
		}
	}
	
	free(source);
	close(file2handle);
	fclose(file1);
	fclose(file2);
	fclose(file3);
	return 0;
}

char VerifyAndUnhide()
{
	TitleWave tw;
	
	FILE *file1,*file2;
	char ext1[4],*n;
	unsigned char *source,*target,i,alreadyhidden=0;
	unsigned char mask1=1,mask2=0;
	unsigned long int bits=0,bitmask;
	long int datasize=16777216,y=0;

	GetWindowText(hEdit1,filename1,2048);
	GetWindowText(hEdit2,filename2,2048);
	
	N=SendMessage(hTrackbar1,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
	if(N==5)N=6;

	n=strchr(filename1,'\0');
	strncpy(ext1,n-3,4);

	 WAVfile=0;
	
	if((file1=fopen(filename1,"rb"))==NULL)
		return 1;
	if((file2=fopen(filename2,"wb"))==NULL)
		return 2;
	if(!WAVfile)
	{		
		fread(&tw,sizeof(TitleWave),1,file1);
		if(strncmp(tw.id_riff,"RIFF",4)!=0)return 4;
		if(strncmp(tw.id_chunk,"WAVE",4)!=0)return 5;
		if(strncmp(tw.fmt,"fmt ",4)!=0)return 6;
		if(tw.type!=1)return 7;
		source=(unsigned char*)malloc(tw.align);
	}
	
	target=(unsigned char*)malloc(3);
	for(i=0;i<N;i++)
		mask1*=2;
	mask1-=1;    
	mask2-=mask1+1;    
         
	i=24/N-1;
		
	if(!WAVfile)
	{
		while(y<datasize+3)
		{
			fread(source,tw.align,1,file1);
			bitmask=(unsigned long)*(source+tw.align-1);
			bitmask&=mask1;
			bitmask<<=(N*i);
			bits|=bitmask;
			if(!i)
			{
				i=24/N;
				if(!y)datasize=bits;
				else
				{
					target=(unsigned char*)&bits;
					if(y<datasize)fwrite(target,3,1,file2);
					else fwrite(target,datasize%3,1,file2);
				}
				y+=3;
				bits=0;
			}
			i--;
		}

	} else
	{
		while(y<datasize+3)
		{
			fread(source,1,1,file1);
			bitmask=(unsigned long int)*source;
			bitmask&=mask1;
			bitmask<<=(N*i);
			bits|=bitmask;
			if(!i)
			{
				i=24/N;
				if(!y)datasize=bits;
				else
				{
					target=(unsigned char*)&bits;
					if(y<datasize)fwrite(target,3,1,file2);
					else fwrite(target,datasize%3,1,file2);
				}
				y+=3;
				bits=0;
			}
			i--;
		}
	}
	
	free(source);
	free(target);
	fclose(file1);
	fclose(file2);
	return 0;
}
