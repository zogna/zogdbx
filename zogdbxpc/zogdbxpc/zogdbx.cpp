//#include "stdafx.h"
#include"zogdbx.h"

/*
//返回 1 成功
//返回 0 无数据 但非致命错误
// 返回-1 文件无法打开或者数据读取错误 致命错误
*/

#define ZOGDBX_DEBUG 0

TCHAR zogdbxHourDIR[MAX_PATH]={0};
TCHAR zogdbxEveryDIR[MAX_PATH]={0};


#if (!RELEASE_BULID)
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow)
int main(void)
{

	
	double ee[100];
	double pp[100];
	int number;


	unsigned long int CurrentUTC=0;
	double  CurrentSummationPrice=0;
	double  CurrentSummationDelivered=0;

	double CurrentConsumeElect;
	double CurrentConsumePrice;
	
	unsigned long int LastUTC;
	double  LastSumElect;
	double  LastSumPrice;
	double  LastConsumeElect=0;
	double  LastConsumePrice=0;
	
	DWORD 	StartTime = GetTickCount(); 

	int timezone=-5;
		
	double  HourSumElect;
	double  HourSumPrice;
	double  HourConsumeElect;
	double  HourConsumePrice;
int s0year,s0month,s0day,s0hour,s0min,s0sec;

 zogdbx_ex_SetDbxDirEverytime(_T("a"));
 zogdbx_ex_SetDbxDirHourtime(_T("b"));
 
	zogdst_ex_ReadDSTtime(_T("zogdst.time"));


 //zogdbx_ex_SetDbxDirEverytime(_T("\\Background\\a"));
 //zogdbx_ex_SetDbxDirHourtime(_T("\\Background\\b"));
 
	//zogdst_ex_ReadDSTtime(_T("\\Flash Disk\\CID\\zogdst.time"));

	CurrentUTC=zogdbx_in_TIMEtoUTC(2011,11,1,0,0,0);
	printf("%lu\n",CurrentUTC);
	//getchar();
	unsigned long int i=0;
	double nnnnnn;
	double mmmm;
#if 0
	srand(0);
	for(i=0;i<300000; i++)
	{		//printf("i=%lu ",i+1);

#if 1
		CurrentSummationDelivered+=0.09;
		CurrentSummationPrice+=1;
#else
		//一天20KWH
		//nnnnnn=rand()/1000000.0;
		//一天200KWH
		nnnnnn=rand()/100000.0;
	
		mmmm=rand()/65535.0;

		CurrentSummationDelivered+=nnnnnn;
		CurrentSummationPrice+=nnnnnn*mmmm;
#endif
	//	if(i%30==0)
	//	{
		//	CurrentUTC+=60;
		//	continue;
		//}

		if(1==zogdbx_ex_GetLastEverytime(&LastUTC,&LastSumElect,&LastSumPrice,
										&LastConsumeElect,&LastConsumePrice))
		{

			//之前电量大于当前电量
			//之前时间大于当前时间
			if((LastSumElect > CurrentSummationDelivered) ||
				(LastUTC >CurrentUTC)				 )
			{
					zogdbx_ex_CheakAndSave(0,0,0,
							CurrentUTC,
							CurrentSummationDelivered,
							CurrentSummationPrice,
							CurrentSummationDelivered,
							CurrentSummationPrice);
			}
			else
			{
				CurrentConsumeElect=CurrentSummationDelivered-LastSumElect;
				CurrentConsumePrice=CurrentSummationPrice-LastSumPrice; 
		
				zogdbx_ex_CheakAndSave(LastUTC,LastSumElect,LastSumPrice,
							CurrentUTC,
							CurrentSummationDelivered,
							CurrentSummationPrice,
							CurrentConsumeElect,
							CurrentConsumePrice);
			}
		}
		else
		{
			zogdbx_ex_CheakAndSave(0,0,0,
							CurrentUTC,
							CurrentSummationDelivered,
							CurrentSummationPrice,
							CurrentSummationDelivered,
							CurrentSummationPrice);
		}
	/*	
		double ConsumeElect,ConsumePrice;
		zogdbx_ex_GetPreviousHour(CurrentUTC-3540,
									&ConsumeElect,
									&ConsumePrice);
		printf("%lu,%lu,%lf,%lf,%lf,%lf\n",CurrentUTC-3540,CurrentUTC,CurrentSummationDelivered,
							CurrentSummationPrice,ConsumeElect,ConsumePrice);
							*/
/*
		if(i%60==0)
		{
			 zogdst_ex_DSTHourReport(2012,3,5,timezone,1,
									&number,ee,pp);
printf("H----total=%d\n",number);
 for(int j=0;j<number;j++)
	 printf("%d,e=%lf,p=%lf\n",j,ee[j],pp[j]);

      zogdst_ex_DSTMonthDayReport(2012,3,timezone,1,
									&number,ee,pp);
  printf("M----total=%d\n",number);
 for(int j=0;j<number;j++)
	 printf("%d,e=%lf,p=%lf\n",j,ee[j],pp[j]);
 getchar();
		}
*/

#if 0
		zogdbx_in_UTCtoTIME(CurrentUTC,
							&s0year,&s0month,&s0day,&s0hour,&s0min,&s0sec);

		printf("curtime=%d,%d,%d,%d,%d\n",s0year,s0month,s0day,s0hour,s0min);

		double ConsumeElect,ConsumePrice;
		zogdbx_ex_GetPreviousHour(CurrentUTC-3540,
									&ConsumeElect,
									&ConsumePrice);
		printf("%lu,%lu,%lf,%lf,%lf,%lf\n",CurrentUTC-3540,CurrentUTC,CurrentSummationDelivered,
							CurrentSummationPrice,ConsumeElect,ConsumePrice);
	
		//if(0==i%60)
		//	printf("i=%lu\n",i/60);
			

double  TotalSumElect;
double  TotalSumPrice;

//要做时区 夏时令处理
unsigned long int UTC5time=CurrentUTC+timezone*3600;
/*
zogdbx_in_UTCtoTIME(UTC5time,
							&s0year,&s0month,&s0day,&s0hour,&s0min,&s0sec);

printf("%d,%d,%d,%d,%d\n",s0year,s0month,s0day,s0hour,s0min);
*/

zogdst_ex_UTCzero2UTCzone(CurrentUTC,timezone,1,
						  &UTC5time,
						  &s0year,&s0month,&s0day,&s0hour,&s0min,&s0sec);
printf("utc=%ld,%d,%d,%d,%d,%d\n",UTC5time,s0year,s0month,s0day,s0hour,s0min);

UTC5time=zogdst_ex_UTCzone2UTCzero(timezone,1,
											s0year,s0month,s0day,s0hour,s0min, s0sec);
printf("utc0=%ld\n",UTC5time);

zogdst_ex_UTCzero2UTCzone(zogdst_ex_UTCzone2UTCzero(timezone,1,
											s0year,s0month,s0day,s0hour,s0min, s0sec),timezone,1,
						  &UTC5time,
						  &s0year,&s0month,&s0day,&s0hour,&s0min,&s0sec);

printf("utc=%ld,%d,%d,%d,%d,%d\n",UTC5time,s0year,s0month,s0day,s0hour,s0min);

	zogdst_ex_DSTToday(s0year,s0month,s0day,timezone,1,&TotalSumElect,&TotalSumPrice);
	printf("today=%lf,%lf\n",TotalSumElect,TotalSumPrice);

	 TotalSumElect=0;
	TotalSumPrice=0;
	zogdst_ex_DSTYesterday(s0year,s0month,s0day,timezone,1,&TotalSumElect,&TotalSumPrice);
	printf("yesterday=%lf,%lf\n",TotalSumElect,TotalSumPrice);

	 TotalSumElect=0;
	TotalSumPrice=0;
	zogdst_ex_DSTLastMonth(s0year,s0month,timezone,1,&TotalSumElect,&TotalSumPrice);
	printf("month=%lf,%lf\n",TotalSumElect,TotalSumPrice);
	 TotalSumElect=0;
	TotalSumPrice=0;

	zogdst_ex_DSTLastNday(s0year,s0month,s0day,30,timezone,1,&TotalSumElect,&TotalSumPrice);
	printf("nday=%lf,%lf\n",TotalSumElect,TotalSumPrice);
	 TotalSumElect=0;
	TotalSumPrice=0;
	zogdst_ex_DSTLastWeek(s0year,s0month,s0day,1,timezone,1,&TotalSumElect,&TotalSumPrice);
	printf("week=%lf,%lf\n",TotalSumElect,TotalSumPrice);
#endif
	CurrentUTC+=60;
	}
#endif
	DWORD 	FinishinitTime = GetTickCount(); 


	printf("time=%ld\n",FinishinitTime-StartTime);

	
	zogdbx_ex_GetEveryHour(0,2012,02,02,18,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
	printf("%lf,%lf,%lf,%lf\n",HourSumElect,
									HourSumPrice,
									HourConsumeElect,
									HourConsumePrice);
	double  startHourSumElect;
	double  startHourSumPrice;
	double  startHourConsumeElect;
	double  startHourConsumePrice;
	unsigned int sday;
	double  endHourSumElect;
	double	endHourSumPrice;
	double  endHourConsumeElect;
	double  endHourConsumePrice;
	unsigned int eday;

	 int ggg=zogdbx_in_GetFirstEveryHour(201405,
									600,3200,
									&startHourSumElect,
									&startHourSumPrice,
									&startHourConsumeElect,
									&startHourConsumePrice,
									&sday);

	


		printf("%lf,%lf,%lf,%lf,%d,%d\n\n",startHourSumElect,
									startHourSumPrice,
									startHourConsumeElect,
									startHourConsumePrice,
									sday,ggg);

		zogdbx_in_GetLastEveryHour(201305,
									518,520,
									&endHourSumElect,
									&endHourSumPrice,
									&endHourConsumeElect,
									&endHourConsumePrice,
									&eday);
		
		printf("%lf,%lf,%lf,%lf,%d\n\n",endHourSumElect,
									endHourSumPrice,
									endHourConsumeElect,
									endHourConsumePrice,
									eday);
		zogdbx_ex_GetLastEverytime(&LastUTC,&LastSumElect,&LastSumPrice,
										&LastConsumeElect,&LastConsumePrice);
			printf("%lu,%lf,%lf\n",LastUTC,
									LastSumElect,
									LastSumPrice);
			
	StartTime = GetTickCount(); 
	
	//zogdbx_test_Today();
	//zogdbx_test_Yesterday();
	//zogdbx_test_LastWeek();
	//zogdbx_test_LastMonth();
	//zogdbx_test_Last99Day();
	//zogdbx_test_LastHour();


	FinishinitTime = GetTickCount();
	printf("time=%ld\n",FinishinitTime-StartTime);

	StartTime = GetTickCount(); 
	//zogdbx_test_DownloadHour();
	FinishinitTime = GetTickCount();
	printf("time=%ld\n",FinishinitTime-StartTime);


	//zogdbx_test_DownloadDay();
	
	StartTime = GetTickCount(); 
zogdst_ex_DownloadDSTHour(_T("a"),2011,3,13,timezone,1,0);
zogdst_ex_DownloadDSTHour(_T("a"),2011,11,4,timezone,1,0);
zogdst_ex_DownloadDSTHour(_T("a"),2011,11,5,timezone,1,1);
zogdst_ex_DownloadDSTHour(_T("a"),2011,11,6,timezone,1,0);
zogdst_ex_DownloadDSTHour(_T("a"),2011,11,7,timezone,1,0);
zogdst_ex_DownloadDSTHour(_T("a"),2011,11,8,timezone,1,0);
zogdst_ex_DownloadDSTDay(_T("a"),2011,12,10,2012,1,1,timezone,1,0);
zogdst_ex_DownloadDSTDay(_T("a"),2011,12,10,2012,1,1,timezone,1,1);
	FinishinitTime = GetTickCount(); 
	printf("time=%ld\n",FinishinitTime-StartTime);
	fprintf(stderr,"ok\n");

	zogdst_ex_DownloadDSTWeek(_T("a"),2011,6,timezone,1,0);
	zogdst_ex_DownloadDSTWeek(_T("a"),2012,0,timezone,1,1);
	zogdst_ex_DownloadDSTMonth(_T("a"),2011,timezone,1,0);
	zogdst_ex_DownloadDSTMonth(_T("a"),2012,timezone,1,1);

	 zogdst_ex_DSTHourReport(2012,3,1,timezone,1,
									&number,ee,pp);
printf("20120301 total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

	 zogdst_ex_DSTHourReport(2012,2,29,timezone,1,
									&number,ee,pp);
printf("20120229 total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

 zogdst_ex_DSTHourReport(2012,3,8,timezone,1,
									&number,ee,pp);
printf("20120308 total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

 zogdst_ex_DSTHourReport(2011,11,6,timezone,1,
									&number,ee,pp);
printf("20111106 total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

   zogdst_ex_DSTMonthReport(2011,timezone,1,
								&number,ee,pp);
 printf("2011 total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

   zogdst_ex_DSTMonthReport(2012,timezone,1,
								&number,ee,pp);
 printf("2012 total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

 /*
zogdst_ex_DSTWeekReport(2011,1,6,timezone,1,
								&number,ee,pp);
printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

zogdst_ex_DSTWeekReport(2011,2,5,timezone,1,
								&number,ee,pp);
printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

 zogdst_ex_DSTWeekReport(2011,3,5,timezone,1,
								&number,ee,pp);
printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

 zogdst_ex_DSTWeekReport(2011,4,6,timezone,1,
								&number,ee,pp);
printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

  zogdst_ex_DSTMonthReport(2011,timezone,1,
								&number,ee,pp);
 printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

  zogdst_ex_DSTWeekDayReport(2011,6,1,timezone,1,
									&number,ee,pp);
 printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

  zogdst_ex_DSTWeekDayReport(2011,6,2,timezone,1,
									&number,ee,pp);
 printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

   zogdst_ex_DSTWeekDayReport(2011,6,12,timezone,1,
									&number,ee,pp);
 printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);
 
   zogdst_ex_DSTWeekDayReport(2011,6,46,timezone,1,
									&number,ee,pp);
 printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);
 
   zogdst_ex_DSTWeekDayReport(2012,0,55,timezone,1,
									&number,ee,pp);
 printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);


  zogdst_ex_DSTMonthDayReport(2011,1,timezone,1,
									&number,ee,pp);
  printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

  zogdst_ex_DSTMonthDayReport(2011,2,timezone,1,
									&number,ee,pp);
  printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

  zogdst_ex_DSTMonthDayReport(2011,11,timezone,1,
									&number,ee,pp);
  printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

   zogdst_ex_DSTMonthDayReport(2012,2,timezone,1,
									&number,ee,pp);
  printf("total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);
	*/
     zogdst_ex_DSTMonthDayReport(2012,3,timezone,1,
									&number,ee,pp);
  printf("201203 total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);

     zogdst_ex_DSTMonthDayReport(2012,2,timezone,1,
									&number,ee,pp);
  printf("201202 total=%d\n",number);
 for(i=0;i<number;i++)
 	printf("%d,e=%lf,p=%lf\n",i,ee[i],pp[i]);


 for(int j=1;j<=30;j++)
 {
	 zogdst_ex_DownloadDSTHour(_T("c"),2011,11,j,timezone,1,1);
  zogdst_ex_DownloadDSTHour(_T("c"),2012,4,j,timezone,1,1);
 }
 for(int j=1;j<=31;j++)
 {
	 zogdst_ex_DownloadDSTHour(_T("c"),2011,12,j,timezone,1,1);
  zogdst_ex_DownloadDSTHour(_T("c"),2012,1,j,timezone,1,1);
    zogdst_ex_DownloadDSTHour(_T("c"),2012,3,j,timezone,1,1);
 }
 for(int j=1;j<=29;j++)
 {
	 zogdst_ex_DownloadDSTHour(_T("c"),2012,2,j,timezone,1,1);
 }


zogdst_ex_DownloadDSTMonthDay(_T("c"),2011,11,timezone,1,1);
zogdst_ex_DownloadDSTMonthDay(_T("c"),2011,12,timezone,1,1);
zogdst_ex_DownloadDSTMonthDay(_T("c"),2012,1,timezone,1,1);
zogdst_ex_DownloadDSTMonthDay(_T("c"),2012,2,timezone,1,1);
zogdst_ex_DownloadDSTMonthDay(_T("c"),2012,3,timezone,1,1);
zogdst_ex_DownloadDSTMonthDay(_T("c"),2012,4,timezone,1,1);
zogdst_ex_DownloadDSTMonth(_T("c"),2011,timezone,1,1);
	zogdst_ex_DownloadDSTMonth(_T("c"),2012,timezone,1,1);
	
	//getchar();
	return 0;
}
#endif 

//删除目录。含本目录及子目录及目录内所有文件
int zogdbx_ex_DelTree(LPCTSTR lpszPath)
{
	SHFILEOPSTRUCT FileOp;
	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_SILENT|FOF_NOERRORUI;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = lpszPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp);
}

//拷贝目录或文件	
//已废 被	CopyFile(源文件,目标文件,false=覆盖 true=不覆盖); 替换
/*
int zogdbx_ex_COPY(LPCTSTR fromPath,LPCTSTR toPath)
{
	SHFILEOPSTRUCT FileOp;
	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_SILENT|FOF_NOERRORUI;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = fromPath;
	FileOp.pTo = toPath;
	FileOp.wFunc = FO_COPY;
	return SHFileOperation(&FileOp);
}
*/

//设置数据库分钟的根目录
void zogdbx_ex_SetDbxDirEverytime(TCHAR *dir)
{
	//赋值给zogdbxDIR
	_sntprintf(zogdbxEveryDIR,MAX_PATH,_T("%s"),dir);
	//创建文件夹 小五增加SetFileAttributes(dir,FILE_ATTRIBUTE_HIDDEN);
	if(CreateDirectory(zogdbxEveryDIR, NULL))
		SetFileAttributes(dir,FILE_ATTRIBUTE_HIDDEN);
}

//设置数据库小时的根目录
void zogdbx_ex_SetDbxDirHourtime(TCHAR *dir)
{
	//赋值给zogdbxDIR
	_sntprintf(zogdbxHourDIR,MAX_PATH,_T("%s"),dir);
	//创建文件夹
	CreateDirectory(zogdbxHourDIR, NULL);
}

//设置数据库的所有文件
void zogdbx_ex_ClearDbxDir(void)
{
	//删除文件夹所有文件
	zogdbx_ex_DelTree(zogdbxEveryDIR);
	zogdbx_ex_DelTree(zogdbxHourDIR);
		
	//_tprintf(_T("delete %s\n"),zogdbxDIR);
	
	CreateDirectory(zogdbxEveryDIR, NULL);
	CreateDirectory(zogdbxHourDIR, NULL);
}
//设置数据库的分钟文件
void zogdbx_ex_ClearDbxEverytime(void)
{
	TCHAR zogdbxFILE[MAX_PATH]={0};
	//删除文件
	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%s"),zogdbxEveryDIR,EVERY_TIME_FILENAME);
	//	_tprintf(_T("delete %s\n"),zogdbxFILE);
	zogdbx_ex_DelTree(zogdbxFILE);
}

//进行处理
void zogdbx_ex_CheakAndSave(unsigned long int PrevUTC,
							double  PrevSumElect,
							double  PrevSumPrice,
							unsigned long int CurrentUTC,
							double  CurrentSumElect,
							double  CurrentSumPrice,
							double  CurrentConsumeElect,
							double  CurrentConsumePrice)
{
	unsigned long int FirstUTC;
	double  FirstSumElect;
	double  FirstSumPrice;
	double  FirstConsumeElect;
	double  FirstConsumePrice;
	
	unsigned long int filen;
	unsigned int time;

	double  HourSumElect;
	double  HourSumPrice;
	double  HourConsumeElect;
	double  HourConsumePrice;
	
	int cyear,cmonth,cday,chour,cmin,csec;
	int pyear,pmonth,pday,phour,pmin,psec;
	
	//有效值
	if(PrevUTC)
	{
		//正常模式
		zogdbx_in_UTCtoTIME(PrevUTC,&pyear,&pmonth,&pday,&phour,&pmin,&psec);
		zogdbx_in_UTCtoTIME(CurrentUTC,&cyear,&cmonth,&cday,&chour,&cmin,&csec);
		//切换增量
		if(( phour != chour) ||
			( pday != cday)   ||
			( pmonth != cmonth)   ||
			( pyear != cyear)  )
		{
			//对时进行封盘
			zogdbx_ex_GetFirstEverytime(&FirstUTC,&FirstSumElect,&FirstSumPrice,
											&FirstConsumeElect,&FirstConsumePrice);
			
			filen=(unsigned long int)pyear*100+pmonth;
			time=pday*100+phour;

			HourSumElect=PrevSumElect;
			HourSumPrice=PrevSumPrice;
			HourConsumeElect=PrevSumElect-FirstSumElect+FirstConsumeElect;
			HourConsumePrice=PrevSumPrice-FirstSumPrice+FirstConsumePrice;

			zogdbx_in_SaveEveryHour(filen,time,
										HourSumElect,
										HourSumPrice,
										HourConsumeElect,
										HourConsumePrice);
			//保存旧数据
			zogdbx_in_SaveOldEveryTime();
			//清空后建立
			zogdbx_ex_ClearDbxEverytime();
			zogdbx_in_SaveEveryTime(CurrentUTC, CurrentSumElect,CurrentSumPrice,
										CurrentConsumeElect, CurrentConsumePrice);
			
		}
		//还在同一个小时里
		else
		{
			//printf("Atime =%04d,%02d,%02d,%02d,%02d\n",cyear,cmonth,cday,chour,cmin);
			zogdbx_in_SaveEveryTime(CurrentUTC, CurrentSumElect,CurrentSumPrice,
										CurrentConsumeElect, CurrentConsumePrice);
		}
	}
	// 0 为初始化数据库 
	else
	{	
		zogdbx_ex_ClearDbxDir();
		zogdbx_in_SaveEveryTime(CurrentUTC, CurrentSumElect,CurrentSumPrice,
								CurrentConsumeElect, CurrentConsumePrice);
	}
}

//获取EVERYTIME中的第一个用电量 
//返回 1 成功
//返回 0 无数据 但非致命错误
// 返回-1 文件无法打开或者数据读取错误 致命错误
int zogdbx_ex_GetFirstEverytime(unsigned long int *FirstUTC,
										double  *FirstSumElect,
										double  *FirstSumPrice,
										double  *FirstConsumeElect,
										double  *FirstConsumePrice)
{
	struct EVERYTIME Everytime={0};

	FILE *fp=NULL;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	//获取Everytime结构体的字节
	size_t EverytimeBit= sizeof(struct EVERYTIME);

	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%s"),zogdbxEveryDIR,EVERY_TIME_FILENAME);

	int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		//读第一个结构体
		fread(&Everytime, EverytimeBit, 1, fp); 
		/*
		fscanf(fp,"%lu,%lf,%lf\n",
						&Everytime.CurrentUTC,
						&Everytime.CurrentSumElect,
						&Everytime.CurrentSumPrice);
		*/
		
		zogdbx_CloseFile(fp);
		if(Everytime.CurrentUTC)
		{
			*FirstUTC=Everytime.CurrentUTC;
			*FirstSumElect=Everytime.CurrentSumElect;
			*FirstSumPrice=Everytime.CurrentSumPrice;
			*FirstConsumeElect=Everytime.CurrentConsumeElect;
			*FirstConsumePrice=Everytime.CurrentConsumePrice;
			return 1;
		}
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetFirstEverytime  \nread data error CurrentUTC=0\n");
			#endif
			return -1;
		}
	}
	else
	{
		#if ZOGDBX_DEBUG
		printf("ERROR zogdbx_ex_GetFirstEverytime  \ncon't read %s file\n",zogdbxFILE);
		#endif
		return -1;
	}
}

//获取最后一次保存在数据库中的用电量 
//返回 1 成功
//返回 0 无数据 但非致命错误
// 返回-1 文件无法打开或者数据读取错误 致命错误
int zogdbx_ex_GetLastEverytime(unsigned long int *LastUTC,
										double  *LastSumElect,
										double  *LastSumPrice,
										double *LastConsumeElect,
										double *LastConsumePrice)
{
	struct EVERYTIME Everytime={0};

	FILE *fp=NULL;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	//获取Everytime结构体的字节
	long int EverytimeBit= sizeof(struct EVERYTIME);

	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%s"),zogdbxEveryDIR,EVERY_TIME_FILENAME);
	//一定要用RB AB WB
	int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		//定位到尾部前一个结构体
		fseek(fp, -EverytimeBit, SEEK_END); 
		//读最后一个结构体
		fread(&Everytime, EverytimeBit, 1, fp); 
		/*
		fseek(fp, -43, SEEK_END); 
		fscanf(fp,"%lu,%lf,%lf\n",
						&Everytime.CurrentUTC,
						&Everytime.CurrentSumElect,
						&Everytime.CurrentSumPrice);
		*/
		zogdbx_CloseFile(fp);
		if(Everytime.CurrentUTC)
		{
			*LastUTC=Everytime.CurrentUTC;
			*LastSumElect=Everytime.CurrentSumElect;
			*LastSumPrice=Everytime.CurrentSumPrice;
			*LastConsumeElect=Everytime.CurrentConsumeElect;
			*LastConsumePrice=Everytime.CurrentConsumePrice;
			return 1;
		}
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetLastEverytime  \nread data error CurrentUTC=0\n");
			#endif
			return -1;
		}
	}
	else
	{
		#if ZOGDBX_DEBUG
		printf("ERROR zogdbx_ex_GetLastEverytime  \ncon't read %s file\n",zogdbxFILE);
		#endif
		return -1;
	}
}

//实时的获取最近一小时内的耗电量和电价
//比如当前时间2012.03.05 14:23:55 
//返回2012.03.05 13:23:00->14:23:55的耗电量和价格
//返回1成功-1不成功
int zogdbx_ex_GetPreviousHour(unsigned long int PreviousUTC,
									double  *ConsumeElect,
									double  *ConsumePrice)
{
	struct EVERYTIME EverytimeA={0},EverytimeB={0};

	FILE *fp=NULL;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	double oldConsumeElect,newConsumeElect;
	double oldConsumePrice,newConsumePrice;
									
	//获取Everytime结构体的字节
	long int EverytimeBit= sizeof(struct EVERYTIME);

	if(0>=PreviousUTC )
		return -1;

	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%s"),zogdbxEveryDIR,OLD_EVERY_TIME_FILENAME);

#if 0
	//一定要用RB AB WB
	if( fp=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		while(!feof(fp))
		{
			//读第一个结构体
			flag=fread(&EverytimeA, EverytimeBit, 1, fp); 
			if(0==flag)
				break;
		
			if(EverytimeA.CurrentUTC >= PreviousUTC)
			{		
				#if ZOGDBX_DEBUG 
				printf("time=%lu,%lu\n",EverytimeA.CurrentUTC,PreviousUTC);
				#endif
				break;
			}
		}
		//读最后一个结构体
		fseek(fp, -EverytimeBit, SEEK_END); 
		//读最后一个结构体
		fread(&EverytimeB, EverytimeBit, 1, fp); 
	
		zogdbx_CloseFile(fp);
		
		if(EverytimeB.CurrentUTC >= PreviousUTC)
		{		
			oldConsumeElect=EverytimeB.CurrentSumElect-EverytimeA.CurrentSumElect+ 
													EverytimeA.CurrentConsumeElect;
			oldConsumePrice=EverytimeB.CurrentSumPrice-EverytimeA.CurrentSumPrice+
													EverytimeA.CurrentConsumePrice;
		}
		else
		{
			oldConsumeElect=0;
			oldConsumePrice=0;
		}
#endif 
		//加速 未验证
	//一定要用RB AB WB
	int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		//读最后一个结构体
		fseek(fp, -EverytimeBit, SEEK_END); 
		//读最后一个结构体
		fread(&EverytimeB, EverytimeBit, 1, fp); 
	
		if(EverytimeB.CurrentUTC >= PreviousUTC)
		{		
			fseek(fp, 0, SEEK_SET); 

			while(!feof(fp))
			{
				//读第一个结构体
				if(0==fread(&EverytimeA, EverytimeBit, 1, fp))
					break;
		
				if(EverytimeA.CurrentUTC >= PreviousUTC)
				{		
					#if ZOGDBX_DEBUG 
					printf("time=%lu,%lu\n",EverytimeA.CurrentUTC,PreviousUTC);
					#endif
					break;
				}
			}
			oldConsumeElect=EverytimeB.CurrentSumElect-EverytimeA.CurrentSumElect+ 
													EverytimeA.CurrentConsumeElect;
			oldConsumePrice=EverytimeB.CurrentSumPrice-EverytimeA.CurrentSumPrice+
													EverytimeA.CurrentConsumePrice;
		}
		else
		{
			oldConsumeElect=0;
			oldConsumePrice=0;
		}
		
		zogdbx_CloseFile(fp);
		
		#if ZOGDBX_DEBUG 
		printf("OLD=%lf,%lf\n",oldConsumeElect,oldConsumePrice);
		#endif

		if(1==zogdbx_ex_GetCurrentHour(&newConsumeElect,&newConsumePrice))
		{
			*ConsumeElect=oldConsumeElect+newConsumeElect;
			*ConsumePrice=oldConsumePrice+newConsumePrice;
			#if ZOGDBX_DEBUG 
			printf("a=%lf,%lf\n",*ConsumeElect,*ConsumePrice);
			#endif
			return 1;
		}
		else
		{
			*ConsumeElect=oldConsumeElect;
			*ConsumePrice=oldConsumePrice;
			#if ZOGDBX_DEBUG 
			printf("b=%lf,%lf\n",*ConsumeElect,*ConsumePrice);
			#endif
			return 1;
		}
	}
	
	if(1==zogdbx_ex_GetCurrentHour(&newConsumeElect,&newConsumePrice))
	{
		*ConsumeElect=newConsumeElect;
		*ConsumePrice=newConsumePrice;
		#if ZOGDBX_DEBUG 
		printf("c=%lf,%lf\n",*ConsumeElect,*ConsumePrice);
		#endif
		return 1;
	}
	else
	{
		*ConsumeElect=0;
		*ConsumePrice=0;
		return -1;
	}

}

//获取当前小时消耗的电量 价格
//计算今天2013.05.03日14:30:25
//现在分钟文件的所有数据14:00:00-14:30:25
//返回 1 成功 
// 0数据出错 
//-1 没有EVERYTIME文件
int zogdbx_ex_GetCurrentHour(double  *ConsumeElect,
									double  *ConsumePrice)
{
	struct EVERYTIME EverytimeA={0},EverytimeB={0};

	FILE *fp=NULL;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	//获取Everytime结构体的字节
	long int EverytimeBit= sizeof(struct EVERYTIME);

	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%s"),zogdbxEveryDIR,EVERY_TIME_FILENAME);

	int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		//读第一个结构体
		fread(&EverytimeA, EverytimeBit, 1, fp); 
		/*
		fscanf(fp,"%lu,%lf,%lf\n",
						&Everytime.CurrentUTC,
						&Everytime.CurrentSumElect,
						&Everytime.CurrentSumPrice);
		*/

		//定位到尾部前一个结构体
		fseek(fp, -EverytimeBit, SEEK_END); 
		//读最后一个结构体
		fread(&EverytimeB, EverytimeBit, 1, fp); 
		/*
		fseek(fp, -43, SEEK_END); 
		fscanf(fp,"%lu,%lf,%lf\n",
						&Everytime.CurrentUTC,
						&Everytime.CurrentSumElect,
						&Everytime.CurrentSumPrice);
		*/
		
		zogdbx_CloseFile(fp);
		
		if(EverytimeA.CurrentUTC && EverytimeB.CurrentUTC)
		{
			*ConsumeElect=EverytimeB.CurrentSumElect-EverytimeA.CurrentSumElect+ 
													EverytimeA.CurrentConsumeElect;
			*ConsumePrice=EverytimeB.CurrentSumPrice-EverytimeA.CurrentSumPrice+
													EverytimeA.CurrentConsumePrice;
			return 1;
		}
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetCurrentHour \nread data error CurrentUTC=0\n");
			#endif
			return -1;
		}
	}
	else
	{
		#if ZOGDBX_DEBUG
		printf("ERROR zogdbx_ex_GetCurrentHour  \ncon't read %s file\n",zogdbxFILE);
		#endif
		return -1;
	}
}

//获取小时的单个数据
//输入2012 02 03 12
//获取2012.02.03 12:00:00- 2012.02.03.12:59:59
//若使破损文件获取信息为
// 断电时间那个小时-> 2012.02.03 12:59:59
//比如断电时间2011.05.06 17:30:58
//则数据获取2011.05.06 17:00:00-> 2012.02.03 12:59:59
//返回 1 成功
//返回 0 无数据 但非致命错误
// 返回-1 文件无法打开或者数据读取错误 致命错误
int  zogdbx_ex_GetEveryHour(unsigned long int UTCtime,
									int UTCyear,
									int UTCmonth,
									int UTCday,
									int UTChour,
									double  *HourSumElect,
									double  *HourSumPrice,
									double  *HourConsumeElect,
									double  *HourConsumePrice)
{
	struct EVERYHOUR Everyhour={0};
	
	FILE *fp=NULL;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	int flag=0;
	unsigned int i=0;
	unsigned int time=0;

	int returnflag;

	unsigned long int filen;
	unsigned int dayhour;

	if(UTCyear!=0)
	{
		filen=(unsigned long int)UTCyear*100+UTCmonth;
		dayhour=UTCday*100+UTChour;
	}
	else if(UTCtime!=0)
	{
		int pyear,pmonth,pday,phour,pmin,psec;
		zogdbx_in_UTCtoTIME(UTCtime,&pyear,&pmonth,&pday,&phour,&pmin,&psec);
		filen=(unsigned long int)pyear*100+pmonth;
		dayhour=pday*100+phour;
	}
	else
	{
		#if ZOGDBX_DEBUG
		printf("ERROR zogdbx_ex_GetEveryHour \n no time\n");
		#endif
		return -1;
	}
	
	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.h"),zogdbxHourDIR,filen);

	//读取
	int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		flag=0;
		for(i=0;!feof(fp);i++)
		{
			if(fread(&time, sizeof(unsigned int), 1, fp))
			{
				//找到数据
				if(time == dayhour)
				{
					flag=1;
					break ;
				}
			}
			else 
				//文件到底
				break;
		}
		
		zogdbx_CloseFile(fp);
		
		//无数据
		if(0==flag)
		{
			_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.b"),zogdbxHourDIR,filen);

			int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
			if( 0==err )
			//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
			{	
				for(i=0;!feof(fp);i++)
				{
					fread(&Everyhour, sizeof(struct EVERYHOUR), 1, fp); 
			
					//找到数据头出问题
					if(Everyhour.HourTime  == dayhour)
					{
						zogdbx_CloseFile(fp);
						
						*HourSumElect=Everyhour.HourSumElect;
						*HourSumPrice=Everyhour.HourSumPrice;
						*HourConsumeElect=Everyhour.HourConsumeElect;
						*HourConsumePrice=Everyhour.HourConsumePrice;
						//修复
						zogdbx_in_RebuildHeadEveryHour(filen);
						return 1;
				
					}
				}
				zogdbx_CloseFile(fp);
				//真的没数据
				#if ZOGDBX_DEBUG
				printf("ERROR zogdbx_ex_GetEveryHour \n real nodata\n");
				#endif
				return 0;
			}
			else
			{
				#if ZOGDBX_DEBUG
				printf("ERROR zogdbx_ex_GetEveryHour \n con't read %s file\n",zogdbxFILE);
				#endif
				return -1;
			}
		}
		
		//找到头，开始找体
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.b"),zogdbxHourDIR,filen);

		int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
		if( 0==err )
		//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
		{	
			//读取
			fseek(fp, sizeof(struct EVERYHOUR)*i, SEEK_SET); 
			fread(&Everyhour, sizeof(struct EVERYHOUR), 1, fp); 
			zogdbx_CloseFile(fp);
			
			//找到数据
			if(Everyhour.HourTime == dayhour )
			{
				*HourSumElect=Everyhour.HourSumElect;
				*HourSumPrice=Everyhour.HourSumPrice;
				*HourConsumeElect=Everyhour.HourConsumeElect;
				*HourConsumePrice=Everyhour.HourConsumePrice;
				
				return 1;
			}
			else
			{
				//修补 恢复HH 然后再来一次，头出问题
				if( 1== (returnflag=zogdbx_in_RebuildHeadEveryHour(filen) ))
					return 	zogdbx_ex_GetEveryHour( UTCtime,UTCyear,UTCmonth,UTCday,UTChour,
													HourSumElect,
													HourSumPrice,
													HourConsumeElect,
													HourConsumePrice);
				else
				{
					#if ZOGDBX_DEBUG
					printf("ERROR zogdbx_ex_GetEveryHour \n nodata or file error\n");
					#endif
					return returnflag;
				}
			}
		}
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetEveryHour \n con't read %s file\n",zogdbxFILE);
			#endif
			return -1;
		}
	}
	else
	{
		//修补 恢复HH 然后再来一次没看到头文件
		if( 1== (returnflag=zogdbx_in_RebuildHeadEveryHour(filen) ))
			return 	zogdbx_ex_GetEveryHour(UTCtime,UTCyear,UTCmonth,UTCday,UTChour,
												HourSumElect,
												HourSumPrice,
												HourConsumeElect,
												HourConsumePrice);
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetEveryHour \n nodata or file error\n");
			#endif
			return returnflag;
		}
	}
}

//发UTC秒级返回这段时间内消耗电量 和花费电价
//返回 1成功
// 返回0成功但不完整 可能只有部分数据
//返回-1 信息未知文件出错 致命错误
int zogdbx_ex_GetBetweenHour_UTCT(unsigned long int starttime,
											unsigned long int endtime,
											double  *ConsumeElect,
											double  *ConsumePrice)
{
	int cyear,cmonth,cday,chour,cmin,csec;
	int pyear,pmonth,pday,phour,pmin,psec;
	
	zogdbx_in_UTCtoTIME(starttime,&pyear,&pmonth,&pday,&phour,&pmin,&psec);
	zogdbx_in_UTCtoTIME(endtime,&cyear,&cmonth,&cday,&chour,&cmin,&csec);
	
	return zogdbx_ex_GetBetweenHour_YMDH(pyear,
											pmonth,
											pday,
											phour,
											cyear,
											cmonth,
											cday,
											chour,
											ConsumeElect,
											ConsumePrice);
}

//发UTC年月日时返回这段时间内消耗电量 和花费电价
//返回 1成功
// 返回0成功但不完整 可能只有部分数据
//返回-1 信息未知文件出错 致命错误完全无数据
int zogdbx_ex_GetBetweenHour_YMDH(int startyear,
											int startmonth,
											int startday,
											int starthour,
											int endyear,
											int endmonth,
											int endday,
											int endhour,
											double  *ConsumeElect,
											double  *ConsumePrice)
{
	unsigned long int startfilen;
	unsigned int startdayhour;
	unsigned long int endfilen;
	unsigned int enddayhour;
	
	startfilen=(unsigned long int)startyear*100+startmonth;
	startdayhour=startday*100+starthour;
	
	endfilen=(unsigned long int)endyear*100+endmonth;
	enddayhour=endday*100+endhour;

	int returnflag;
	int flaga,flagb;
	
	double  StartSumElect,StartSumPrice,StartConsumeElect,StartConsumePrice;
	unsigned int Startday;
	double  EndSumElect,EndSumPrice,EndConsumeElect,EndConsumePrice;
	unsigned int Endday;
	
	unsigned long int i;
	//起始时间小于结束时间 则出错
	if((endfilen <= startfilen)  &&
		(enddayhour < startdayhour) )
		return -1;
									
	//跨文件不同年月
	if(startfilen !=endfilen)
	{	
		flaga=-1;
		
		flaga=zogdbx_in_GetFirstEveryHour(startfilen,
										startdayhour,3200,
										&StartSumElect,
										&StartSumPrice,
										&StartConsumeElect,
										&StartConsumePrice,
										&Startday);
		if(1!=flaga)
		{	//找N个文件
			for(i=startfilen+1;i<endfilen;i++)
			{
				if(13== i%100 )
				{
					//满一年 变成XXX101
					i=(i/100+1)*100+1;
					#if ZOGDBX_DEBUG
					printf("startyear=%d\n",i);
					#endif
				}	
				flaga=zogdbx_in_GetFirstEveryHour(i,
											0000,3200,
											&StartSumElect,
											&StartSumPrice,
											&StartConsumeElect,
											&StartConsumePrice,
											&Startday);
				if(1==flaga)
					break;
			}
			if(1!=flaga)
			{
				flaga=zogdbx_in_GetFirstEveryHour(endfilen,
										0000,enddayhour,
										&StartSumElect,
										&StartSumPrice,
										&StartConsumeElect,
										&StartConsumePrice,
										&Startday);
				//没找到值
				if(1!=flaga)
					return -1;
			}	
			//找到时间
			if((i==startfilen) && (startdayhour ==Startday))
				returnflag=1;
			else
				returnflag=0;
		}
		else
		{
			//找到
			if(startdayhour ==Startday)
				returnflag=1;
			else
				returnflag=0;
		}
	

		//逆序找
		flagb=zogdbx_in_GetLastEveryHour(endfilen,
											enddayhour,0000,
											&EndSumElect,
											&EndSumPrice,
											&EndConsumeElect,
											&EndConsumePrice,
											&Endday);
		if(1!=flagb)
		{
			for(i=endfilen-1;i>startfilen;i--)
			{
				if(0== i%100 )
				{
					//满一年 变成XXX912
					i=(i/100-1)*100+12;
					#if ZOGDBX_DEBUG
					printf("endyear=%d\n",i);
					#endif
				}
			
				flagb=zogdbx_in_GetLastEveryHour(i,
											3200,0000,
											&EndSumElect,
											&EndSumPrice,
											&EndConsumeElect,
											&EndConsumePrice,
											&Endday);
				if(1==flagb )
					break;
			}
			if(1!=flagb)
			{
				flagb=zogdbx_in_GetLastEveryHour(startfilen,
											3200,startdayhour,
											&EndSumElect,
											&EndSumPrice,
											&EndConsumeElect,
											&EndConsumePrice,
											&Endday);
				//没找到值
				if(1!=flagb)
					return -1;
			}
			
			//找到时间
			if((i==endfilen) && (enddayhour ==Endday))
				returnflag=1;
			else
				returnflag=0;
		}
		else
		{
			//找到
			if(enddayhour == Endday)
				returnflag=1;
			else
				returnflag=0;
		}
		
		//那么flaga肯定=1 flagb也肯定=1
		*ConsumeElect=EndSumElect-StartSumElect+StartConsumeElect;
		*ConsumePrice=EndSumPrice-StartSumPrice+StartConsumePrice;
											
		return returnflag;
	}
	else
	{
		#if ZOGDBX_DEBUG 
		printf("g\n");	
		#endif
		//不跨文件
		returnflag=zogdbx_in_GetFirstEveryHour(startfilen,
											startdayhour,enddayhour,
											&StartSumElect,
											&StartSumPrice,
											&StartConsumeElect,
											&StartConsumePrice,
											&Startday);
		#if ZOGDBX_DEBUG 
		printf("zogdbx_in_GetFirstEveryHour=%d\n",returnflag);
		#endif
		if(1==returnflag)	
		{
			returnflag=zogdbx_in_GetLastEveryHour(endfilen,
											enddayhour,startdayhour,
											&EndSumElect,
											&EndSumPrice,
											&EndConsumeElect,
											&EndConsumePrice,
											&Endday);
			#if ZOGDBX_DEBUG 
			printf("zogdbx_in_GetLastEveryHour=%d\n",returnflag);
			#endif
			if(1==returnflag)	
			{
				*ConsumeElect=EndSumElect-StartSumElect+StartConsumeElect;
				*ConsumePrice=EndSumPrice-StartSumPrice+StartConsumePrice;
											
				if( (startdayhour==Startday) && (enddayhour == Endday))
					//绝对成功
					return 1;
				else
					return 0;			
			}
			else
				return -1;
		}
		else
			return -1;
			
	}
}


/////////////////////////////////////////////////////////////////////////////////////
//HTML OUTPUT
/*
//输入FILE 指针
void zogdbx_ex_HTMLinit(FILE*fp)
{
	fprintf(fp,"<html>\n<body>\n");
}
void zogdbx_ex_HTMLend(FILE*fp)
{
	fprintf(fp,"</body>\n</html>\n");
}
*/
/*
// 输入FILE 指针以及表格标题名
// border=1显示边框cellpadding=10表格间距
void zogdbx_ex_HTMLtableinit(FILE *fp,char *caption,int border,int cellpadding)
{
	fprintf(fp,"<table border=\"%d\"\ncellpadding=\"%d\"\n>\n<caption>%s</caption>\n",
												border,cellpadding,caption);
}
*/
/*
void zogdbx_ex_HTMLtableend(FILE*fp)
{
	fprintf(fp,"</table>\n");
}
*/
/*
//此句代表一行的头
void zogdbx_ex_HTMLtablebody(FILE *fp,char name[][100],int num)
{
	int i;

	fprintf(fp,"<tr>\n");

	for(i=0;i<num;i++)
  	{
  		if(0==name[i][0])
  		{
			fprintf(fp,"<td>&nbsp;</td>\n");
		}
		else
		{
			fprintf(fp,"<td>%s</td>\n",name[i]);
		}
	}
  
	fprintf(fp,"</tr>\n");
}
*/
/*
//此句代表一行的体
void zogdbx_ex_HTMLtablehead(FILE *fp,char name[][100],int num)
{
	int i;

	fprintf(fp,"<tr>\n");

	for(i=0;i<num;i++)
  	{
  		if(0==name[i][0])
  		{
			fprintf(fp,"<th>&nbsp;</th>\n");
		}
		else
		{
			fprintf(fp,"<th>%s</th>\n",name[i]);
		}
	}
  
	fprintf(fp,"</tr>\n");
}
*/

/////////////////////////////////////////////////////////////////////////////////////

//递增每分钟的数据
//每次均保存 
//返回 1 成功
//返回 0 无数据 但非致命错误
// 返回-1 文件无法打开或者数据读取错误 致命错误
int  zogdbx_in_SaveEveryTime(unsigned long int CurrentUTC,
							double  CurrentSumElect,
							double  CurrentSumPrice,
							double  CurrentConsumeElect,
							double  CurrentConsumePrice)
{
	struct EVERYTIME Everytime=
	{
		CurrentUTC,
		CurrentSumElect,
		CurrentSumPrice,
		CurrentConsumeElect,
		CurrentConsumePrice
	};

	FILE *fp=NULL;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%s"),zogdbxEveryDIR,EVERY_TIME_FILENAME);

/*
a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留） 
*/
	int err=_tfopen_s(&fp,zogdbxFILE,_T("ab+"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("ab+")) )
	{	
		fwrite(&Everytime, sizeof(struct EVERYTIME), 1, fp); 
		/*
		fprintf(fp,"%010lu,%015.6lf,%015.6lf\n",
						Everytime.CurrentUTC,
						Everytime.CurrentSumElect,
						Everytime.CurrentSumPrice);
		*/
		zogdbx_CloseFile(fp);
		return 1;
	}
	else
	{
		#if ZOGDBX_DEBUG
		printf("ERROR zogdbx_ex_SaveEveryTime  \ncon't ab+ %s file\n",zogdbxFILE);
		#endif
		return -1;
	}
}

	
//保存 旧的每分钟的数据
//每次均保存 
//返回 1 成功
//返回 0 无数据 但非致命错误
// 返回-1 文件无法打开或者数据读取错误 致命错误
void  zogdbx_in_SaveOldEveryTime(void)
{
	TCHAR filen[MAX_PATH]={0},fileo[MAX_PATH]={0};
		
	_sntprintf(filen,MAX_PATH,_T("%s\\%s"),zogdbxEveryDIR,EVERY_TIME_FILENAME);
	_sntprintf(fileo,MAX_PATH,_T("%s\\%s"),zogdbxEveryDIR,OLD_EVERY_TIME_FILENAME);

	//删除旧的拷贝新的
	/*
	//_tprintf(_T("filen=%s fileo=%s\n"),filen,fileo);
	//zogdbx_ex_COPY(filen,fileo);
	//zogdbx_ex_COPY 被下面替换CopyFile
	*/
	CopyFile(filen,fileo,false);

	/*
	struct EVERYTIME Everytime={0};
	//获取Everytime结构体的字节
	size_t EverytimeBit= sizeof(struct EVERYTIME);
	
	if(FILE *fp=_tfopen(fileo,_T("rb")))
	{
		fread(&Everytime,EverytimeBit,1,fp);
		
		printf("%lu,%lf,%lf\n",
						Everytime.CurrentUTC,
						Everytime.CurrentSumElect,
						Everytime.CurrentSumPrice);
		
		zogdbx_CloseFile(fp);
	}
	*/
		
}
//保存到小时数据
//返回 1 成功
//返回 0 无数据 但非致命错误
// 返回-1 文件无法打开或者数据读取错误 致命错误
int  zogdbx_in_SaveEveryHour(unsigned long int filen,
								unsigned int time,
								double  HourSumElect,
								double  HourSumPrice,
								double  HourConsumeElect,
								double  HourConsumePrice)
{
	struct EVERYHOUR everyhour=
		{
			time,
			HourSumElect,
			HourSumPrice,
			HourConsumeElect,
			HourConsumePrice
		};

	FILE *fp=NULL;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.b"),zogdbxHourDIR,filen);

/*
a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留） 
*/
	int err=_tfopen_s(&fp,zogdbxFILE,_T("ab+"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("ab+")) )
	{	
		fwrite(&everyhour, sizeof(struct EVERYHOUR), 1, fp); 
		zogdbx_CloseFile(fp);
		#if ZOGDBX_DEBUG
		printf("%06lu.hb = %04d,%lf,%lf,%lf,%lf\n",filen,
			everyhour.HourTime,
			everyhour.HourSumElect,
			everyhour.HourSumPrice,
			everyhour.HourConsumeElect,
			everyhour.HourConsumePrice);
		#endif
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.h"),zogdbxHourDIR,filen);

		err=_tfopen_s(&fp,zogdbxFILE,_T("ab+"));
		if( 0==err )
		//if( fp=_tfopen(zogdbxFILE,_T("ab+")) )
		{
			fwrite(&everyhour.HourTime, sizeof(unsigned int), 1, fp); 
			zogdbx_CloseFile(fp);
			#if ZOGDBX_DEBUG
			printf("%06lu.hh= %04d\n",filen,
				everyhour.HourTime);
			#endif
			return 1;
		}
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_SaveEveryHour \n con't creat %s file\n",zogdbxFILE);
			#endif
			return -1;
		}
	}
	else
	{	
		#if ZOGDBX_DEBUG
		printf("ERROR zogdbx_ex_SaveEveryHour \n con't creat %s file\n",zogdbxFILE);
		#endif
		return -1;
	}

}

//获取单个文件小时的单个数据
//若此次数据没有。则找下一个
//直到小时文件 结束或者到达LIMITDAY
//返回 1 成功
//返回 0 无数据 但非致命错误到达文件末尾
// 返回-1 文件无法打开或者数据读取错误 致命错误
// 若 返回1  &&  outdayhour  ==dayhour 为成功找到
// 若 返回1   && outdayhour  ==limitday 为到达极限日
//若 返回1   && outdayhour  <limitday 为 接近dayhour的最近一小时
int  zogdbx_in_GetFirstEveryHour(unsigned long int filen,
									unsigned int dayhour,unsigned int limitday,
									double  *HourSumElect,
									double  *HourSumPrice,
									double  *HourConsumeElect,
									double  *HourConsumePrice,
									unsigned int *outdayhour)
{
	struct EVERYHOUR Everyhour={0};
	
	FILE *fp=NULL;

	int flag=0;
	unsigned int i=0;
	unsigned int time=0;

	int returnflag;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.h"),zogdbxHourDIR,filen);

	//读取
	int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		flag=0;
		for(i=0;!feof(fp);i++)
		{
			if(fread(&time, sizeof(unsigned int), 1, fp))
			{
				//找到数据
				if((time >= dayhour) && (time <=limitday))
				{
					flag=1;
					break ;
				}
			}
			else 
				//文件到底
				break;
		}
		
		zogdbx_CloseFile(fp);
		
		//无数据
		if(0==flag)
		{
			_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.b"),zogdbxHourDIR,filen);
			
			err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
			if( 0==err )
			//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
			{	
				for(i=0;!feof(fp);i++)
				{
					fread(&Everyhour, sizeof(struct EVERYHOUR), 1, fp); 
			
					//找到数据头出问题
					if((Everyhour.HourTime  >= dayhour) && (Everyhour.HourTime <=limitday))
					{
						zogdbx_CloseFile(fp);
						*outdayhour=Everyhour.HourTime;
						*HourSumElect=Everyhour.HourSumElect;
						*HourSumPrice=Everyhour.HourSumPrice;
						*HourConsumeElect=Everyhour.HourConsumeElect;
						*HourConsumePrice=Everyhour.HourConsumePrice;
						
						//修复
						zogdbx_in_RebuildHeadEveryHour(filen);
						
						return 1;
					}
				}
				zogdbx_CloseFile(fp);
				#if ZOGDBX_DEBUG
				//真的没数据
				printf("ERROR zogdbx_ex_GetFirstEveryHour \n NODATA\n");
				#endif
				return 0;
			}
			else
			{
				#if ZOGDBX_DEBUG
				printf("ERROR zogdbx_ex_GetFirstEveryHour con't read %s file\n",zogdbxFILE);
				#endif
				return -1;
			}
		}
		
		//找到头，开始找体
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.b"),zogdbxHourDIR,filen);

		err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
		if( 0==err )
		//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
		{	
			//读取
			fseek(fp, sizeof(struct EVERYHOUR)*i, SEEK_SET); 
			fread(&Everyhour, sizeof(struct EVERYHOUR), 1, fp); 
			zogdbx_CloseFile(fp);
			
			//找到数据
			if((Everyhour.HourTime >= dayhour) && (Everyhour.HourTime <=limitday))
			{
				*outdayhour=Everyhour.HourTime;
				*HourSumElect=Everyhour.HourSumElect;
				*HourSumPrice=Everyhour.HourSumPrice;
				*HourConsumeElect=Everyhour.HourConsumeElect;
				*HourConsumePrice=Everyhour.HourConsumePrice;
				
				return 1;
			}
			else
			{
				//修补 恢复HH 然后再来一次没看到头文件
				if( 1== (returnflag=zogdbx_in_RebuildHeadEveryHour(filen) ))
					return 	zogdbx_in_GetFirstEveryHour(filen,dayhour,limitday,
													HourSumElect,
													HourSumPrice,
													HourConsumeElect,
													HourConsumePrice,
													outdayhour);
				else
				{
					#if ZOGDBX_DEBUG
					printf("ERROR zogdbx_ex_GetFirstEveryHour \n nodata or file error\n");
					#endif
					return returnflag;
				}
			}
		}
		else
		{	
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetFirstEveryHour con't read %s file\n",zogdbxFILE);
			#endif
			return -1;
		}
	}
	else
	{
		//修补 恢复HH 然后再来一次没看到头文件
		if( 1== (returnflag=zogdbx_in_RebuildHeadEveryHour(filen) ))
			return 	zogdbx_in_GetFirstEveryHour(filen,dayhour,limitday,
													HourSumElect,
													HourSumPrice,
													HourConsumeElect,
													HourConsumePrice,
													outdayhour);
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetFirstEveryHour \n nodata or file error\n");
			#endif
			return returnflag;
		}
	}
}

//获取单个小时文件的单个数据
//若此次数据没有。则找下一个
//直到小时文件 结束或者到达LIMITDAY
//返回 1 成功
//返回 0 无数据 但非致命错误到达文件末尾
// 返回-1 文件无法打开或者数据读取错误 致命错误
// 若 返回1  &&  outdayhour  ==dayhour 为成功找到
// 若 返回1   && outdayhour  ==limitday 为到达极限日
//若 返回1   && outdayhour  <limitday 为 接近dayhour的最近一小时
int  zogdbx_in_GetLastEveryHour(unsigned long int filen,
									unsigned int dayhour,unsigned int limitday,
									double  *HourSumElect,
									double  *HourSumPrice,
									double  *HourConsumeElect,
									double  *HourConsumePrice,
									unsigned int *outdayhour)
{
	struct EVERYHOUR Everyhour={0};
	
	FILE *fp=NULL;

	int flag=0;
	unsigned int i=0;
	unsigned int time=0;
	unsigned int mini=0;
	int returnflag;
	TCHAR zogdbxFILE[MAX_PATH]={0};
	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.h"),zogdbxHourDIR,filen);

	//读取
	int err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
	if( 0==err )
	//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		mini=0;
		flag=0;
		//注意下标从1开始
		for(i=0;!feof(fp);i++)
		{
			if(fread(&time, sizeof(unsigned int), 1, fp))
			{
				//找到数据
				if(time <= dayhour )
				{
					flag=1;
					mini=i;
				}
				else
					break;
			}
			else 
				//文件到底
				break;
		}
		
		zogdbx_CloseFile(fp);
		
		//无数据
		if(0==flag)
		{
			_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.b"),zogdbxHourDIR,filen);
			
			err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
			if( 0==err )
			//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
			{	
				mini=0;
				flag=0;
				for(i=0;!feof(fp);i++)
				{
					if(fread(&Everyhour, sizeof(struct EVERYHOUR), 1, fp))
					{
						//找到数据除非文件坏了 否则肯定有数据
						if(Everyhour.HourTime <= dayhour )
						{
							flag=1;
							mini=i;
						}	
						else
							break;
					}
					else 
						//文件到底
						break;
				}
				if(0==flag)
				{
					zogdbx_CloseFile(fp);
					
					//真的没数据
					#if ZOGDBX_DEBUG
					printf("ERROR zogdbx_ex_GetLastEveryHour \n NODATA\n");
					#endif
					return 0;
				}
				else
				{
					fseek(fp, sizeof(struct EVERYHOUR)*mini, SEEK_SET); 
					fread(&Everyhour, sizeof(struct EVERYHOUR), 1, fp); 
					zogdbx_CloseFile(fp);
					//找到数据头出问题
					if((Everyhour.HourTime  <= dayhour) && (Everyhour.HourTime >=limitday))
					{
						*outdayhour=Everyhour.HourTime;
						*HourSumElect=Everyhour.HourSumElect;
						*HourSumPrice=Everyhour.HourSumPrice;
						*HourConsumeElect=Everyhour.HourConsumeElect;
						*HourConsumePrice=Everyhour.HourConsumePrice;
						
						//修复
						zogdbx_in_RebuildHeadEveryHour(filen);
						return 1;
					}
					else
					{
						//读写错误
						#if ZOGDBX_DEBUG
						printf("ERROR zogdbx_ex_GetLastEveryHour \n error DATA\n");
						#endif
						return -1;
					}
				}
				
			}
			else
			{
				//没体
				//真的没数据
				#if ZOGDBX_DEBUG
				printf("ERROR zogdbx_ex_GetLastEveryHour \n con't read %s file\n",zogdbxFILE);
				#endif
				return -1;
			}
		}
		
		//找到头，开始找体
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.b"),zogdbxHourDIR,filen);
		
		err=_tfopen_s(&fp,zogdbxFILE,_T("rb"));
		if( 0==err )
		//if( fp=_tfopen(zogdbxFILE,_T("rb")) )
		{	
			//读取
			fseek(fp, sizeof(struct EVERYHOUR)*mini, SEEK_SET); 
			fread(&Everyhour, sizeof(struct EVERYHOUR), 1, fp); 
			zogdbx_CloseFile(fp);
			
			//找到数据
			if((Everyhour.HourTime <= dayhour) && (Everyhour.HourTime >=limitday))
			{
				*outdayhour=Everyhour.HourTime;
				*HourSumElect=Everyhour.HourSumElect;
				*HourSumPrice=Everyhour.HourSumPrice;
				*HourConsumeElect=Everyhour.HourConsumeElect;
				*HourConsumePrice=Everyhour.HourConsumePrice;
				
				return 1;
			}
			else
			{
				//修补 恢复HH 然后再来一次，头出问题
				if( 1== (returnflag=zogdbx_in_RebuildHeadEveryHour(filen)) )
					return 	zogdbx_in_GetLastEveryHour(filen,dayhour,limitday,
													HourSumElect,
													HourSumPrice,
													HourConsumeElect,
													HourConsumePrice,
													outdayhour);
				else
				{
					#if ZOGDBX_DEBUG
					printf("ERROR zogdbx_ex_GetLastEveryHour \n nodata or file error\n");
					#endif
					return returnflag;
				}
			}
		}
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetLastEveryHour \n con't read %s file\n",zogdbxFILE);
			#endif
			return -1;
		}
	}
	else
	{
		//修补 恢复HH 然后再来一次没看到头文件
		if( 1== (returnflag=zogdbx_in_RebuildHeadEveryHour(filen)) )
			return 	zogdbx_in_GetLastEveryHour(filen,dayhour,limitday,
													HourSumElect,
													HourSumPrice,
													HourConsumeElect,
													HourConsumePrice,
													outdayhour);
		else
		{
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_GetLastEveryHour \n nodata or file error\n");
			#endif
			return returnflag;
		}
	}
}

//根据体文件 修复头文件
// 返回1 修复完成
//返回 0  无HB体数据 无法修复 
//返回 -1 无法创建体文件
int  zogdbx_in_RebuildHeadEveryHour(unsigned long int filen)
{
	FILE *fpb=NULL,*fph=NULL;
	TCHAR zogdbxFILE[MAX_PATH]={0};

	struct EVERYHOUR everyhour={0};
	
	_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.b"),zogdbxHourDIR,filen);
	//看看有没体文件
	int err=_tfopen_s(&fpb,zogdbxFILE,_T("rb"));
	if( 0==err )
	//if( fpb=_tfopen(zogdbxFILE,_T("rb")) )
	{	
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\%06lu.h"),zogdbxHourDIR,filen);

		err=_tfopen_s(&fph,zogdbxFILE,_T("wb+"));
		if( 0==err )
		//if( fph=_tfopen(zogdbxFILE,_T("wb+")) )
		{
			fread(&everyhour, sizeof(struct EVERYHOUR), 1, fpb); 
			fwrite(&everyhour.HourTime, sizeof(unsigned int), 1, fph); 
			while(!feof(fpb))
			{
				fread(&everyhour, sizeof(struct EVERYHOUR), 1, fpb); 
				fwrite(&everyhour.HourTime, sizeof(unsigned int), 1, fph); 
			}
			zogdbx_CloseFile(fph);
			zogdbx_CloseFile(fpb);
			return 1;
		}
		else
		{
			//头文件无法创建
			zogdbx_CloseFile(fpb);
			#if ZOGDBX_DEBUG
			printf("ERROR zogdbx_ex_RebuildHeadEveryHour \n con't creat %s file\n",zogdbxFILE);
			#endif
			return -1;
		}
	}
	else
	{
		//没体文件所以也没头文件
		#if ZOGDBX_DEBUG
		printf("ERROR zogdbx_ex_RebuildHeadEveryHour \n  no %s file\n",zogdbxFILE);
		#endif
		return 0;
	}

}
#if 0
/////////////////////////////////////////////////////////////////////////////////////
//测试演示 使用UTC0时间
void zogdbx_test_Last99Day(void)
{
	double  ConsumeElect;
	double  ConsumePrice;

	int flag=0;
	//一月
	//计算从2013.07.01  00:00:00  - 2014.01.31 23:59:59
	flag=zogdbx_ex_GetBetweenHour_YMDH(2010,7,1,0,
										2019,1,31,23,
										&ConsumeElect,
										&ConsumePrice);
	
	printf("Last99Day=%d,%lf,%lf\n",flag,ConsumeElect,ConsumePrice);
	
}

//测试演示 使用UTC0时间
void zogdbx_test_LastMonth(void)
{
	double  ConsumeElect;
	double  ConsumePrice;

	int flag=0;
	//一月
	//计算从2014.01.01  00:00:00  - 2014.01.31 23:59:59
	flag=zogdbx_ex_GetBetweenHour_YMDH(2014,1,1,0,
										2014,1,31,23,
										&ConsumeElect,
										&ConsumePrice);
	
	printf("Month=%d,%lf,%lf\n",flag,ConsumeElect,ConsumePrice);
	
}
//测试演示 使用UTC0时间
void zogdbx_test_LastWeek(void)
{
	double  ConsumeElect;
	double  ConsumePrice;

	int flag=0;
	//一周
	//计算从2013.12.27  00:00:00  - 2014.01.02 23:59:59
	flag=zogdbx_ex_GetBetweenHour_YMDH(2013,12,27,0,
										2014,1,2,23,
										&ConsumeElect,
										&ConsumePrice);
	
	printf("Week=%d,%lf,%lf\n",flag,ConsumeElect,ConsumePrice);
	
}

//测试演示 使用UTC0时间
void zogdbx_test_Yesterday(void)
{
	double  ConsumeElect;
	double  ConsumePrice;

	int flag=0;

	//计算从2013.05.03  00:00:00  - 2013.05.03 23:59:59
	flag=zogdbx_ex_GetBetweenHour_YMDH(2013,5,3,0,
										2013,5,3,23,
										&ConsumeElect,
										&ConsumePrice);
	
	printf("Yesterday=%d,%lf,%lf\n",flag,ConsumeElect,ConsumePrice);
	
}

//测试演示 使用UTC0时间
void zogdbx_test_Today(void)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  ConsumeElect;
	double  ConsumePrice;

	int flag=0;

	//计算今天2013.05.03日14:30:25
	//从2013.05.03  00:00:00  - 2013.05.03 23:59:59
	//先从小时文件中得出数据
	flag=zogdbx_ex_GetBetweenHour_YMDH(2013,5,3,0,
										2013,5,3,23,
										&HourConsumeElect,
										&HourConsumePrice);

	zogdbx_ex_GetCurrentHour(&ConsumeElect,&ConsumePrice);
	ConsumeElect +=HourConsumeElect;
	ConsumePrice +=HourConsumePrice;

	printf("TodayA=%d,%lf,%lf\n",flag,ConsumeElect,ConsumePrice);
	
	//或者用下面代码
	//从2013.05.03  00:00:00  - 2013.05.03 13:59:59
	//先从小时文件中得出数据
	flag=zogdbx_ex_GetBetweenHour_YMDH(2013,5,3,0,
										2013,5,3,13,
										&HourConsumeElect,
										&HourConsumePrice);

	//再加上现在分钟文件的所有数据14:00:00-14:30:25
	zogdbx_ex_GetCurrentHour(&ConsumeElect,&ConsumePrice);
	ConsumeElect +=HourConsumeElect;
	ConsumePrice +=HourConsumePrice;
									
	printf("TodayB=%d,%lf,%lf\n",flag,ConsumeElect,ConsumePrice);
	
}

//测试演示 使用UTC0时间
void zogdbx_test_LastHour(void)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	int flag=0;

	//计算今天2013.05.03日14:30:25
	//从2013.05.03  13:00:00  - 2013.05.03 13:59:59
	//先从小时文件中得出数据
	flag=zogdbx_ex_GetBetweenHour_YMDH(2013,5,3,13,
										2013,5,3,13,
										&HourConsumeElect,
										&HourConsumePrice);
	
	printf("LastHourA=%d,%lf,%lf\n",flag,HourConsumeElect,HourConsumePrice);
	//或者用下面代码
	// 速度更快
	flag=zogdbx_ex_GetEveryHour(0,
									2013,5,3,13,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
									
	printf("LastHourB=%d,%lf,%lf\n",flag,HourConsumeElect,HourConsumePrice);
	
}

void zogdbx_test_DownloadHour(void)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	double startElect,startPrice;
	double endElect,endPrice;
	
	int flag=0,first=0;
	int i;
	int total=0;

	FILE *fp;
	char name[100]={0};
	
	//不可以有B
	if( !(fp=_tfopen(_T("hour.html"),_T("w+"))) )
		return ;
	
	zogdbx_ex_HTMLinit(fp);

	zogdbx_ex_HTMLtableinit(fp,"Date Report 1",1,10);

	zogdbx_ex_HTMLtableRowinit(fp);
	zogdbx_ex_HTMLtableheadStr(fp,"yyyy/mm/dd hh->hh");
	zogdbx_ex_HTMLtableheadStr(fp,"ConsumeElect");
	zogdbx_ex_HTMLtableheadStr(fp,"ConsumePrice");
	zogdbx_ex_HTMLtableRowend(fp);
	
	total=0;
	for(i=0;i<24;i++)
	{
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									2013,5,10,i,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag && 0==first)
		{
			startElect=HourSumElect-HourConsumeElect;
			startPrice=HourSumPrice-HourConsumePrice;
			first=1;
		}

		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		sprintf(name,"%04d/%02d/%02d %02d->%02d",2013,5,10,i,i+1);
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(1==flag)
		{
			//有效时间+1
			total++;
			//结束
			endElect=HourSumElect;
			endPrice=HourSumPrice;
			zogdbx_ex_HTMLtablebodyDouble(fp,HourConsumeElect);
			zogdbx_ex_HTMLtablebodyDouble(fp,HourConsumePrice);
		
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
		
	}
	zogdbx_ex_HTMLtableend(fp);

	zogdbx_ex_HTMLtableinit(fp,"Date Report 2",1,10);

	zogdbx_ex_HTMLtableRowinit(fp);
	zogdbx_ex_HTMLtableheadStr(fp,"yyyy/mm/dd ");
	zogdbx_ex_HTMLtableheadStr(fp,"SumElect");
	zogdbx_ex_HTMLtableheadStr(fp,"SumPrice");
	zogdbx_ex_HTMLtableheadStr(fp,"AverageElect");
	zogdbx_ex_HTMLtableheadStr(fp,"AveragePrice");
	zogdbx_ex_HTMLtableRowend(fp);

	zogdbx_ex_HTMLtableRowinit(fp);
	sprintf(name,"%04d/%02d/%02d",2013,5,3);
	zogdbx_ex_HTMLtablebodyStr(fp,name);
	// 部分 或全部有值
	
	if(1==first)
	{
		zogdbx_ex_HTMLtablebodyDouble(fp,endElect-startElect);
		zogdbx_ex_HTMLtablebodyDouble(fp,endPrice-startPrice);
		zogdbx_ex_HTMLtablebodyDouble(fp,(endElect-startElect)/(double)total);
		zogdbx_ex_HTMLtablebodyDouble(fp,(endPrice-startPrice)/(double)total);
	}
	else
	{
		//无数据
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
	}
	zogdbx_ex_HTMLtableRowend(fp);
	
	zogdbx_ex_HTMLtableend(fp);
	
	zogdbx_ex_HTMLend(fp);
	zogdbx_CloseFile(fp);
									
}

void zogdbx_test_DownloadDay(void)
{
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0,first=0;
	int i;
	int total=0;

	
	FILE *fp;
	char name[100]={0};
	
	//不可以有B
	if( !(fp=_tfopen(_T("day.html"),_T("w+"))) )
		return ;
	
	zogdbx_ex_HTMLinit(fp);

	zogdbx_ex_HTMLtableinit(fp,"Date Report 1",1,10);

	zogdbx_ex_HTMLtableRowinit(fp);
	zogdbx_ex_HTMLtableheadStr(fp,"yyyy/mm/dd");
	zogdbx_ex_HTMLtableheadStr(fp,"ConsumeElect");
	zogdbx_ex_HTMLtableheadStr(fp,"ConsumePrice");
	zogdbx_ex_HTMLtableRowend(fp);
	
	total=0;
	for(i=3;i<20;i++)
	{
		//获取每个小时的值
		flag=zogdbx_ex_GetBetweenHour_YMDH(2013,5,i,0,
										2013,5,i,23,
										&ConsumeElect,
										&ConsumePrice);
		
		//获取初始值用于以后统计
		if(-1!=flag && 0==first)
		{
			first=1;
		}
		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		sprintf(name,"%04d/%02d/%02d ",2013,5,i);
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(-1!=flag)
		{
			//有效时间+1
			total++;
			zogdbx_ex_HTMLtablebodyDouble(fp,ConsumeElect);
			zogdbx_ex_HTMLtablebodyDouble(fp,ConsumePrice);
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
		
	}
	zogdbx_ex_HTMLtableend(fp);

	zogdbx_ex_HTMLtableinit(fp,"Date Report 2",1,10);

	zogdbx_ex_HTMLtableRowinit(fp);
	zogdbx_ex_HTMLtableheadStr(fp,"yyyy/mm/dd -> yyyy/mm/dd");
	zogdbx_ex_HTMLtableheadStr(fp,"SumElect");
	zogdbx_ex_HTMLtableheadStr(fp,"SumPrice");
	zogdbx_ex_HTMLtableheadStr(fp,"AverageElect");
	zogdbx_ex_HTMLtableheadStr(fp,"AveragePrice");
	zogdbx_ex_HTMLtableRowend(fp);

	zogdbx_ex_HTMLtableRowinit(fp);
	sprintf(name,"%04d/%02d/%02d -> %04d/%02d/%02d ",2013,5,3,2013,5,19);
	zogdbx_ex_HTMLtablebodyStr(fp,name);
	// 部分 或全部有值
	
	if(1==first)
	{
		flag=zogdbx_ex_GetBetweenHour_YMDH(2013,5,3,0,
										2013,5,19,23,
										&ConsumeElect,
										&ConsumePrice);

		zogdbx_ex_HTMLtablebodyDouble(fp,ConsumeElect);
		zogdbx_ex_HTMLtablebodyDouble(fp,ConsumePrice);
		zogdbx_ex_HTMLtablebodyDouble(fp,ConsumeElect/(double)total);
		zogdbx_ex_HTMLtablebodyDouble(fp,ConsumePrice/(double)total);
	}
	else
	{
		//无数据
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
	}
	zogdbx_ex_HTMLtableRowend(fp);
	
	zogdbx_ex_HTMLtableend(fp);
	
	zogdbx_ex_HTMLend(fp);
	zogdbx_CloseFile(fp);
									
}
#endif

