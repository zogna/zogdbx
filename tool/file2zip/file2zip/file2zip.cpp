#include "windows.h"
#include "stdio.h"
#include "string.h"

//���ѹ��100���ļ�
#define MAX_FILE 100

struct FILE_INFO
{
	char FileName[260];
	unsigned long int FileLength;
};
//��ȡ����ļ���
char *GetName(char *file)
{
	char *p=NULL;
	//��ȡ���һ��'\'
	while(*file != NULL)
	{
		if('\\'== *file)
			p=file;

		file++;
	}

	return (p+1);
}
//����ѹ���ļ���·��
void SetOutName(char *infile,char *outfile)
{
	char *p=NULL;

	char *startp=infile;
	//��ȡ���һ��'\'
	while(*infile != NULL)
	{
		if('\\'== *infile)
			p=infile;

		infile++;
	}

	infile=startp;
	//����·����Ŀ¼
	while(infile != p)
	{
		*outfile=*infile;
		outfile++;
		infile++;
	}
	//���÷����
	*outfile='\\';
	outfile++;
	*outfile='C';
	outfile++;
	*outfile='I';
	outfile++;
	*outfile='D';
	outfile++;
	*outfile='.';
	outfile++;
	*outfile='u';
	outfile++;
	*outfile='p';
	outfile++;
	*outfile='d';
	outfile++;
	*outfile='a';
	outfile++;
	*outfile='t';
	outfile++;
	*outfile='e';
	outfile++;
	*outfile='\0';
	outfile++;
}

int main( int number, char ** filename )
{
	int	i;
	FILE *infp=NULL;
	FILE *outfp=NULL;

	char outfile[260]={0};

	unsigned char *temp=NULL;

	struct FILE_INFO FileInfo[MAX_FILE]={{0}};
	int totalfile=0;
	//���ñ��
	char sign[20]="LILLIPUT_CID";
	unsigned long int time=0;

	if(number <=1)
	{
		printf( "Usage: file....\n" );
		getchar();
		return 0;
	}
	//�������ʱ��
	printf("input update time\nformat:yyyymmdd\nfor example:20140715\n");
	scanf("%lu",&time);
	
	//ͳ���ļ����� �ļ���Ŀ
	for(i = 1;i < number; i++ )
    {
		if( filename[i] == NULL )
		{
			printf( "Usage: file....\n" );
			getchar();
			getchar();
			return 0;
		}

		if(infp=fopen(filename[i],"rb"))
		{
			//���� �ļ���
			sprintf(FileInfo[totalfile].FileName,"%s",GetName(filename[i]));
			//�����ļ�����
			fseek(infp,0,SEEK_END);
			FileInfo[totalfile].FileLength=ftell(infp);

			totalfile++;

			fclose(infp);
			infp=NULL;
		}
		else
		{		
			printf("%s file con't read\n",filename[i]);
			getchar();
			getchar();
			return 0;
		}
	}
	//֮��ǵö�ҪFREE
	temp=(unsigned char *)calloc(1,sizeof(unsigned char));

	//�����ļ���
	SetOutName(filename[1],outfile);

	if(outfp=fopen(outfile,"wb"))
	{
		//дͷ���
		fwrite(&sign,sizeof(char),20,outfp);
		fwrite(&time,sizeof(unsigned long int),1,outfp);
		//д�ļ���
		fwrite(&totalfile,sizeof(int),1,outfp);
		//д�ļ��������ļ���
		for(i=0;i<totalfile;i++)
			fwrite(&FileInfo[i],sizeof(struct FILE_INFO),1,outfp);

		//д�ļ�����
		for(i=0;i<totalfile;i++)
		{
			temp=(unsigned char *)realloc(temp,FileInfo[i].FileLength*sizeof(unsigned char));
			//����+1 �±��1��ʼ
			if(infp=fopen(filename[i+1],"rb"))
			{
				fread(temp,sizeof(unsigned char),FileInfo[i].FileLength,infp);
	
				fclose(infp);
				infp=NULL;
			}
			else
			{		
				if(temp)
				{
					free(temp);
					temp=NULL;
				}
				printf("%s file con't read 2\n",filename[i+1]);
				getchar();
				getchar();
				return 0;
			}
			
			fwrite(temp,sizeof(unsigned char),FileInfo[i].FileLength,outfp);
		}

		fclose(outfp);
		outfp=NULL;
	}
	else
	{		
		if(temp)
		{
			free(temp);
			temp=NULL;
		}
		printf("%s file con't write\n",outfile);
		getchar();
		getchar();
		return 0;
	}

	if(temp)
	{
		free(temp);
		temp=NULL;
	}
	printf("OK\n");
		getchar();
				getchar();
	
}