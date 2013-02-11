//#include "stdafx.h"
#include "zogdbx.h"

//非夏天的时区未+-过的
//int TimeZone=2;
//是否开启夏令时功能 1是0否
//int DSTflag=0;

#define STRING_NUMBER 250

//预存100年
int zogdst_DSTtime[100][2][3]={0};//在此区间内为夏 时区-1
int zogdst_DSTtimenum=0;
//跳转的那小时
int zogdst_jumptime=0;

#define ZOGDST_DEBUG 0
 
//初始化读取 DST时间表
void zogdst_ex_ReadDSTtime(TCHAR *file)
{
	FILE *fp=NULL;
	//成功返回0
	int err=_tfopen_s(&fp,file,_T("r"));
	if( 0==err )
	{
		fscanf_s(fp,"%d\n",&zogdst_jumptime);
						
		while(!feof(fp))
		{
			fscanf_s(fp,"%d %d %d\n",
						&zogdst_DSTtime[zogdst_DSTtimenum][0][0],
						&zogdst_DSTtime[zogdst_DSTtimenum][0][1],
						&zogdst_DSTtime[zogdst_DSTtimenum][0][2]);
			fscanf_s(fp,"%d %d %d\n",
						&zogdst_DSTtime[zogdst_DSTtimenum][1][0],
						&zogdst_DSTtime[zogdst_DSTtimenum][1][1],
						&zogdst_DSTtime[zogdst_DSTtimenum][1][2]);
		
			zogdst_DSTtimenum++;
		}
		zogdbx_CloseFile(fp);
	}

#if ZOGDST_DEBUG
	int i=0;
	printf("%d,%d\n",zogdst_DSTtimenum,zogdst_jumptime);
	for(i=0;i<zogdst_DSTtimenum;i++)
		printf("i=%d,%d-%d-%d,%d-%d-%d\n",i,
						zogdst_DSTtime[i][0][0],
						zogdst_DSTtime[i][0][1],
						zogdst_DSTtime[i][0][2],
						zogdst_DSTtime[i][1][0],
						zogdst_DSTtime[i][1][1],
						zogdst_DSTtime[i][1][2]);
#endif

}

//int dayflag =1 夏日
//int dayflag =2 冬日
//int dayflag =3 夏时当日
//int dayflag =4 冬时当日
int zogdst_in_JudgeDSTDay(int year,int month,int day)
{
	int i;
	for(i=0;i<zogdst_DSTtimenum;i++)
	{
		//夏天起始当日25小时
		if( (year == zogdst_DSTtime[i][0][0])  && 
			(month ==zogdst_DSTtime[i][0][1]) && 
			(day ==zogdst_DSTtime[i][0][2]) )
		{
			return 3;
		}
		//冬天开始当日23小时
		else		if( (year == zogdst_DSTtime[i][1][0])  && 
					(month ==zogdst_DSTtime[i][1][1]) && 
					(day ==zogdst_DSTtime[i][1][2]) )
		{
			return 4;
		}
		else if((year == zogdst_DSTtime[i][0][0]) && (year == zogdst_DSTtime[i][1][0]) )
		{
			if((month >zogdst_DSTtime[i][0][1]) && (month <zogdst_DSTtime[i][1][1] ) )
				return 1;
			else if((month ==zogdst_DSTtime[i][0][1]) && (day >zogdst_DSTtime[i][0][2]) )
				return 1;
			else if((month ==zogdst_DSTtime[i][1][1]) && (day <zogdst_DSTtime[i][1][2]) )
				return 1;
		}
	}
	
	return 2;
}

// 传入0时区 的时间 
// 时区
//是否开启时令 1=开 0不开
//输出经过时区加减的UTCTIME 和年月日时分秒
void zogdst_ex_UTCzero2UTCzone(unsigned long int UTCzero, int TimeZone,int DSTflag,
							unsigned long int *UTCtime,
							int *year,int *month,int *day,int *hour,int *min,int *sec)
{
	int dayflag;

	*UTCtime=UTCzero+TimeZone*3600;
	zogdbx_in_UTCtoTIME(*UTCtime,year,month,day,hour,min,sec);
		
	//开启DST
	if(DSTflag)
	{
		dayflag=zogdst_in_JudgeDSTDay(*year,*month,*day);
		
		if(1==dayflag)
		{
			*UTCtime=UTCzero+(TimeZone-1)*3600;
			zogdbx_in_UTCtoTIME(*UTCtime,year,month,day,hour,min,sec);
		}
		else if(2==dayflag)
		{
			//冬天不做任何事
			NULL;
		}
		else  if(3==dayflag)
		{

			if(*hour <=zogdst_jumptime)	
				//冬天不做任何事
				NULL;
			else
			{
				*UTCtime=UTCzero+(TimeZone-1)*3600;
				zogdbx_in_UTCtoTIME(*UTCtime,year,month,day,hour,min,sec);
			}	
		}
		else if(4==dayflag)
		{
			if(*hour <=zogdst_jumptime)	
			{
				*UTCtime=UTCzero+(TimeZone-1)*3600;
				zogdbx_in_UTCtoTIME(*UTCtime,year,month,day,hour,min,sec);
			}
			else
				//冬天不做任何事
				NULL;
		}
	}

}
// 传入当前系统时令时区 的时间 
// 时区
//是否开启时令 1=开 0不开
//输入设置前的时区和设置前的时令
//返回 时区
unsigned long int  zogdst_ex_UTCzone2UTCzero(int TimeZone,int DSTflag,
											int year,int month,int day,int hour,int min,int sec)
{
	int dayflag;

	if(DSTflag)
		dayflag=zogdst_in_JudgeDSTDay(year,month,day);
	else
		dayflag=2;
	
	unsigned long int iUTC=zogdbx_in_TIMEtoUTC(year,month,day,hour,min, sec);

	if(1==dayflag)
		return iUTC-(TimeZone-1)*3600;
	else if(2==dayflag)
		return iUTC-TimeZone*3600;
	else  if(3==dayflag)
	{
		if(hour <=zogdst_jumptime)	
			return iUTC-TimeZone*3600;
		else
			return iUTC-(TimeZone-1)*3600;
	}
	else if(4==dayflag)
	{
		if(hour <=zogdst_jumptime)	
			return iUTC-(TimeZone-1)*3600;
		else
			return iUTC-TimeZone*3600;
	}
	else
		return 0;
}
#if 0
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif

//下载DST一天一条数据的报告
//输入报告目录路径
//输入起始年月日 结束年月日 时区 以及是否使用夏时令 是=1 否=0
//输入 语言1=台湾0=加拿大(默认) 
//失败-1
//成功1
int zogdst_ex_DownloadDSTDay(TCHAR *dir,int syear,int smonth,int sday,
											int eyear,int emonth,int eday,
											int TimeZone,int DSTflag,int language)
{
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0,first=0;
	int total=0;
	int dayflag;
	double totalConsumeElect=0.0;
	double totalConsumePrice=0.0;
	
	unsigned long int startUTC,endUTC,iUTC;
	int year,month,day,hour,min,sec;
	//年月日转秒级
	startUTC=zogdbx_in_TIMEtoUTC(syear,smonth,sday,0,0,1);
	endUTC=zogdbx_in_TIMEtoUTC(eyear,emonth,eday,0,0,1);
	
	FILE *fp=NULL;
	char name[STRING_NUMBER]={0};
	
	TCHAR zogdbxFILE[MAX_PATH]={0};

	if(1==language)
		//创建路径
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\時段報表 %04d-%02d-%02d 至 %04d-%02d-%02d.html"),dir,syear,smonth,sday,eyear,emonth,eday);
	else
		//创建路径
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\Period Report %02d-%02d-%04d to %02d-%02d-%04d.html"),dir,smonth,sday,syear,emonth,eday,eyear);

	//不可以有B
	//成功返回0
	int err=_tfopen_s(&fp,zogdbxFILE,_T("w+"));
	if( 0!=err )
		return -1;
	//if( !(fp=_tfopen(zogdbxFILE,_T("w+"))) )
	//	return -1;

	zogdbx_ex_HTMLinit(fp);

	if(1==language)
		_snprintf(name,STRING_NUMBER,"用電量及電費時段報表<br>%04d年%02d月%02d日 至 %04d年%02d月%02d日<p></p>",syear,smonth,sday,eyear,emonth,eday);
	else
		_snprintf(name,STRING_NUMBER,"Electricity Consumption Period Report<br>%02d-%02d-%04d to %02d-%02d-%04d<p></p>",smonth,sday,syear,emonth,eday,eyear);
	
	zogdbx_ex_HTMLtableinitCenter(fp,name,1,2);

	zogdbx_ex_HTMLtableRowinit(fp);
	
	if(1==language)
	{
		zogdbx_ex_HTMLtableheadStr(fp,"日期");
		zogdbx_ex_HTMLtableheadStr(fp,"用電量 (度)");
		zogdbx_ex_HTMLtableheadStr(fp,"電費 (元)");
	}
	else
	{
		zogdbx_ex_HTMLtableheadStr(fp,"mm/dd/yyyy");
		zogdbx_ex_HTMLtableheadStr(fp,"Electricity Consumption (KWh)");
		zogdbx_ex_HTMLtableheadStr(fp,"Estimated Cost ($)");
	}
	
	zogdbx_ex_HTMLtableRowend(fp);
	
	total=0;
	//每次+一天的秒
	for(iUTC=startUTC;iUTC<=endUTC;iUTC+=86400)
	{
		zogdbx_in_UTCtoTIME(iUTC,&year,&month,&day,&hour,&min,&sec);
		//判断这天是什么时候	
		if(DSTflag)
			dayflag=zogdst_in_JudgeDSTDay(year,month,day);
		else
			dayflag=2;
		//获取每个天的值
		flag=zogdst_in_DownloadDayLite(iUTC,TimeZone,dayflag,
										&ConsumeElect,
										&ConsumePrice);
		
		//获取初始值用于以后统计
		if((-1!=flag) && (0==first))
		{
			first=1;
		}
		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		if(1==language)
			_snprintf(name,STRING_NUMBER,"<b>%04d年%02d月%02d日</b>",year,month,day);
		else
			_snprintf(name,STRING_NUMBER,"<b>%02d/%02d/%04d</b>",month,day,year);	
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(-1!=flag)
		{
			//有效时间+1
			total++;
			totalConsumeElect +=ConsumeElect;
			totalConsumePrice +=ConsumePrice;
			zogdbx_ex_HTMLtablebody03Double(fp,ConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,ConsumePrice);
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
		
	}
	// 部分 或全部有值
	zogdst_in_DownloadEnd(fp,
						totalConsumeElect,totalConsumePrice,total,
						first,language);
	return 1;
}

/*
//int dayflag =1 夏日
//int dayflag =2 冬日
//int dayflag =3 夏时当日
//int dayflag =4 冬时当日
int zogdst_in_DownloadDayLite(int year,int month,int day,int tz,int dayflag,
									double *ConsumeElect,
									double *ConsumePrice)
{
	int syear,smonth,sday,shour,smin,ssec;
	int eyear,emonth,eday,ehour,emin,esec;

	if(1==dayflag)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,0,0,1)-(tz-1)*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,23,0,1)-(tz-1)*3600,
							&eyear,&emonth,&eday,&ehour,&emin,&esec);
	}
	else if(2==dayflag)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,0,0,1)-tz*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,23,0,1)-tz*3600,
							&eyear,&emonth,&eday,&ehour,&emin,&esec);
	}
	else  if(3==dayflag)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,0,0,1)-tz*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,23,0,1)-(tz-1)*3600,
							&eyear,&emonth,&eday,&ehour,&emin,&esec);
	}
	else if(4==dayflag)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,0,0,1)-(tz-1)*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,23,0,1)-tz*3600,
							&eyear,&emonth,&eday,&ehour,&emin,&esec);
	}
	else 
		return -1;
		
	return zogdbx_ex_GetBetweenHour_YMDH(syear,smonth,sday,shour,
										eyear,emonth,eday,ehour,
										ConsumeElect,
										ConsumePrice);	
}
*/
//返回 1成功
// 返回0成功但不完整 可能只有部分数据
//返回-1 信息未知文件出错 致命错误完全无数据
//int dayflag =1 夏日
//int dayflag =2 冬日
//int dayflag =3 夏时当日
//int dayflag =4 冬时当日
int zogdst_in_DownloadDayLite(unsigned long int UTCtime,int tz,int dayflag,
									double *ConsumeElect,
									double *ConsumePrice)
{
	int syear,smonth,sday,shour,smin,ssec;
	int eyear,emonth,eday,ehour,emin,esec;

	//一天的起始 输入00 结束输入23
	//以下将目前时区转为0时区 并且夏令时也经过转换
	if(1==dayflag)
	{
		zogdbx_in_UTCtoTIME(UTCtime-(tz-1)*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME((UTCtime+23*3600)-(tz-1)*3600,
							&eyear,&emonth,&eday,&ehour,&emin,&esec);
	}
	else if(2==dayflag)
	{
		zogdbx_in_UTCtoTIME(UTCtime-tz*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME((UTCtime+23*3600)-tz*3600,
							&eyear,&emonth,&eday,&ehour,&emin,&esec);
	}
	else  if(3==dayflag)
	{
		zogdbx_in_UTCtoTIME(UTCtime-tz*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME((UTCtime+23*3600)-(tz-1)*3600,
							&eyear,&emonth,&eday,&ehour,&emin,&esec);
	}
	else if(4==dayflag)
	{
		zogdbx_in_UTCtoTIME(UTCtime-(tz-1)*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME((UTCtime+23*3600)-tz*3600,
							&eyear,&emonth,&eday,&ehour,&emin,&esec);
	}
	else 
		return -1;
	
	#if ZOGDST_DEBUG 
	printf("dayflag=%d,%d,%d,%d,%d,-%d,%d,%d,%d\n",dayflag,syear,smonth,sday,shour,
										eyear,emonth,eday,ehour);
	#endif
	//查询
	return zogdbx_ex_GetBetweenHour_YMDH(syear,smonth,sday,shour,
										eyear,emonth,eday,ehour,
										ConsumeElect,
										ConsumePrice);	
}

void zogdst_in_MonthName_Day(int y,int m,int d,char *name)
{
	char dhz[3]={0};
	
	switch(d)
	{
		case 1:
		case 21:
		case 31:
			dhz[0]='s';
			dhz[1]='t';
			break;
		case 2:
		case 22:
			dhz[0]='n';
			dhz[1]='d';
			break;
		case 3:
		case 23:	
			dhz[0]='r';
			dhz[1]='d';
			break;
		default:
			dhz[0]='t';
			dhz[1]='h';
			break;
	}
	_snprintf(name,STRING_NUMBER,"Electricity Consumption Daily Report<br>%s %02d%s, %04d<p></p>",
					zogdst_in_MonthNameC(m),d,dhz,y);
}

//下载DST一小时一条数据的报告
//输入报告目录路径
//输入年月日  时区 以及是否使用夏时令 是=1 否=0
//输入 语言1=台湾0=加拿大(默认) 
//失败-1
//成功1
int zogdst_ex_DownloadDSTHour(TCHAR *dir,int year,int month,int day,
									int TimeZone,int DSTflag,int language)
{
	int dayflag;

	FILE *fp=NULL;
	char name[STRING_NUMBER]={0};

	TCHAR zogdbxFILE[MAX_PATH]={0};
	
	if(1==language)
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\日報表 %04d-%02d-%02d.html"),dir,year,month,day);
	else
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\Hourly Report %02d-%02d-%04d.html"),dir,month,day,year);
	
	//不可以有B
	int err=_tfopen_s(&fp,zogdbxFILE,_T("w+"));
	if( 0!=err )
		return -1;
	//if( !(fp=_tfopen(zogdbxFILE,_T("w+"))) )
	//	return -1;
	
	zogdbx_ex_HTMLinit(fp);
	
	if(1==language)
		_snprintf(name,STRING_NUMBER,"用電量及電費日報表<br>%04d年%02d月%02d日<p></p>",year,month,day);	
	else
		//_snprintf(name,"%02d/%02d/%04d Electricity Consumption Report",month,day,year);
		zogdst_in_MonthName_Day(year,month,day,name);
	
	zogdbx_ex_HTMLtableinitCenter(fp,name,1,2);

	zogdbx_ex_HTMLtableRowinit(fp);
	
	if(1==language)
	{
		zogdbx_ex_HTMLtableheadStr(fp,"时:分 -> 时:分");
		zogdbx_ex_HTMLtableheadStr(fp,"用電量 (度)");
		zogdbx_ex_HTMLtableheadStr(fp,"電費 (元)");
	}
	else
	{
		zogdbx_ex_HTMLtableheadStr(fp,"hh:mm->hh:mm");
		zogdbx_ex_HTMLtableheadStr(fp,"Electricity Consumption  (KWh)");
		zogdbx_ex_HTMLtableheadStr(fp,"Estimated Cost ($)");
	}
	zogdbx_ex_HTMLtableRowend(fp);
	
	//开启DST
	if(DSTflag)
	{
		//int dayflag =1 夏日
		//int dayflag =2 冬日
		//int dayflag =3 夏时当日
		//int dayflag =4 冬时当日
		dayflag=zogdst_in_JudgeDSTDay(year,month,day);

		if(1==dayflag)
			return zogdst_in_DownloadHour(fp,year,month,day,TimeZone-1,language);
		else 	if(2==dayflag)
			return zogdst_in_DownloadHour(fp,year,month,day,TimeZone,language);
		else 	if(3==dayflag)
			return zogdst_in_DownloadSummerHour(fp,year,month,day,TimeZone,language);
		else 	if(4==dayflag)
			return zogdst_in_DownloadWinterHour(fp,year,month,day,TimeZone,language);
		else
			return -1;
	}
	else
	{
		//无DST
		return zogdst_in_DownloadHour(fp,year,month,day,TimeZone,language);
	}
	
}

//输入 语言1=台湾0=加拿大(默认) 
//下载统计的打印
void zogdst_in_DownloadEnd(FILE *fp,
								double TotalElect,double TotalPrice,int TotalTimes,
								int voidflag,int language)
{
	if(1==voidflag)
	{
		zogdbx_ex_HTMLtableRowinit(fp);
		
		if(1==language)
			zogdbx_ex_HTMLtableheadStr(fp,"總共");
		else
			zogdbx_ex_HTMLtableheadStr(fp,"Total");
		
		zogdbx_ex_HTMLtablebody03Double(fp,TotalElect);
		zogdbx_ex_HTMLtablebody02Double(fp,TotalPrice);
		zogdbx_ex_HTMLtableRowend(fp);
		
		zogdbx_ex_HTMLtableRowinit(fp);

		if(1==language)
			zogdbx_ex_HTMLtableheadStr(fp,"平均");
		else
			zogdbx_ex_HTMLtableheadStr(fp,"Average");
				
		zogdbx_ex_HTMLtablebody03Double(fp,TotalElect/(double)TotalTimes);
		zogdbx_ex_HTMLtablebody02Double(fp,TotalPrice/(double)TotalTimes);
		zogdbx_ex_HTMLtableRowend(fp);
	}
	else
	{
		//无数据
		zogdbx_ex_HTMLtableRowinit(fp);
		
		if(1==language)
			zogdbx_ex_HTMLtableheadStr(fp,"總共");
		else
			zogdbx_ex_HTMLtableheadStr(fp,"Total");
		
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtableRowend(fp);
		
		zogdbx_ex_HTMLtableRowinit(fp);

		if(1==language)
			zogdbx_ex_HTMLtableheadStr(fp,"平均");
		else
			zogdbx_ex_HTMLtableheadStr(fp,"Average");
		
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtableRowend(fp);
	}
	
	//关闭文件
	zogdbx_ex_HTMLtableend(fp);
	zogdbx_ex_HTMLend(fp);
	zogdbx_CloseFile(fp);	
	
}

//当正好是夏令时当日 要打印25条数据 
//输入的时区为冬天 设置的正常时区
//失败-1
//成功1
int zogdst_in_DownloadSummerHour(FILE *fp,int year,int month,int day,int tz,int language)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	double startElect=0,startPrice=0;
	double endElect=0,endPrice=0;
	
	int flag=0,first=0;
	int i;
	int total=0;
	
	char name[STRING_NUMBER]={0};
	
	int iyear,imonth,iday,ihour,imin,isec;
	
	total=0;
	//在跳转时刻zogdst_jumptime 进行跳转
	for(i=0;i<zogdst_jumptime;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-tz*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if((1==flag) && (0==first))
		{
			startElect=HourSumElect-HourConsumeElect;
			startPrice=HourSumPrice-HourConsumePrice;
			first=1;
		}

		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		//_snprintf(name,"%02d/%02d/%04d %02d:00->%02d:00",month,day,year,i,i+1);
		_snprintf(name,STRING_NUMBER,"<b>%02d:00 -> %02d:00</b>",i,i+1);
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(1==flag)
		{
			//有效时间+1
			total++;
			//结束
			endElect=HourSumElect;
			endPrice=HourSumPrice;
			zogdbx_ex_HTMLtablebody03Double(fp,HourConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,HourConsumePrice);
		
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
	}
	//在跳转时刻zogdst_jumptime 进行跳转
	for(i=zogdst_jumptime-1;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-(tz-1)*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if((1==flag) && (0==first))
		{
			startElect=HourSumElect-HourConsumeElect;
			startPrice=HourSumPrice-HourConsumePrice;
			first=1;
		}

		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		//_snprintf(name,STRING_NUMBER,"%02d/%02d/%04d %02d:00->%02d:00",month,day,year,i,i+1);
		_snprintf(name,STRING_NUMBER,"<b>%02d:00 -> %02d:00</b>",i,i+1);
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(1==flag)
		{
			//有效时间+1
			total++;
			//结束
			endElect=HourSumElect;
			endPrice=HourSumPrice;
			zogdbx_ex_HTMLtablebody03Double(fp,HourConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,HourConsumePrice);
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
	}
	
	// 部分 或全部有值
	zogdst_in_DownloadEnd(fp,
						endElect-startElect,endPrice-startPrice,total,
						first,language);
	return 1;
}
//当正好是冬令时当日 要打印23条数据 
//输入的时区为冬天 设置的正常时区
//失败-1
//成功1
int  zogdst_in_DownloadWinterHour(FILE *fp,int year,int month,int day,int tz,int language)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	double startElect=0,startPrice=0;
	double endElect=0,endPrice=0;
	
	int flag=0,first=0;
	int i;
	int total=0;
	
	char name[STRING_NUMBER]={0};
	
	int iyear,imonth,iday,ihour,imin,isec;
	
	total=0;
	for(i=0;i<zogdst_jumptime;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-(tz-1)*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if((1==flag) && (0==first))
		{
			startElect=HourSumElect-HourConsumeElect;
			startPrice=HourSumPrice-HourConsumePrice;
			first=1;
		}

		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		//_snprintf(name,STRING_NUMBER,"%02d/%02d/%04d %02d:00->%02d:00",month,day,year,i,i+1);
		_snprintf(name,STRING_NUMBER,"<b>%02d:00 -> %02d:00</b>",i,i+1);
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(1==flag)
		{
			//有效时间+1
			total++;
			//结束
			endElect=HourSumElect;
			endPrice=HourSumPrice;
			zogdbx_ex_HTMLtablebody03Double(fp,HourConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,HourConsumePrice);
		
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
	}
	for(i=zogdst_jumptime+1;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-tz*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if((1==flag) && (0==first))
		{
			startElect=HourSumElect-HourConsumeElect;
			startPrice=HourSumPrice-HourConsumePrice;
			first=1;
		}

		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		//_snprintf(name,STRING_NUMBER,"%02d/%02d/%04d %02d:00->%02d:00",month,day,year,i,i+1);
		_snprintf(name,STRING_NUMBER,"<b>%02d:00 -> %02d:00</b>",i,i+1);
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(1==flag)
		{
			//有效时间+1
			total++;
			//结束
			endElect=HourSumElect;
			endPrice=HourSumPrice;
			zogdbx_ex_HTMLtablebody03Double(fp,HourConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,HourConsumePrice);
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
	}
	// 部分 或全部有值
	zogdst_in_DownloadEnd(fp,
						endElect-startElect,endPrice-startPrice,total,
						first,language);
	return 1;
}


//夏天或者冬天 要打印24条数据 
// 返回-1 路径出错
//失败-1
//成功1
int  zogdst_in_DownloadHour(FILE *fp,int year,int month,int day,int tz,int language)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	double startElect=0,startPrice=0;
	double endElect=0,endPrice=0;
	
	int flag=0,first=0;
	int i;
	int total=0;

	char name[STRING_NUMBER]={0};
	
	int iyear,imonth,iday,ihour,imin,isec;
	
	total=0;
	for(i=0;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-tz*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if((1==flag) && (0==first))
		{
			startElect=HourSumElect-HourConsumeElect;
			startPrice=HourSumPrice-HourConsumePrice;
			first=1;
		}

		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		//_snprintf(name,STRING_NUMBER,"%02d/%02d/%04d %02d:00->%02d:00",month,day,year,i,i+1);
		_snprintf(name,STRING_NUMBER,"<b>%02d:00 -> %02d:00</b>",i,i+1);
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(1==flag)
		{
			//有效时间+1
			total++;
			//结束
			endElect=HourSumElect;
			endPrice=HourSumPrice;
			zogdbx_ex_HTMLtablebody03Double(fp,HourConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,HourConsumePrice);
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
	}
	
	// 部分 或全部有值
	zogdst_in_DownloadEnd(fp,
						endElect-startElect,endPrice-startPrice,total,
						first,language);
	return 1;
}



//下载DST一周一条的报告
//输入报告目录路径
//输入年 
//输入该年1月1日是星期几
//时区 以及是否使用夏时令 是=1 否=0
//失败-1
//成功1
//周日0 周一1.周六6
//输入 语言1=台湾0=加拿大(默认) 
int zogdst_ex_DownloadDSTWeek(TCHAR *dir,int year,int weekday,
											int TimeZone,int DSTflag,int language)
{
	int i;
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0,first=0;
	int total=0;
	double totalConsumeElect=0.0;
	double totalConsumePrice=0.0;
	
	unsigned long int startUTC,endUTC,iUTC;
	int syear,smonth,sday,shour,smin,ssec;
	int eyear,emonth,eday,ehour,emin,esec;
		
	FILE *fp=NULL;
	char name[STRING_NUMBER]={0};
	
	TCHAR zogdbxFILE[MAX_PATH]={0};
	
	//创建路径
	if(language)
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\周報表 %04d年.html"),dir,year);
	else
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\Weekly Report %04d.html"),dir,year);

	//不可以有B
	int err=_tfopen_s(&fp,zogdbxFILE,_T("w+"));
	if( 0!=err )
		return -1;
	//if( !(fp=_tfopen(zogdbxFILE,_T("w+"))) )
	//	return -1;

	zogdbx_ex_HTMLinit(fp);
	if(language)
		_snprintf(name,STRING_NUMBER,"%04d年 用電量及電費周報表<p></p>",year);
	else
		_snprintf(name,STRING_NUMBER,"Electricity Consumption Weekly Report<br>Year %04d<p></p>",year);
	
	if(language)
		zogdbx_ex_HTMLtableinitLeft(fp,name,1,2);
	else
		zogdbx_ex_HTMLtableinitCenter(fp,name,1,2);

	zogdbx_ex_HTMLtableRowinit(fp);
	if(language)
	{
		zogdbx_ex_HTMLtableheadStr(fp,"第N周:月/日->月/日");
		zogdbx_ex_HTMLtableheadStr(fp,"用電量 (度)");
		zogdbx_ex_HTMLtableheadStr(fp,"電費 (元)");
	}
	else
	{
		zogdbx_ex_HTMLtableheadStr(fp,"Week No: mm/dd -> mm/dd");
		zogdbx_ex_HTMLtableheadStr(fp,"Electricity Consumption (KWh)");
		zogdbx_ex_HTMLtableheadStr(fp,"Estimated Cost ($)");
	}
	
	zogdbx_ex_HTMLtableRowend(fp);

	total=0;

	//显示第一周
	startUTC=zogdbx_in_TIMEtoUTC(year,1,1+(7-weekday),0,0,1);

	flag= zogdst_in_DSTBetweenAny(year,1,1,
							year,1,1+(7-weekday),
							startUTC,
							TimeZone,DSTflag,
							&ConsumeElect,
							&ConsumePrice);
	if((-1!=flag) && (0==first))
	{
		first=1;
	}

	zogdbx_ex_HTMLtableRowinit(fp);
	//时间
	zogdbx_in_UTCtoTIME(startUTC-60,&syear,&smonth,&sday,&shour,&smin,&ssec);
	if(language)
		_snprintf(name,STRING_NUMBER,"<b>第01周:01/01->01/%02d</b>",sday);
	else
		_snprintf(name,STRING_NUMBER,"<b>Week 01: 01/01 -> 01/%02d</b>",sday);
	
	zogdbx_ex_HTMLtablebodyStr(fp,name);
	if(-1!=flag)
	{
		//有效时间+1
		total++;
		totalConsumeElect +=ConsumeElect;
		totalConsumePrice +=ConsumePrice;
		zogdbx_ex_HTMLtablebody03Double(fp,ConsumeElect);
		zogdbx_ex_HTMLtablebody02Double(fp,ConsumePrice);
	}
	else
	{	
		//无数据
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
	}

	zogdbx_ex_HTMLtableRowend(fp);
	
	////////////////////////////显示中间周
	endUTC=zogdbx_in_TIMEtoUTC(year+1,1,1,0,0,1);
		
	//每次+一周的秒
	for(i=2,iUTC=startUTC;(iUTC+604800)<endUTC;iUTC+=604800,i++)
	{
		//另外一个表格
		if((language) && (28==i))
		{
			zogdbx_ex_HTMLtableend(fp);
			
			zogdbx_ex_HTMLtableinit(fp,".<p></p>",1,2);

			zogdbx_ex_HTMLtableRowinit(fp);
			zogdbx_ex_HTMLtableheadStr(fp,"第N周:月/日->月/日");
			zogdbx_ex_HTMLtableheadStr(fp,"用電量 (度)");
			zogdbx_ex_HTMLtableheadStr(fp,"電費 (元)");		
			zogdbx_ex_HTMLtableRowend(fp);
		}

		
		zogdbx_in_UTCtoTIME(iUTC,&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME(iUTC+604800,&eyear,&emonth,&eday,&ehour,&emin,&esec);
		
			
		flag= zogdst_in_DSTBetweenAny(syear,smonth,sday,
							eyear,emonth,eday,
							iUTC+604800,
							TimeZone,DSTflag,
							&ConsumeElect,
							&ConsumePrice);
		if((-1!=flag) && (0==first))
		{
			first=1;
		}

		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		zogdbx_in_UTCtoTIME(iUTC+604800-60,&eyear,&emonth,&eday,&ehour,&emin,&esec);
		
		if(language)
			_snprintf(name,STRING_NUMBER,"<b>第%02d周:%02d/%02d->%02d/%02d</b>",i,smonth,sday,emonth,eday);
		else
			_snprintf(name,STRING_NUMBER,"<b>Week %02d: %02d/%02d -> %02d/%02d</b>",i,smonth,sday,emonth,eday);
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(-1!=flag)
		{
			//有效时间+1
			total++;
			totalConsumeElect +=ConsumeElect;
			totalConsumePrice +=ConsumePrice;
			zogdbx_ex_HTMLtablebody03Double(fp,ConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,ConsumePrice);
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}
	}

	//显示最后一周
	zogdbx_in_UTCtoTIME(iUTC,&syear,&smonth,&sday,&shour,&smin,&ssec);
	
	//endUTC=zogdbx_in_TIMEtoUTC(year+1,1,1,0,0,1);

	flag= zogdst_in_DSTBetweenAny(syear,smonth,sday,
							year+1,1,1,
							endUTC,
							TimeZone,DSTflag,
							&ConsumeElect,
							&ConsumePrice);
	if((-1!=flag) && (0==first))
	{
		first=1;
	}

	zogdbx_ex_HTMLtableRowinit(fp);
	//时间
	if(language)
		_snprintf(name,STRING_NUMBER,"<b>第%02d周:%02d/%02d->12/31</b>",i,smonth,sday);
	else
		_snprintf(name,STRING_NUMBER,"<b>Week %02d: %02d/%02d -> 12/31</b>",i,smonth,sday);

	zogdbx_ex_HTMLtablebodyStr(fp,name);
	if(-1!=flag)
	{
		//有效时间+1
		total++;
		totalConsumeElect +=ConsumeElect;
		totalConsumePrice +=ConsumePrice;
		zogdbx_ex_HTMLtablebody03Double(fp,ConsumeElect);
		zogdbx_ex_HTMLtablebody02Double(fp,ConsumePrice);
	}
	else
	{	
		//无数据
		zogdbx_ex_HTMLtablebodyNull(fp);
		zogdbx_ex_HTMLtablebodyNull(fp);
	}
	
	// 部分 或全部有值
	zogdst_in_DownloadEnd(fp,
						totalConsumeElect,totalConsumePrice,total,
						first,language);
	
	return 1;
}

char * zogdst_in_MonthNameC(int m)
{
	switch(m)
	{
		case 1:	return "January";
		case 2:	return "February";
		case 3:	return "March";
		case 4:	return "April";
		case 5:	return "May";
		case 6:	return "June";
		case 7:	return "July";
		case 8:	return "August";
		case 9:	return "September";
		case 10: return "October";
		case 11: return "November";
		case 12: return "December";
		default: return "";
	}
}

TCHAR * zogdst_in_MonthNameT(int m)
{
	switch(m)
	{
		case 1:	return _T("January");
		case 2:	return _T("February");
		case 3:	return _T("March");
		case 4:	return _T("April");
		case 5:	return _T("May");
		case 6:	return _T("June");
		case 7:	return _T("July");
		case 8:	return _T("August");
		case 9:	return _T("September");
		case 10: return _T("October");
		case 11: return _T("November");
		case 12: return _T("December");
		default: return _T("");
	}
}

//下载DST一月一条的报告
//输入报告目录路径
//输入年 
//时区 以及是否使用夏时令 是=1 否=0
//失败-1
//成功1
//输入 语言1=台湾0=加拿大(默认) 
int zogdst_ex_DownloadDSTMonth(TCHAR *dir,int year,
											int TimeZone,int DSTflag,int language)
{
	int i;
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0,first=0;
	int total=0;
	double totalConsumeElect=0.0;
	double totalConsumePrice=0.0;
	
	unsigned long int iUTC;
		
	FILE *fp=NULL;
	char name[STRING_NUMBER]={0};
	
	TCHAR zogdbxFILE[MAX_PATH]={0};
	
	//创建路径
	if(language)
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\月報表 %04d年.html"),dir,year);
	else
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\Monthly Report %04d.html"),dir,year);

	//不可以有B
	int err=_tfopen_s(&fp,zogdbxFILE,_T("w+"));
	if( 0!=err )
		return -1;
	//if( !(fp=_tfopen(zogdbxFILE,_T("w+"))) )
	//	return -1;

	zogdbx_ex_HTMLinit(fp);
	if(language)
		_snprintf(name,STRING_NUMBER,"%04d年 用電量及電費月報表<p></p>",year);
	else
		_snprintf(name,STRING_NUMBER,"Electricity Consumption Monthly Report<br>Year %04d<p></p>",year);
	

	zogdbx_ex_HTMLtableinitCenter(fp,name,1,2);

	zogdbx_ex_HTMLtableRowinit(fp);
	if(language)
	{
		zogdbx_ex_HTMLtableheadStr(fp,"月份");
		zogdbx_ex_HTMLtableheadStr(fp,"用電量 (度)");
		zogdbx_ex_HTMLtableheadStr(fp,"電費 (元)");
	}
	else
	{
		zogdbx_ex_HTMLtableheadStr(fp,"Month");
		zogdbx_ex_HTMLtableheadStr(fp,"Electricity Consumption (KWh)");
		zogdbx_ex_HTMLtableheadStr(fp,"Estimated Cost ($)");
	}
	
	zogdbx_ex_HTMLtableRowend(fp);

	total=0;

	for(i=1;i<13;i++)
	{
		if(i==11)
			i=11;
		if(12==i)
		{
			iUTC=zogdbx_in_TIMEtoUTC(year+1,1,1,0,0,1);
			
			flag= zogdst_in_DSTBetweenAny(year,i,1,
										year+1,1,1,
										iUTC,
										TimeZone,DSTflag,
										&ConsumeElect,
										&ConsumePrice);
		}
		else
		{
			iUTC=zogdbx_in_TIMEtoUTC(year,i+1,1,0,0,1);
			
			flag= zogdst_in_DSTBetweenAny(year,i,1,
										year,i+1,1,
										iUTC,
										TimeZone,DSTflag,
										&ConsumeElect,
										&ConsumePrice);
		}

		
		if((-1!=flag) && (0==first))
		{
			first=1;
		}

		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		if(language)
			_snprintf(name,STRING_NUMBER,"<b>%02d月份</b>",i);
		else
			_snprintf(name,STRING_NUMBER,"<div align=\"center\"><b>%s</b></div>",
	 						zogdst_in_MonthNameC(i));
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(-1!=flag)
		{
			//有效时间+1
			total++;
			totalConsumeElect +=ConsumeElect;
			totalConsumePrice +=ConsumePrice;
			zogdbx_ex_HTMLtablebody03Double(fp,ConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,ConsumePrice);
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}
	}

	// 部分 或全部有值
	zogdst_in_DownloadEnd(fp,
						totalConsumeElect,totalConsumePrice,total,
						first,language);
	
	return 1;
}

//下载DST一月里一天一条的报告
//输入年 月
//时区 以及是否使用夏时令 是=1 否=0			
//失败-1
//成功1
//输入 语言1=台湾0=加拿大(默认) 
int zogdst_ex_DownloadDSTMonthDay(TCHAR *dir,int year,int month,
											int TimeZone,int DSTflag,int language)
{
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0,first=0;
	int total=0;
	double totalConsumeElect=0.0;
	double totalConsumePrice=0.0;
	
	int dayflag;
	
	unsigned long int startUTC,endUTC,iUTC;
	int iyear,imonth,iday,ihour,imin,isec;

	FILE *fp=NULL;
	char name[STRING_NUMBER]={0};
	
	TCHAR zogdbxFILE[MAX_PATH]={0};
	
	//创建路径
	if(language)
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\日報表 %04d年%02d月.html"),dir,year,month);
	else
		_sntprintf(zogdbxFILE,MAX_PATH,_T("%s\\Daily Report %s %04d.html"),
								dir,zogdst_in_MonthNameT(month),year);

	//不可以有B
	int err=_tfopen_s(&fp,zogdbxFILE,_T("w+"));
	if( 0!=err )
		return -1;
	//if( !(fp=_tfopen(zogdbxFILE,_T("w+"))) )
	//	return -1;

	zogdbx_ex_HTMLinit(fp);
	if(language)
		_snprintf(name,STRING_NUMBER,"%04d年%02d月 用電量及電費月報表<p></p>",year,month);
	else
		_snprintf(name,STRING_NUMBER,"Electricity Consumption Monthly Report<br>%s %04d<p></p>",
									zogdst_in_MonthNameC(month),year);
	
	zogdbx_ex_HTMLtableinitCenter(fp,name,1,2);

	zogdbx_ex_HTMLtableRowinit(fp);
	if(language)
	{
		zogdbx_ex_HTMLtableheadStr(fp,"日期");
		zogdbx_ex_HTMLtableheadStr(fp,"用電量 (度)");
		zogdbx_ex_HTMLtableheadStr(fp,"電費 (元)");
	}
	else
	{
		zogdbx_ex_HTMLtableheadStr(fp,"mm/dd");
		zogdbx_ex_HTMLtableheadStr(fp,"Electricity Consumption (KWh)");
		zogdbx_ex_HTMLtableheadStr(fp,"Estimated Cost ($)");
	}
	
	zogdbx_ex_HTMLtableRowend(fp);

	//年月日转秒级
	startUTC=zogdbx_in_TIMEtoUTC(year,month,1,0,0,1);

	if(12==month)
		endUTC=zogdbx_in_TIMEtoUTC(year+1,1,1,0,0,1);
	else
		endUTC=zogdbx_in_TIMEtoUTC(year,month+1,1,0,0,1);
	
	total=0;
	//每次+一天的秒
	for(iUTC=startUTC;iUTC<endUTC;iUTC+=86400)
	{
		zogdbx_in_UTCtoTIME(iUTC,&iyear,&imonth,&iday,&ihour,&imin,&isec);
		//判断这天是什么时候	
		if(DSTflag)
			dayflag=zogdst_in_JudgeDSTDay(iyear,imonth,iday);
		else
			dayflag=2;
		//获取每个天的值
		flag=zogdst_in_DownloadDayLite(iUTC,TimeZone,dayflag,
										&ConsumeElect,
										&ConsumePrice);
		//获取初始值用于以后统计
		if((-1!=flag) && (0==first))
		{
			first=1;
		}
		zogdbx_ex_HTMLtableRowinit(fp);
		//时间
		if(1==language)
			_snprintf(name,STRING_NUMBER,"<b>%02d月%02d日</b>",imonth,iday);
		else
			_snprintf(name,STRING_NUMBER,"<div align=\"center\"><b>%02d/%02d</b></div>",imonth,iday);	
		
		zogdbx_ex_HTMLtablebodyStr(fp,name);
		if(-1!=flag)
		{
			//有效时间+1
			total++;
			totalConsumeElect +=ConsumeElect;
			totalConsumePrice +=ConsumePrice;
			zogdbx_ex_HTMLtablebody03Double(fp,ConsumeElect);
			zogdbx_ex_HTMLtablebody02Double(fp,ConsumePrice);
		}
		else
		{	
			//无数据
			zogdbx_ex_HTMLtablebodyNull(fp);
			zogdbx_ex_HTMLtablebodyNull(fp);
		}

		zogdbx_ex_HTMLtableRowend(fp);
		
	}
	// 部分 或全部有值
	zogdst_in_DownloadEnd(fp,
						totalConsumeElect,totalConsumePrice,total,
						first,language);
	return 1;
}


#if 0
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif

//返回 1有数据 -1无数据
//输入今天的日期 时区 是否夏令时
//返回今日耗电量和电费
int zogdst_ex_DSTToday(int year,int month,int day,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice)
{
	double HourConsumeElect;
	double HourConsumePrice;
	int dayflag,flag;
	int returnflag=-1;

	*TotalSumElect=0.0;
	*TotalSumPrice=0.0;
	
	unsigned long int iUTC=zogdbx_in_TIMEtoUTC(year,month,day,0,0,1);
	
	if(DSTflag)
		dayflag=zogdst_in_JudgeDSTDay(year,month,day);
	else
		dayflag=2;
	//获取每个天的值 00-23点
	flag=zogdst_in_DownloadDayLite(iUTC,TimeZone,dayflag,
										TotalSumElect,
										TotalSumPrice);
	if(-1!=flag)
	{
		returnflag=1;
	}
	else
	{
		*TotalSumElect=0.0;
		*TotalSumPrice=0.0;
	}

	flag=zogdbx_ex_GetCurrentHour(&HourConsumeElect,&HourConsumePrice);
	if(1==flag)
	{
		*TotalSumElect +=HourConsumeElect;
		*TotalSumPrice +=HourConsumePrice;
		returnflag=1;
	}
	
	return returnflag;
}

//返回 1成功
// 返回0成功但不完整 可能只有部分数据
//返回-1 信息未知文件出错 致命错误完全无数据
//输入今天的日期 时区 是否夏时令
//返回昨日的耗电量和电费
int zogdst_ex_DSTYesterday(int year,int month,int day,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice)
{
	int dayflag;
	int iyear,imonth,iday,ihour,imin,isec;
	//设置成昨日的00点
	unsigned long int iUTC=zogdbx_in_TIMEtoUTC(year,month,day,0,0,1)-86400;
	zogdbx_in_UTCtoTIME(iUTC,&iyear,&imonth,&iday,&ihour,&imin,&isec);
	
	if(DSTflag)
		dayflag=zogdst_in_JudgeDSTDay(iyear,imonth,iday);
	else
		dayflag=2; 
	
	//获取每个天的值 昨日的00-23
	return zogdst_in_DownloadDayLite(iUTC,TimeZone,dayflag,
										TotalSumElect,
										TotalSumPrice);
}

//返回0 部分数据 1有数据 -1无数据
//输入今天的日期 时区 是否夏时令
//返回上个月的耗电量和电费
int zogdst_ex_DSTLastMonth(int year,int month,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice)
{
	int syear,smonth;

	//将当前月减去一个月
	if(1==month)
	{
		syear=year-1;
		smonth=12;
	}
	else
	{
		syear=year;
		smonth=month-1;
	}

	unsigned long int endUTC=zogdbx_in_TIMEtoUTC(year,month,1,0,0,1);
		 
	return zogdst_in_DSTBetweenAny(syear,smonth,1,
							year,month,1,
							endUTC,
							TimeZone,DSTflag,
							TotalSumElect,
							TotalSumPrice);

}
//返回0 部分数据 1有数据 -1无数据
//输入今天的日期  前N天
//输入今天的日期 时区 是否夏时令
//返回上个N天的耗电量和电费 不含今天
int zogdst_ex_DSTLastNday(int year,int month,int day,int num,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice)
{
	int syear,smonth,sday,shour,smin,ssec;

	unsigned long int endUTC=zogdbx_in_TIMEtoUTC(year,month,day,0,0,1);

	zogdbx_in_UTCtoTIME(endUTC-num*86400,
								&syear,&smonth,&sday,&shour,&smin,&ssec);
	//类似上个月的流程
	return zogdst_in_DSTBetweenAny(syear,smonth,sday,
							year,month,day,
							endUTC,
							TimeZone,DSTflag,
							TotalSumElect,
							TotalSumPrice);

}

//返回0 部分数据 1有数据 -1无数据
//输入今天的日期 时区 是否夏时令
//返回上个N天的耗电量和电费 不含今天
//周日0 周一1.周六6
int zogdst_ex_DSTLastWeek(int year,int month,int day,int weekday,int TimeZone,int DSTflag,
							double  *TotalSumElect,double  *TotalSumPrice)
{
	int syear,smonth,sday,shour,smin,ssec;
	int eyear,emonth,eday,ehour,emin,esec;

	unsigned long int endUTC=zogdbx_in_TIMEtoUTC(year,month,day,0,0,1)-weekday*86400;
	//7*86400=604800
	zogdbx_in_UTCtoTIME(endUTC-604800,&syear,&smonth,&sday,&shour,&smin,&ssec);

	zogdbx_in_UTCtoTIME(endUTC,&eyear,&emonth,&eday,&ehour,&emin,&esec);
 
	return zogdst_in_DSTBetweenAny(syear,smonth,sday,
							eyear,emonth,eday,
							endUTC,
							TimeZone,DSTflag,
							TotalSumElect,
							TotalSumPrice);

}

#if 0 
// 非归一到24小时代码

//下载DST一小时一条数据的报告
//输入报告目录路径
//输入年月日  时区 以及是否使用夏时令 是=1 否=0
//返回num 数据的总数个数的地址
//返回Elect 电表读数数组，若数据无效数据为0
//返回Price 价格数组，若数据无效数据为0
void zogdst_ex_DSTHourReport(int year,int month,int day,
									int TimeZone,int DSTflag,
									int *num,double *Elect,double *Price)
{
	int dayflag;

	//开启DST
	if(DSTflag)
	{
		//int dayflag =1 夏日
		//int dayflag =2 冬日
		//int dayflag =3 夏时当日
		//int dayflag =4 冬时当日
		dayflag=zogdst_in_JudgeDSTDay(year,month,day);

		if(1==dayflag)
		{
			*num=24;
			zogdst_in_DSTHourReportLite(year,month,day,TimeZone-1,Elect, Price);
		}
		else 	if(2==dayflag)
		{
			*num=24;
			zogdst_in_DSTHourReportLite(year,month,day,TimeZone,Elect, Price);
		}
		else 	if(3==dayflag)
		{
			*num=25;
			zogdst_in_DSTHourReportSummer(year,month,day,TimeZone,Elect, Price);
		}
		else 	if(4==dayflag)
		{
			*num=23;
			zogdst_in_DSTHourReportWinter(year,month,day,TimeZone,Elect, Price);
		}
	}
	else
	{
		//无DST
		*num=24;
		zogdst_in_DSTHourReportLite(year,month,day,TimeZone,Elect, Price);
	}
	
}

//夏天或者冬天 要打印24条数据 
//返回Elect 电表读数数组，若数据无效数据为0
//返回Price 价格数组，若数据无效数据为0
void  zogdst_in_DSTHourReportLite(int year,int month,int day,int timezone,
										double *Elect,double *Price)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	int flag=0;
	int i;

	int iyear,imonth,iday,ihour,imin,isec;
	
	for(i=0;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-timezone*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
	
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[i]=HourConsumeElect;
			Price[i]=HourConsumePrice;
		}
		else
		{
			Elect[i]=0;
			Price[i]=0;
		}
	
	}
	
}

//当正好是夏令时当日 要打印25条数据 
//输入的时区为冬天 设置的正常时区
//返回Elect 电表读数数组，若数据无效数据为0
//返回Price 价格数组，若数据无效数据为0
void zogdst_in_DSTHourReportSummer(int year,int month,int day,int timezone,
										double *Elect,double *Price)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;
	
	int flag=0;
	int i;
	int total=0;
	
	int iyear,imonth,iday,ihour,imin,isec;
	
	total=0;
	//在跳转时刻zogdst_jumptime 进行跳转
	for(i=0;i<zogdst_jumptime;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-timezone*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[total]=HourConsumeElect;
			Price[total]=HourConsumePrice;
			total++;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
			total++;
		}
	}
	
	//在跳转时刻zogdst_jumptime 进行跳转
	for(i=zogdst_jumptime-1;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-(timezone-1)*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[total]=HourConsumeElect;
			Price[total]=HourConsumePrice;
			total++;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
			total++;
		}
	}
}

//当正好是冬令时当日 要打印23条数据 
//输入的时区为冬天 设置的正常时区
//返回Elect 电表读数数组，若数据无效数据为0
//返回Price 价格数组，若数据无效数据为0
void  zogdst_in_DSTHourReportWinter(int year,int month,int day,int timezone,
										double *Elect,double *Price)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	int flag=0;
	int i;
	int total=0;
	
	int iyear,imonth,iday,ihour,imin,isec;
	
	total=0;
	for(i=0;i<zogdst_jumptime;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-(timezone-1)*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[total]=HourConsumeElect;
			Price[total]=HourConsumePrice;
			total++;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
			total++;
		}
	}
	for(i=zogdst_jumptime+1;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-timezone*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[total]=HourConsumeElect;
			Price[total]=HourConsumePrice;
			total++;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
			total++;
		}
	}
}

#else
// 归一到24小时代码

//下载DST一小时一条数据的报告
//输入报告目录路径
//输入年月日  时区 以及是否使用夏时令 是=1 否=0
//返回num 数据的总数个数的地址
//返回Elect 电表读数数组，若数据无效数据为0
//返回Price 价格数组，若数据无效数据为0
void zogdst_ex_DSTHourReport(int year,int month,int day,
									int TimeZone,int DSTflag,
									int *num,double *Elect,double *Price)
{
	int dayflag;

	//开启DST
	if(DSTflag)
	{
		//int dayflag =1 夏日
		//int dayflag =2 冬日
		//int dayflag =3 夏时当日
		//int dayflag =4 冬时当日
		dayflag=zogdst_in_JudgeDSTDay(year,month,day);

		if(1==dayflag)
			zogdst_in_DSTHourReportLite(year,month,day,TimeZone-1,Elect, Price);
		else 	if(2==dayflag)
			zogdst_in_DSTHourReportLite(year,month,day,TimeZone,Elect, Price);
		else 	if(3==dayflag)
			zogdst_in_DSTHourReportSummer(year,month,day,TimeZone,Elect, Price);
		else 	if(4==dayflag)
			zogdst_in_DSTHourReportWinter(year,month,day,TimeZone,Elect, Price);
	}
	else
	{
		//无DST
		zogdst_in_DSTHourReportLite(year,month,day,TimeZone,Elect, Price);
	}

	*num=24;
}

//夏天或者冬天 要打印24条数据 
//返回Elect 电表读数数组，若数据无效数据为0
//返回Price 价格数组，若数据无效数据为0
void  zogdst_in_DSTHourReportLite(int year,int month,int day,int timezone,
										double *Elect,double *Price)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	int flag=0;
	int i;

	int iyear,imonth,iday,ihour,imin,isec;
	
	for(i=0;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-timezone*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
	
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[i]=HourConsumeElect;
			Price[i]=HourConsumePrice;
		}
		else
		{
			Elect[i]=0;
			Price[i]=0;
		}
	}
}

//当正好是夏令时当日 要打印25条数据 
//输入的时区为冬天 设置的正常时区
//返回Elect 电表读数数组，若数据无效数据为0
//返回Price 价格数组，若数据无效数据为0
void zogdst_in_DSTHourReportSummer(int year,int month,int day,int timezone,
										double *Elect,double *Price)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;
	
	int flag=0;
	int i;
	int total=0;
	
	int iyear,imonth,iday,ihour,imin,isec;
	
	total=0;
	//在跳转时刻zogdst_jumptime 进行跳转
	for(i=0;i<zogdst_jumptime;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-timezone*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[total]=HourConsumeElect;
			Price[total]=HourConsumePrice;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
		}
		total++;
	}

	zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,zogdst_jumptime-1,0,1)-(timezone-1)*3600,
											&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
	//获取每个小时的值
	flag=zogdbx_ex_GetEveryHour(0,
								iyear,imonth,iday,ihour,
								&HourSumElect,
								&HourSumPrice,
								&HourConsumeElect,
								&HourConsumePrice);
	total--;
	//获取初始值用于以后统计
	if(1==flag )
	{
		Elect[total]+=HourConsumeElect;
		Price[total]+=HourConsumePrice;
	}
	total++;
	
	//在跳转时刻zogdst_jumptime 进行跳转
	for(i=zogdst_jumptime;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-(timezone-1)*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[total]=HourConsumeElect;
			Price[total]=HourConsumePrice;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
		}
		total++;
	}
}

//当正好是冬令时当日 要打印23条数据 
//输入的时区为冬天 设置的正常时区
//返回Elect 电表读数数组，若数据无效数据为0
//返回Price 价格数组，若数据无效数据为0
void  zogdst_in_DSTHourReportWinter(int year,int month,int day,int timezone,
										double *Elect,double *Price)
{
	double  HourConsumeElect;
	double  HourConsumePrice;
	double  HourSumElect;
	double  HourSumPrice;

	int flag=0;
	int i;
	int total=0;
	
	int iyear,imonth,iday,ihour,imin,isec;
	
	total=0;
	for(i=0;i<zogdst_jumptime;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-(timezone-1)*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[total]=HourConsumeElect;
			Price[total]=HourConsumePrice;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
		}
		total++;
	}
	//多一个
	Elect[total]=0;
	Price[total]=0;
	total++;
	
	for(i=zogdst_jumptime+1;i<24;i++)
	{
		zogdbx_in_UTCtoTIME(zogdbx_in_TIMEtoUTC(year,month,day,i,0,1)-timezone*3600,
							&iyear,&imonth,&iday,&ihour,&imin,&isec);
		
		//获取每个小时的值
		flag=zogdbx_ex_GetEveryHour(0,
									iyear,imonth,iday,ihour,
									&HourSumElect,
									&HourSumPrice,
									&HourConsumeElect,
									&HourConsumePrice);
		//获取初始值用于以后统计
		if(1==flag )
		{
			Elect[total]=HourConsumeElect;
			Price[total]=HourConsumePrice;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
		}
		total++;
	}
}

#endif
//下载DST一周里一天一条的报告
//输入年 
//输入当年一月一日 是星期几weekday:周日0 周一1.周六6
// 输入第几周weekN =1到53
//时区 以及是否使用夏时令 是=1 否=0
void zogdst_ex_DSTWeekDayReport(int year,int weekday,int weekN,
										int TimeZone,int DSTflag,
										int *num,double *Elect,double *Price)
{
	int i;
	int dayflag;
	
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0;
	int total=0;

	unsigned long int startUTC,endUTC,iUTC,eiUTC;
	int syear,smonth,sday,shour,smin,ssec;

	if(weekN<1)
	{
		for(i=0;i<7;i++)
		{
			Elect[i]=0;
			Price[i]=0;
		}
		*num=7;
		//出错
		return ;
	}
	
	if(1==weekN)
	{
		startUTC=zogdbx_in_TIMEtoUTC(year,1,1,0,0,1);
		endUTC=zogdbx_in_TIMEtoUTC(year,1,1+(7-weekday),0,0,1);
		
		for(i=0;i<weekday;i++)
		{
			Elect[total]=0;
			Price[total]=0;
			total++;
		}
	}
	else
	{
		iUTC=zogdbx_in_TIMEtoUTC(year,1,1+(7-weekday),0,0,1);

		startUTC=iUTC+604800*(weekN-2);
		endUTC=iUTC+604800*(weekN-1);

		total=0;
		
		eiUTC=zogdbx_in_TIMEtoUTC(year+1,1,1,0,0,1);
		//按最后一周来算//否则为今年内部周
		if(endUTC>=eiUTC)
		{
			for(i=51;i<=53;i++)
			{
				startUTC=iUTC+604800*(i-2);
				endUTC=iUTC+604800*(i-1);
				if(endUTC>=eiUTC)
					break;
			}
			
			endUTC=eiUTC;
		}
	}
	
	//每次+一天的秒
	for(iUTC=startUTC;iUTC<endUTC;iUTC+=86400)
	{
		zogdbx_in_UTCtoTIME(iUTC,&syear,&smonth,&sday,&shour,&smin,&ssec);
		//判断这天是什么时候	
		if(DSTflag)
			dayflag=zogdst_in_JudgeDSTDay(syear,smonth,sday);
		else
			dayflag=2;
		//获取每个天的值
		flag=zogdst_in_DownloadDayLite(iUTC,TimeZone,dayflag,
										&ConsumeElect,
										&ConsumePrice);
		if(-1!=flag )
		{
			Elect[total]=ConsumeElect;
			Price[total]=ConsumePrice;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
		}
		total++;
	}
	
	while(total<7)
	{
		Elect[total]=0;
		Price[total]=0;
		total++;
	}

	*num=7;
}

//DST一月里一天一条的报告
//输入年 月
//时区 以及是否使用夏时令 是=1 否=0				
void zogdst_ex_DSTMonthDayReport(int year,int month,
											int TimeZone,int DSTflag,
											int *num,double *Elect,double *Price)
{
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0;
	int total=0;
	int dayflag;
	
	unsigned long int startUTC,endUTC,iUTC;
	int iyear,imonth,iday,ihour,imin,isec;
	
	//年月日转秒级
	startUTC=zogdbx_in_TIMEtoUTC(year,month,1,0,0,1);

	if(12==month)
		endUTC=zogdbx_in_TIMEtoUTC(year+1,1,1,0,0,1);
	else
		endUTC=zogdbx_in_TIMEtoUTC(year,month+1,1,0,0,1);
	
	total=0;
	//每次+一天的秒
	for(iUTC=startUTC;iUTC<endUTC;iUTC+=86400)
	{
		zogdbx_in_UTCtoTIME(iUTC,&iyear,&imonth,&iday,&ihour,&imin,&isec);
		//判断这天是什么时候	
		if(DSTflag)
			dayflag=zogdst_in_JudgeDSTDay(iyear,imonth,iday);
		else
			dayflag=2;
		//获取每个天的值
		flag=zogdst_in_DownloadDayLite(iUTC,TimeZone,dayflag,
										&ConsumeElect,
										&ConsumePrice);
		if(-1!=flag )
		{
			Elect[total]=ConsumeElect;
			Price[total]=ConsumePrice;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
		}
		total++;
	}
	
	*num=total;
}


//下载DST一周一条的报告
//输入年 
//时区 以及是否使用夏时令 是=1 否=0
//第一季1.1-3.31 ==season =1
//第二季4.1-6.30 ==season =2
//第三季7.1-9.30 ==season =3
//第四季10.1-12.31 ==season =4
//每一季起始日的星期几 :周日0 周一1.周六6
void zogdst_ex_DSTWeekReport(int year,int season,int weekday,
										int TimeZone,int DSTflag,
										int *num,double *Elect,double *Price)
{
	int i;
	int fsmonth,fsday,eeyear,eemonth,eeday;
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0;
	int total=0;

	unsigned long int startUTC,endUTC,iUTC;
	int syear,smonth,sday,shour,smin,ssec;
	int eyear,emonth,eday,ehour,emin,esec;
		
	switch(season)
	{
		case 1:	
			fsmonth=1;fsday=1;
			eeyear=year;eemonth=4;eeday=1;
			break;
		case 2:	
			fsmonth=4;fsday=1;
			eeyear=year;eemonth=7;eeday=1;
			break;
		case 3:	
			fsmonth=7;fsday=1;
			eeyear=year;eemonth=10;eeday=1;
			break;
		case 4:	
			fsmonth=10;fsday=1;
			eeyear=year+1;eemonth=1;eeday=1;
			break;
		default:
			return ;
	}
	
	total=0;
	//显示第一周
	startUTC=zogdbx_in_TIMEtoUTC(year,fsmonth,fsday+(7-weekday),0,0,1);

	flag= zogdst_in_DSTBetweenAny(year,fsmonth,fsday,
							year,fsmonth,fsday+(7-weekday),
							startUTC,
							TimeZone,DSTflag,
							&ConsumeElect,
							&ConsumePrice);
	if(-1!=flag )
	{
		Elect[total]=ConsumeElect;
		Price[total]=ConsumePrice;
		total++;
	}
	else
	{
		Elect[total]=0;
		Price[total]=0;
		total++;
	}
	
	////////////////////////////显示中间周
	endUTC=zogdbx_in_TIMEtoUTC(eeyear,eemonth,eeday,0,0,1);
		
	//每次+一周的秒
	for(i=2,iUTC=startUTC;(iUTC+604800)<endUTC;iUTC+=604800,i++)
	{
		zogdbx_in_UTCtoTIME(iUTC,&syear,&smonth,&sday,&shour,&smin,&ssec);
		zogdbx_in_UTCtoTIME(iUTC+604800,&eyear,&emonth,&eday,&ehour,&emin,&esec);
		
		flag= zogdst_in_DSTBetweenAny(syear,smonth,sday,
							eyear,emonth,eday,
							iUTC+604800,
							TimeZone,DSTflag,
							&ConsumeElect,
							&ConsumePrice);
		if(-1!=flag )
		{
			Elect[total]=ConsumeElect;
			Price[total]=ConsumePrice;
			total++;
		}
		else
		{
			Elect[total]=0;
			Price[total]=0;
			total++;
		}
	}

	//显示最后一周
	zogdbx_in_UTCtoTIME(iUTC,&syear,&smonth,&sday,&shour,&smin,&ssec);
	
	flag= zogdst_in_DSTBetweenAny(syear,smonth,sday,
							eeyear,eemonth,eeday,
							endUTC,
							TimeZone,DSTflag,
							&ConsumeElect,
							&ConsumePrice);
	if(-1!=flag )
	{
		Elect[total]=ConsumeElect;
		Price[total]=ConsumePrice;
		total++;
	}
	else
	{
		Elect[total]=0;
		Price[total]=0;
		total++;
	}
	
	*num=total;
}

//下载DST一月一条的报告
//输入年 
//时区 以及是否使用夏时令 是=1 否=0
//返回num 数据的总数个数的地址
//返回Elect 电表读数数组，若数据无效数据为0
 //返回Price 价格数组，若数据无效数据为0
void zogdst_ex_DSTMonthReport(int year,int TimeZone,int DSTflag,
										int *num,double *Elect,double *Price)
{
	int i;
	double  ConsumeElect;
	double  ConsumePrice;
	
	int flag=0;
	unsigned long int iUTC;
		
	*num=12;

	for(i=1;i<13;i++)
	{
		if(i==11)
			i=11;
		if(12==i)
		{
			iUTC=zogdbx_in_TIMEtoUTC(year+1,1,1,0,0,1);
			
			flag= zogdst_in_DSTBetweenAny(year,i,1,
										year+1,1,1,
										iUTC,
										TimeZone,DSTflag,
										&ConsumeElect,
										&ConsumePrice);
		}
		else
		{
			iUTC=zogdbx_in_TIMEtoUTC(year,i+1,1,0,0,1);
			
			flag= zogdst_in_DSTBetweenAny(year,i,1,
										year,i+1,1,
										iUTC,
										TimeZone,DSTflag,
										&ConsumeElect,
										&ConsumePrice);
		}

		if(-1!=flag )
		{
			Elect[i-1]=ConsumeElect;
			Price[i-1]=ConsumePrice;
		}
		else
		{
			Elect[i-1]=0;
			Price[i-1]=0;
		}
	}

}

#if 0
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif
//返回 1成功
// 返回0成功但不完整 可能只有部分数据
//返回-1 信息未知文件出错 致命错误完全无数据
//输入起始年月日 结束年月日 时区 是否夏时令
//输出耗电量 和电费
//计算 起始年月日 0点0分0秒 到 结束年月日0时0分0秒的数据
int zogdst_in_DSTBetweenAny(int syear,int smonth,int sday,
								int eyear,int emonth,int eday,
								int endUTC,
								int TimeZone,int DSTflag,
								double  *TotalSumElect,
								double  *TotalSumPrice)
{
	double HourConsumeElect;
	double HourConsumePrice;
	
	if(-1!=zogdst_in_DSTBetweenDay(syear,smonth,sday,
							eyear,emonth,eday,
							TimeZone,DSTflag,
							TotalSumElect,
							TotalSumPrice))

	{
		//减去当前这个小时
		if(1==zogdst_in_DSTBetweenHour( eyear, emonth,eday,
										endUTC,
										TimeZone, DSTflag,
										&HourConsumeElect,
										&HourConsumePrice))
		{
			*TotalSumElect -=HourConsumeElect;
			*TotalSumPrice -=HourConsumePrice;
		}
		return 1;
	}
	
	return -1;
}

//当天的0点0分到1点0分的耗电量
int zogdst_in_DSTBetweenHour(int year,int month,int day,
									unsigned long int UTC,
								int TimeZone,int DSTflag,
								double  *HourConsumeElect,
								double  *HourConsumePrice)
{
	int dayflag;
	int flag;
	double HourSumElect;
	double HourSumPrice;
	int syear,smonth,sday,shour,smin,ssec;
	
	//减去当前这个小时
	if(DSTflag)
		dayflag=zogdst_in_JudgeDSTDay(year,month,day);
	else
		dayflag=2;
	
	if(1==dayflag)
		zogdbx_in_UTCtoTIME(UTC-(TimeZone-1)*3600,
							&syear,&smonth,&sday,&shour,&smin,&ssec);
	else if(2==dayflag)
		zogdbx_in_UTCtoTIME(UTC-TimeZone*3600,
								&syear,&smonth,&sday,&shour,&smin,&ssec);
	else  if(3==dayflag)
		zogdbx_in_UTCtoTIME(UTC-TimeZone*3600,
								&syear,&smonth,&sday,&shour,&smin,&ssec);
	else if(4==dayflag)
		zogdbx_in_UTCtoTIME(UTC-(TimeZone-1)*3600,
								&syear,&smonth,&sday,&shour,&smin,&ssec);
	else
		return -1;

	//获取每个小时的值
	flag=zogdbx_ex_GetEveryHour(0,
								syear,smonth,sday,shour,
								&HourSumElect,
								&HourSumPrice,
								HourConsumeElect,
								HourConsumePrice);
	
	return flag;
}
		
//返回 1成功
// 返回0成功但不完整 可能只有部分数据
//返回-1 信息未知文件出错 致命错误完全无数据
//输入起始年月日 结束年月日 时区 是否夏时令
//输出耗电量 和电费
//计算 起始年月日 0点0分0秒 到 结束年月日0时59分59秒的数据
int zogdst_in_DSTBetweenDay(int syear,int smonth,int sday,
								int eyear,int emonth,int eday,
								int TimeZone,int DSTflag,
								double  *TotalSumElect,
								double  *TotalSumPrice)
{	

	int dayflag;
	
	unsigned long int startUTC,endUTC;
	int s0year,s0month,s0day,s0hour,s0min,s0sec;
	int e0year,e0month,e0day,e0hour,e0min,e0sec;
	#if ZOGDST_DEBUG 
	printf("%d,%d,%d,%d,%d,%d,\n", syear, smonth, sday,
								 eyear, emonth,eday);
	#endif
	if(DSTflag)
		dayflag=zogdst_in_JudgeDSTDay(syear,smonth,sday);
	else
		dayflag=2;
	//默认均为0点 
	startUTC=zogdbx_in_TIMEtoUTC(syear,smonth,sday,0,0,1);
	
	if(1==dayflag)
		zogdbx_in_UTCtoTIME(startUTC-(TimeZone-1)*3600,
							&s0year,&s0month,&s0day,&s0hour,&s0min,&s0sec);
	else if(2==dayflag)
		zogdbx_in_UTCtoTIME(startUTC-TimeZone*3600,
							&s0year,&s0month,&s0day,&s0hour,&s0min,&s0sec);
	else  if(3==dayflag)
		zogdbx_in_UTCtoTIME(startUTC-TimeZone*3600,
							&s0year,&s0month,&s0day,&s0hour,&s0min,&s0sec);
	else if(4==dayflag)
		zogdbx_in_UTCtoTIME(startUTC-(TimeZone-1)*3600,
							&s0year,&s0month,&s0day,&s0hour,&s0min,&s0sec);
	else
		return -1;

	//结束
	if(DSTflag)
		dayflag=zogdst_in_JudgeDSTDay(eyear,emonth,eday);
	else
		dayflag=2;
	//默认均为0点
	endUTC=zogdbx_in_TIMEtoUTC(eyear,emonth,eday,0,0,1);
	
	if(1==dayflag)
		zogdbx_in_UTCtoTIME(endUTC-(TimeZone-1)*3600,
							&e0year,&e0month,&e0day,&e0hour,&e0min,&e0sec);
	else if(2==dayflag)
		zogdbx_in_UTCtoTIME(endUTC-TimeZone*3600,
							&e0year,&e0month,&e0day,&e0hour,&e0min,&e0sec);
	else  if(3==dayflag)
		zogdbx_in_UTCtoTIME(endUTC-TimeZone*3600,
							&e0year,&e0month,&e0day,&e0hour,&e0min,&e0sec);
	else if(4==dayflag)
		zogdbx_in_UTCtoTIME(endUTC-(TimeZone-1)*3600,
							&e0year,&e0month,&e0day,&e0hour,&e0min,&e0sec);
	else
		return -1;

	//获取这一区间
	dayflag= zogdbx_ex_GetBetweenHour_YMDH(s0year,s0month,s0day,s0hour,
										e0year,e0month,e0day,e0hour,
										TotalSumElect,
										TotalSumPrice);	
	#if ZOGDST_DEBUG 
	printf("%d=%d,%d,%d,%d=%d,%d,%d,%d,\n", dayflag,s0year,s0month,s0day,s0hour,
										e0year,e0month,e0day,e0hour);
	#endif
	return dayflag;
}

