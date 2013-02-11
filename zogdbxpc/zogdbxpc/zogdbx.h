#ifndef _ZOGDBX_H_INCLUDED
#define _ZOGDBX_H_INCLUDED

//小伍 改 这个为 1=调用  为WINCE版本
//若为 0=PC版本 
#define RELEASE_BULID 0


#include "windows.h"
#include "tchar.h"
#include "shellapi.h"
#include "stdio.h"
#include "string.h"

//外部调用///////////////////////////////////////////////////
void zogdbx_ex_SetDbxDirEverytime(TCHAR *dir);
void zogdbx_ex_SetDbxDirHourtime(TCHAR *dir);

void zogdbx_ex_ClearDbxDir(void);
int zogdbx_ex_DelTree(LPCTSTR lpszPath);
//已废 被	CopyFile(源文件,目标文件,false=覆盖 true=不覆盖); 替换
//int zogdbx_ex_COPY(LPCTSTR fromPath,LPCTSTR toPath);

int zogdbx_ex_GetLastEverytime(unsigned long int *LastUTC,
										double  *LastSumElect,
										double  *LastSumPrice,
										double *LastConsumeElect,
										double *LastConsumePrice);

int zogdbx_ex_GetFirstEverytime(unsigned long int *FirstUTC,
										double  *FirstSumElect,
										double  *FirstSumPrice,
										double  *FirstConsumeElect,
										double  *FirstConsumePrice);

void  zogdbx_ex_CheakAndSave(unsigned long int PrevUTC,
							double  PrevSumElect,
							double  PrevSumPrice,
							unsigned long int CurrentUTC,
							double  CurrentSumElect,
							double  CurrentSumPrice,
							double  CurrentConsumeElect,
							double  CurrentConsumePrice);

int zogdbx_ex_GetPreviousHour(unsigned long int PreviousUTC,
									double  *ConsumeElect,
									double  *ConsumePrice);

int zogdbx_ex_GetCurrentHour(double  *ConsumeElect,
							double  *ConsumePrice);

int  zogdbx_ex_GetEveryHour(unsigned long int UTCtime,
									int UTCyear,
									int UTCmonth,
									int UTCday,
									int UTChour,
									double  *HourSumElect,
									double  *HourSumPrice,
									double  *HourConsumeElect,
									double  *HourConsumePrice);

int zogdbx_ex_GetBetweenHour_UTCT(unsigned long int starttime,
											unsigned long int endtime,
											double  *ConsumeElect,
											double  *ConsumePrice);

int zogdbx_ex_GetBetweenHour_YMDH(int startyear,
											int startmonth,
											int startday,
											int starthour,
											int endyear,
											int endmonth,
											int endday,
											int endhour,
											double  *ConsumeElect,
											double  *ConsumePrice);


#define zogdbx_ex_HTMLinit(fp)	fprintf((fp),"<html>\n<body>\n")
#define zogdbx_ex_HTMLend(fp)	fprintf(fp,"</body>\n</html>\n")
#define zogdbx_ex_HTMLtableend(fp)	fprintf((fp),"</table>\n")
// 输入FILE 指针以及表格标题名
// border=1显示边框cellpadding=10表格间距
#define zogdbx_ex_HTMLtableinit(fp,caption,border,cellpadding)	\
				fprintf((fp),"<table border=\"%d\"\ncellpadding=\"%d\"\ncellspacing=\"0\"\n>\n<caption><b>%s</b></caption>\n\n",	\
																	(border),(cellpadding),(caption))

#define zogdbx_ex_HTMLtableinitCenter(fp,caption,border,cellpadding)	\
				fprintf((fp),"<table border=\"%d\"\ncellpadding=\"%d\"\ncellspacing=\"0\" align=\"center\"\n>\n<caption><b>%s</b></caption>\n\n",	\
																	(border),(cellpadding),(caption))

#define zogdbx_ex_HTMLtableinitLeft(fp,caption,border,cellpadding)	\
	fprintf((fp),"<table border=\"%d\"\ncellpadding=\"%d\"\ncellspacing=\"0\" align=\"left\"\n>\n<caption><b>%s</b></caption>\n\n",	\
																	(border),(cellpadding),(caption))


#define  zogdbx_ex_HTMLtableRowinit(fp)	fprintf((fp),"<tr>\n")
#define  zogdbx_ex_HTMLtableRowend(fp)	fprintf((fp),"</tr>\n")

#define  zogdbx_ex_HTMLtableheadStr(fp,name)	fprintf((fp),"<th>%s</th>\n",(name))
#define  zogdbx_ex_HTMLtableheadNull(fp)		fprintf((fp),"<th>&nbsp;</th>\n")
#define  zogdbx_ex_HTMLtablebodyStr(fp,name)	fprintf((fp),"<td>%s</td>\n",(name))

#define  zogdbx_ex_HTMLtablebody03Double(fp,name)	fprintf((fp),"<td><div align=\"right\">%.3lf</div></td>\n",(name))
#define  zogdbx_ex_HTMLtablebody02Double(fp,name)	fprintf((fp),"<td><div align=\"right\">%.2lf</div></td>\n",(name))
#define  zogdbx_ex_HTMLtablebodyNull(fp)		fprintf((fp),"<td>&nbsp;</td>\n")


//void zogdbx_ex_HTMLinit(FILE*fp);
//void zogdbx_ex_HTMLend(FILE*fp);
//void zogdbx_ex_HTMLtableinitCenter(FILE *fp,char *caption,int border,int cellpadding);
//void zogdbx_ex_HTMLtableend(FILE*fp);

//void zogdbx_ex_HTMLtablebody(FILE *fp,char name[][100],int num);
//void zogdbx_ex_HTMLtablehead(FILE *fp,char name[][100],int num);
#define zogdbx_CloseFile(fp)	fclose((fp))
//#define zogdbx_CloseFile(fp)	fflush((fp));fclose((fp));(fp)=NULL;Sleep(2)

//内部私有//////////////////////////////////////////////////////////

struct EVERYTIME
{
	//UTC+0 秒时间
	unsigned long int CurrentUTC;
	//总用电量
	double  CurrentSumElect;
	//当前总价
	double	CurrentSumPrice;
	//一分钟消耗量
	double  CurrentConsumeElect;
	double  CurrentConsumePrice;
	//价格时段 未来版
	//unsigned char CPriceTier;
};

struct EVERYHOUR
{
	//UTC+0 日日时时
	unsigned int HourTime;
	//小时末读数
	double  HourSumElect;
	double  HourSumPrice;
	//一小时消耗量
	double  HourConsumeElect;
	double  HourConsumePrice;

	//价格时段 未来版
	//unsigned char HPriceTier;
};

#define EVERY_TIME_FILENAME _T("everytime")
#define OLD_EVERY_TIME_FILENAME _T("oldeverytime")

int  zogdbx_in_SaveEveryTime(unsigned long int CurrentUTC,
							double  CurrentSumElect,
							double  CurrentSumPrice,
							double  CurrentConsumeElect,
							double  CurrentConsumePrice);
void  zogdbx_in_SaveOldEveryTime(void);

int  zogdbx_in_SaveEveryHour(unsigned long int filen,
								unsigned int time,
								double  HourSumElect,
								double  HourSumPrice,
								double  HourConsumeElect,
								double  HourConsumePrice);


int  zogdbx_in_GetFirstEveryHour(unsigned long int filen,
									unsigned int dayhour,unsigned int limitday,
									double  *HourSumElect,
									double  *HourSumPrice,
									double  *HourConsumeElect,
									double  *HourConsumePrice,
									unsigned int *outdayhour);

int  zogdbx_in_GetLastEveryHour(unsigned long int filen,
									unsigned int dayhour,unsigned int limitday,
									double  *HourSumElect,
									double  *HourSumPrice,
									double  *HourConsumeElect,
									double  *HourConsumePrice,
									unsigned int *outdayhour);

int  zogdbx_in_RebuildHeadEveryHour(unsigned long int filen);


void zogdbx_in_UTCtoTIME(unsigned long int utcTime,
			int *year,int *month,int *day,int *hour,int *min,int *sec);

unsigned long int zogdbx_in_TIMEtoUTC(int year,int month,int day,int hour,int min,int sec);





/////////////TEST////////////////////////
#if 0
void zogdbx_test_LastHour(void);
void zogdbx_test_Today(void);
void zogdbx_test_Yesterday(void);
void zogdbx_test_LastWeek(void);
void zogdbx_test_LastMonth(void);
void zogdbx_test_Last99Day(void);
void zogdbx_test_DownloadHour(void);
void zogdbx_test_DownloadDay(void);
#endif


//////////////////DST/////////////////////////////////
void zogdst_ex_ReadDSTtime(TCHAR *file);

int zogdst_in_JudgeDSTDay(int year,int month,int day);

void zogdst_ex_UTCzero2UTCzone(unsigned long int UTCzero, int TimeZone,int DSTflag,
							unsigned long int *UTCtime,
							int *year,int *month,int *day,int *hour,int *min,int *sec);

unsigned long int zogdst_ex_UTCzone2UTCzero(int TimeZone,int DSTflag,
										int year,int month,int day,int hour,int min,int sec);

int zogdst_ex_DownloadDSTHour(TCHAR *dir,int year,int month,int day,
									int TimeZone,int DSTflag,int language);
void zogdst_in_DownloadEnd(FILE *fp,
								double TotalElect,double TotalPrice,int TotalTimes,
								int voidflag,int language);

int zogdst_in_DownloadSummerHour(FILE *fp,int year,int month,int day,int tz,int language);
int zogdst_in_DownloadWinterHour(FILE *fp,int year,int month,int day,int tz,int language);
int zogdst_in_DownloadHour(FILE *fp,int year,int month,int day,int tz,int language);

int zogdst_ex_DownloadDSTDay(TCHAR *dir,int syear,int smonth,int sday,
											int eyear,int emonth,int eday,
											int TimeZone,int DSTflag,int language);

int zogdst_in_DownloadDayLite(unsigned long int UTCtime,int tz,int dayflag,
									double *ConsumeElect,
									double *ConsumePrice);

int zogdst_ex_DownloadDSTWeek(TCHAR *dir,int year,int weekday,
											int TimeZone,int DSTflag,int language);

void zogdst_in_MonthName_Day(int y,int m,int d,char *name);

char * zogdst_in_MonthNameC(int m);
TCHAR * zogdst_in_MonthNameT(int m);

int zogdst_ex_DownloadDSTMonth(TCHAR *dir,int year,
											int TimeZone,int DSTflag,int language);

int zogdst_ex_DownloadDSTMonthDay(TCHAR *dir,int year,int month,
											int TimeZone,int DSTflag,int language);

/////////////////////////////////////////////////////////////////////////////////////

int zogdst_ex_DSTToday(int year,int month,int day,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice);
int zogdst_ex_DSTYesterday(int year,int month,int day,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice);

int zogdst_ex_DSTLastMonth(int year,int month,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice);

int zogdst_ex_DSTLastWeek(int year,int month,int day,int weekday,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice);

int zogdst_ex_DSTLastNday(int year,int month,int day,int num,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice);

void zogdst_ex_DSTHourReport(int year,int month,int day,
									int TimeZone,int DSTflag,
									int *num,double *Elect,double *Price);
void zogdst_in_DSTHourReportLite(int year,int month,int day,int timezone,
										double *Elect,double *Price);
void zogdst_in_DSTHourReportWinter(int year,int month,int day,int timezone,
										double *Elect,double *Price);
void zogdst_in_DSTHourReportSummer(int year,int month,int day,int timezone,
										double *Elect,double *Price);

void zogdst_ex_DSTWeekDayReport(int year,int weekday,int weekN,
										int TimeZone,int DSTflag,
										int *num,double *Elect,double *Price);

void zogdst_ex_DSTMonthDayReport(int year,int month,
											int TimeZone,int DSTflag,
											int *num,double *Elect,double *Price);

void zogdst_ex_DSTWeekReport(int year,int season,int weekday,
										int TimeZone,int DSTflag,
										int *num,double *Elect,double *Price);
void zogdst_ex_DSTMonthReport(int year,int TimeZone,int DSTflag,
										int *num,double *Elect,double *Price);

int zogdst_in_DSTBetweenAny(int syear,int smonth,int sday,
								int eyear,int emonth,int eday,
								int endUTC,
								int TimeZone,int DSTflag,
								double  *TotalSumElect,
								double  *TotalSumPrice);

int zogdst_in_DSTBetweenHour(int year,int month,int day,
									unsigned long int UTC,
								int TimeZone,int DSTflag,
								double  *HourConsumeElect,
								double  *HourConsumePrice);

int zogdst_in_DSTBetweenDay(int syear,int smonth,int sday,
								int eyear,int emonth,int eday,
								int TimeZone,int DSTflag,
								double  *TotalSumElect,
								double  *TotalSumPrice);
#if RELEASE_BULID
//伍 增加
int  zip2file(char *infile,CString signmark,unsigned long int *curtime,char *outDir);
#endif

#endif 