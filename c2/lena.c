/****************************************
* Copyright (C) 2013-2013 All rights reserved.
* @Version: 1.0
* @Created: 2013-08-04 02:03
* @Author: 林明晓 -- 563804489@qq.com
* @Description: 
*
* @History: 
****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct IMG
{
    int channel;
    int sx,sy;
    int maxv;
    unsigned char * img;
};

int ReadPPM(char * fname,struct IMG * img)
{

    return 0;
}
int WritePPM(char * fname,struct IMG * img)
{
    return 0;
}

int main()
{
    FILE * fp=NULL;
    char ch;
    int i,j;
    struct IMG image;
    struct IMG * pimg=&image;
    fp=fopen("lena.ppm","rb");
    if(fp==NULL)
    {
	printf("打开文件失败!\n");
	exit(-1);
    }
    fscanf(fp,"%c%d%d%d%d%*c",&ch,&pimg->channel,&pimg->sx,&pimg->sy,&pimg->maxv);
    printf("FLG=%c\n",ch);
    printf("%d %d %d %d\n",pimg->channel,pimg->sx,pimg->sy,pimg->maxv);
    if(ch!='P' && (pimg->channel!=5 || pimg->channel!=6))
    {
	printf("不是PPM格式!\n");
	exit(-1);
    }
    pimg->img=(unsigned char *)malloc(pimg->sx*pimg->sy);
    if(pimg->img == NULL)
    {
	printf("分配内存失败!\n");
	exit(-1);
    }
    fread(pimg->img,pimg->sx*pimg->sy,1,fp);
    fclose(fp);
    //-----操作---------
    for(i=0;i<pimg->sx;i++)
    {
	for(j=0;j<pimg->sy;j++)
	{
	    pimg->img[i*pimg->sy+j]=255-pimg->img[i*pimg->sy+j];
	}
    }
    //-----结束---------
    fp=fopen("lena_m.ppm","wb");
    if(fp==NULL)
    {
	printf("写入文件失败!\n");
	exit(-1);
    }
    fprintf(fp,"P%d\n%d\n%d\n%d\n",pimg->channel,pimg->sx,pimg->sy,pimg->maxv);
    fwrite(pimg->img,pimg->sx*pimg->sy,1,fp);
    fclose(fp);
    return 0;
}
