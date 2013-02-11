#include <stdio.h>

struct EVERYHOUR
{
	//UTC+0 ����ʱʱ
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

		fprintf(outfp,"����2302\n23��02:00-02:59����ĺĵ����͵��\n02:59ʱ�̵ĵ��������ܼ۸�\n");
		fprintf(outfp,"��ʱ\tСʱĩ������\tСʱĩ�ܼ۸�\t��Сʱ�ĵ���\t��Сʱ�ĵ��\n");

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