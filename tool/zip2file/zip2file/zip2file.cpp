#include "windows.h"
#include "stdio.h"
#include "string.h"

struct FILE_INFO
{
	char FileName[260];
	unsigned long int FileLength;
};

//标记="LILLIPUT_CID"

#define ZIP2FILE_DEBUG 1

int  zip2file(char *infile,char *signmark,unsigned long int *curtime,char *outDir);

int main( int number, char ** filename )
{
	unsigned long int curtime=0;
	
	if(number <=1)
	{
		printf( "Usage: file....\n" );
		getchar();
		return 0;
	}
	zip2file(filename[1],"LILLIPUT_CID",&curtime,"e:\\");
}

//返回1 成功 返回0失败
int  zip2file(char *infile,char *signmark,unsigned long int *curtime,char *outDir)
{
	int	i;
	FILE *infp=NULL;
	FILE *outfp=NULL;
	//设置新路径outfile
	char outfile[260]={0};
	
	unsigned char *temp=NULL;

	struct FILE_INFO *FileInfo=NULL;
	int totalfile=0;

	//设置标记
	char sign[20];
	unsigned long int time=0;

	int err=fopen_s(&infp,infile,"rb");
	if( 0==err )
	//if(infp=fopen(infile,"rb"))
	{
		//读标记
		fread(&sign,sizeof(char),20,infp);
#if ZIP2FILE_DEBUG
		printf("%s\n",sign);
#endif
		//这里加入 判断。是否是本公司的文件
		//将SIGN与signmark比较 不相等退出
		///啦啦啦啦啦啦啦啦啦~
		if(0!=strcmp(sign,signmark))
		{
			fclose(infp);
			return 0;
		}

		//读时间
		fread(&time,sizeof(unsigned long int),1,infp);
#if ZIP2FILE_DEBUG
		printf("%lu\n",time);
#endif
		//这里加入 判断。是否是最新版本的文件
		//将time与curtime比较  time比curtime小退出
		///啦啦啦啦啦啦啦啦啦~
		if(time <= *curtime)
		{
			fclose(infp);
			return 0;
		}
		else
		{
			//*curtime=time;
		}
		
		//读文件数
		fread(&totalfile,sizeof(int),1,infp);
#if ZIP2FILE_DEBUG
		printf("%d\n",totalfile);
#endif
		FileInfo=(struct FILE_INFO *)calloc(totalfile,sizeof(struct FILE_INFO));
	
		//读长度 文件名
		for(i=0;i<totalfile;i++)
		{
			fread(&FileInfo[i],sizeof(struct FILE_INFO),1,infp);
#if ZIP2FILE_DEBUG
			printf("%s,%lu\n",FileInfo[i].FileName,FileInfo[i].FileLength);
#endif
		}
		//记得要FREE
		temp=(unsigned char *)calloc(1,sizeof(unsigned char));
		for(i=0;i<totalfile;i++)
		{
			temp=(unsigned char *)realloc(temp,sizeof(unsigned char)*FileInfo[i].FileLength);

			fread(temp,sizeof(unsigned char),FileInfo[i].FileLength,infp);
			//创建输出路径
			_snprintf(outfile,260,"%s%s",outDir,FileInfo[i].FileName);
		
			err=fopen_s(&outfp,outfile,"wb");
			if( 0==err )
			//if(outfp=fopen(outfile,"wb"))
			{
				fwrite(temp,sizeof(unsigned char),FileInfo[i].FileLength,outfp);
				fclose(outfp);
				outfp=NULL;
			}
			else
			{	
				if(FileInfo)
				{
					free(FileInfo);
					FileInfo=NULL;
				}
				if(temp)
				{
					free(temp);
					temp=NULL;
				}
#if ZIP2FILE_DEBUG
				printf("%s file con't write \n",outfile);
				getchar();
#endif
				fclose(infp);
				return 0;
			}
		}
		fclose(infp);
		infp=NULL;
	}
	else
	{	
#if ZIP2FILE_DEBUG	
		printf("%s file con't read\n",infile);
		getchar();
#endif
		return 0;
	}

	if(FileInfo)
	{
		free(FileInfo);
		FileInfo=NULL;
	}
	if(temp)
	{
		free(temp);
		temp=NULL;
	}
	//成功 时间再复制
	*curtime=time;
	return 1;
}