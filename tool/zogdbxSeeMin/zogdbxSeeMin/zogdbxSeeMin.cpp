#include <stdio.h>

struct EVERYTIME
{
	//UTC+0 秒时间
	unsigned long int CurrentUTC;
	//总用电量
	double  CurrentSumElect;
	//当前总价
	double	CurrentSumPrice;
	double  CurrentConsumeElect;
	double  CurrentConsumePrice;
};

struct EVERYTIME Everytime={0};

void zogdbx_in_UTCtoTIME(unsigned long int utcTime,
			int *year,int *month,int *day,int *hour,int *min,int *sec);

int year,month,day,hour,min,sec;

int main( int nArgc, char ** papszArgv )
{
	char *pszFilename;
	char outfilename[260];
	FILE *fp;
	FILE *outfp;

	if(nArgc <=1)
	{
		printf( "Usage: EVERYTIME\n" );
		getchar();
		return 0;
	}
    for( int iArg = 1; iArg < nArgc; iArg++ )
    {
        pszFilename = papszArgv[iArg];
		 
		if( pszFilename == NULL )
		{
			printf( "Usage: EVERYTIME\n" );
			getchar();
			return 0;
		}

		sprintf(outfilename,"%s.txt",pszFilename);
		outfp=fopen(outfilename,"w+");

		fprintf(outfp,"比如UTC时间\n23号02:15:30-02:16:30区间的耗电量和电费\n02:16:30时刻的电表读数和总价格\n");
		fprintf(outfp,"UTC秒\t\t电表读数\t该分钟末的总价格\t该分钟耗电量\t该分钟的电费\n");

		if( fp=fopen(pszFilename,"rb") )
		{	
			while(!feof(fp))
			{
				if(fread(&Everytime, sizeof(struct EVERYTIME), 1, fp))
				{
					zogdbx_in_UTCtoTIME(Everytime.CurrentUTC,&year,&month,&day,&hour,&min,&sec);

					fprintf(outfp,"%010lu\t%04d-%02d-%02d-%02d-%02d-%02d\t%lf\t%lf\t%lf\t%lf\n",
												Everytime.CurrentUTC,
												year,month,day,hour,min,sec,
													Everytime.CurrentSumElect,
													Everytime.CurrentSumPrice,
													Everytime.CurrentConsumeElect,
													Everytime.CurrentConsumePrice);
				}
				else 
					//文件到底
					break;
			}
		
			fclose(fp);
			fclose(outfp);
		}
		else
		{
			printf( "unable open file\n" );
			getchar();
			return 0;
		}

    }
}



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

