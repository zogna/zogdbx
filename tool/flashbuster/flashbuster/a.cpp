#include "windows.h"
#include "tchar.h"
#include "stdio.h"


int wmain(void)
{
	unsigned long int i,j;
	unsigned long int a,b;
	FILE *fp=NULL;

	for(i=0;i<1000000;i++)
	{
		if(fp=_tfopen(_T("\\Flash Disk\\a.txt"),_T("w+")))
		{
			fflush(fp);
			fclose(fp);
		
			fp=NULL;
			Sleep(5);
		}
		else
		{
			printf("con't w+ \n");
			getchar();
			return 0;
		}

		for(j=0;j<100;j++)
		{
			if(fp=_tfopen(_T("\\Flash Disk\\a.txt"),_T("a+")))
			{
				fprintf(fp,"%lu %lu\n",i,j);
				//printf("%lu,%lu\n",i,j);
				fflush(fp);
				fclose(fp);
			
				fp=NULL;
				Sleep(5);
			}
			else
			{
				printf("con't a+ \n");
				getchar();
				return 0;
			}
		}

		if(fp=_tfopen(_T("\\Flash Disk\\a.txt"),_T("r")))
		{
			
			fscanf(fp,"%lu %lu\n",&a,&b);

			if(a!=i || b!=0)
			{
				printf("error %lu,%lu \n",a,b);
				getchar();
				return 0;
			}
			
			fflush(fp);
			fclose(fp);
		
			fp=NULL;
			Sleep(5);
		}
		else
		{
			printf("con't r \n");
			getchar();
			return 0;
		}
		printf("%lu\n",i);
	
	}

	return 0;
}