#include <stdio.h>

struct EVERYTIME
{
	//UTC+0 ��ʱ��
	unsigned long int CurrentUTC;
	//���õ���
	double  CurrentSumElect;
	//��ǰ�ܼ�
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

		fprintf(outfp,"����UTCʱ��\n23��02:15:30-02:16:30����ĺĵ����͵��\n02:16:30ʱ�̵ĵ��������ܼ۸�\n");
		fprintf(outfp,"UTC��\t\t������\t�÷���ĩ���ܼ۸�\t�÷��Ӻĵ���\t�÷��ӵĵ��\n");

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
					//�ļ�����
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

