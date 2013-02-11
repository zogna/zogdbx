#include <stdio.h>

struct EVERYHOUR
{
	//UTC+0 日日时时
	unsigned int HourTime;
	double  HourSumElect;
	double  HourSumPrice;
	double  HourConsumeElect;
	double  HourConsumePrice;
};

struct EVERYHOUR Everyhour={0};

int main( int nArgc, char ** papszArgv )
{
	char *pszFilename;
	char outfilename[260];
	FILE *fp;
	FILE *outfp;

	if(nArgc <=1)
	{
		printf( "Usage: 201102.b \n" );
		getchar();
		return 0;
	}
    for( int iArg = 1; iArg < nArgc; iArg++ )
    {
        pszFilename = papszArgv[iArg];
		 
		if( pszFilename == NULL )
		{
			printf( "Usage: 201102.b \n" );
			getchar();
			return 0;
		}

		sprintf(outfilename,"%s.txt",pszFilename);
		outfp=fopen(outfilename,"w+");

		fprintf(outfp,"比如2302\n23号02:00-02:59区间的耗电量和电费\n02:59时刻的电表读数和总价格\n");
		fprintf(outfp,"日时\t小时末电表读数\t小时末总价格\t该小时耗电量\t该小时的电费\n");

		if( fp=fopen(pszFilename,"rb") )
		{	
			while(!feof(fp))
			{
				if(fread(&Everyhour, sizeof(struct EVERYHOUR), 1, fp))
				{
					fprintf(outfp,"%04u\t%lf\t%lf\t%lf\t%lf\n",Everyhour.HourTime,
													Everyhour.HourSumElect,
													Everyhour.HourSumPrice,
													Everyhour.HourConsumeElect,
													Everyhour.HourConsumePrice);
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