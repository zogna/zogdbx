//#include "stdafx.h"
#include "zogdbx.h"

#define zogdbx_SECONDS_IN_YEAR	31536000
#define zogdbx_SECONDS_IN_DAY	86400
#define zogdbx_SECONDS_IN_HOUR	3600
#define zogdbx_SECONDS_IN_MIN	60
int zogdbx_emberAfDaysInMonth[12] ={ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };  

void zogdbx_in_UTCtoTIME(unsigned long int utcTime,
			int *year,int *month,int *day,int *hour,int *min,int *sec)
{   
	unsigned long int i;  
	int daysInMonth;  
	unsigned long int  daysSince2000 = utcTime / zogdbx_SECONDS_IN_DAY; 
	unsigned long int  secToday = utcTime - (daysSince2000 * zogdbx_SECONDS_IN_DAY);   

	*year = 2000;   
	*month = 1;   
	*day = 1; 
	*hour = (int)(secToday / zogdbx_SECONDS_IN_HOUR);   
	*min = (int)((secToday - ( (*hour) * zogdbx_SECONDS_IN_HOUR)) / 60);   
	*sec = (int)(secToday - (((*hour) * zogdbx_SECONDS_IN_HOUR) + ( (*min) * 60)));   

	int isLeapYear = 1; 

	for (i = 0; i < daysSince2000; i++)   
	{         
		if (isLeapYear && (2 == (*month) )) 	   
		{           
			daysInMonth = 29;       
		}	 
		else 
		{      
			daysInMonth = zogdbx_emberAfDaysInMonth[ (*month) - 1];    
		}    
		if (daysInMonth == (*day) ) 
		{      
			(*month)++;
			*day = 1;
		} 
		else 
		{      
			(*day)++;    
		}    
		if (13 == (*month) ) 
		{      
			(*year)++;      
			(*month) = 1;      
			if ( (*year) % 4 == 0 &&( (*year) % 100 != 0 ||( (*year) % 400 == 0)))        
				isLeapYear = 1;      
			else        
				isLeapYear = 0;    
		}  
	}
}


//本地时间转换成以2000年开始计算的utc时间
unsigned long int zogdbx_in_TIMEtoUTC(int year,int month,int day,int hour,int min,int sec)
{
	unsigned long int utc_time;
	int sum,year_day;
	int cout_year=year-2000;

	if(cout_year-1<0)
		year_day=0;
	else
		year_day=cout_year*365+((cout_year-1)/4)+1;

	if (year % 4 == 0 &&(year % 100 != 0 ||(year % 400 == 0)))
	{
		switch(month)//先计算某月以前月份的总天数。 
			{ 
				case 1:sum=0;break; 
				case 2:sum=31;break; 
				case 3:sum=60;break; 
				case 4:sum=91;break; 
				case 5:sum=121;break; 
				case 6:sum=152;break; 
				case 7:sum=182;break; 
				case 8:sum=213;break; 
				case 9:sum=244;break; 
				case 10:sum=274;break; 
				case 11:sum=305;break; 
				case 12:sum=335;break; 
				default:return 0; 
			} 
	}
	else
	{
			//先计算某月以前月份的总天数。 
			switch(month)
			{ 
				case 1:sum=0;break; 
				case 2:sum=31;break; 
				case 3:sum=59;break; 
				case 4:sum=90;break; 
				case 5:sum=120;break; 
				case 6:sum=151;break; 
				case 7:sum=181;break; 
				case 8:sum=212;break; 
				case 9:sum=243;break; 
				case 10:sum=273;break; 
				case 11:sum=304;break; 
				case 12:sum=334;break; 
				default:return 0; 
			}
	}

	year_day +=sum+day-1;
	utc_time =year_day*zogdbx_SECONDS_IN_DAY+hour*zogdbx_SECONDS_IN_HOUR+min*zogdbx_SECONDS_IN_MIN+sec;

	return utc_time;
}