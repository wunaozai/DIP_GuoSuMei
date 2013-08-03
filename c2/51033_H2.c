/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-09-14 12:03
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
    FILE * fp=NULL;
    char ch;
    fp=fopen(fname,"rb");
    if(fp==NULL)
    {
	printf("打开文件失败!\n");
	exit(-1);
    }
    fscanf(fp,"%c%d%d%d%d%*c",&ch,&img->channel,&img->sx,&img->sy,&img->maxv);
    printf("FLG=%c\n",ch);
    printf("%d %d %d %d\n",img->channel,img->sx,img->sy,img->maxv);
    if(ch!='P' && (img->channel!=5 || img->channel!=6))
    {
	printf("不是PPM格式!\n");
	exit(-1);
    }
    if(img->channel==5)
    {
	img->img=(unsigned char *)malloc(img->sx*img->sy);
	if(img->img == NULL)
	{
	    printf("分配内存失败!\n");
	    exit(-1);
	}
	fread(img->img,img->sx*img->sy,1,fp);
    }
    else if(img->channel==6)
    {
	img->img=(unsigned char *)malloc(img->sx*img->sy*3);
	if(img->img == NULL)
	{
	    printf("分配内存失败!\n");
	    exit(-1);
	}
	fread(img->img,img->sx*img->sy*3,1,fp);
    }
    fclose(fp);
    return 0;
}

int WritePPM(char * fname,struct IMG * img)
{
    FILE * fp=NULL;
    fp=fopen(fname,"wb");
    if(fp==NULL)
    {
	printf("写入文件失败!\n");
	exit(-1);
    }
    fprintf(fp,"P%d\n%d\n%d\n%d\n",img->channel,img->sx,img->sy,img->maxv);
    if(img->channel==5)
    {
	fwrite(img->img,img->sx*img->sy,1,fp);
    }
    else if(img->channel==6)
    {
	fwrite(img->img,img->sx*img->sy*3,1,fp);
    }
    fclose(fp);
    return 0;
}

int OpertorPPM(struct IMG * img)
{
    int i,j,k;
    //-----操作---------
    if(img->channel==5)//黑白处理
    {
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		img->img[i*img->sy+j]=255-img->img[i*img->sy+j];
	    }
	}
    }
    else if(img->channel==6)
    {
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		for(k=0;k<3;k++)
		{
		    img->img[i*img->sy*3+j*3+k]=255-img->img[i*img->sy*3+j*3+k];
		}
	    }
	}
    }
    //-----结束---------
    return 0;
}
int main()
{
    char fname[32];
    struct IMG image;
    struct IMG * pimg=&image;
    //Lena
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    OpertorPPM(pimg);
    strcpy(fname,"lena_m.ppm");
    WritePPM(fname,pimg);
    //Parrots 
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    OpertorPPM(pimg);
    strcpy(fname,"Parrots_m.ppm");
    WritePPM(fname,pimg);
    return 0;
}
