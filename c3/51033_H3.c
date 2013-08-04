/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-09-14 12:03
 * @Author: 林明晓 -- 563804489@qq.com
 * @Description: 
 *
 * @History: 
 *	    2013-09-18 00:42:40 +H3
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DEBUG 1
#define H3 1 /*表示是实验3*/

//The const number
#define file_name_len 32

/*
 * 图片的格式
 * */
struct IMG
{
    char ch;
    int channel;
    int sx,sy;
    int maxv;
    unsigned char * img;
};


/*
 * 读取PPM图片，并返回到IMG中
 * */
int ReadPPM(char * fname,struct IMG * img)
{
    FILE * fp=NULL;
    fp=fopen(fname,"rb");
    if(fp==NULL)
    {
	printf("打开文件失败!\n");
	exit(-1);
    }
    fscanf(fp,"%c%d%d%d%d%*c",&img->ch,&img->channel,&img->sx,&img->sy,&img->maxv);
#ifdef DEBUG
    printf("FLG=%c\n",img->ch);
    printf("%d %d %d %d\n",img->channel,img->sx,img->sy,img->maxv);
#endif
    if(img->ch!='P' && (img->channel!=5 || img->channel!=6))
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

/*
 * 将IMG中的PPM图片文件写入到文件系统中
 * */
int WritePPM(char * fname,struct IMG * img)
{
    FILE * fp=NULL;
    fp=fopen(fname,"wb");
    if(fp==NULL)
    {
	printf("写入文件失败!\n");
	exit(-1);
    }
    fprintf(fp,"%c%d\n%d\n%d\n%d\n",img->ch,img->channel,img->sx,img->sy,img->maxv);
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

/*
 * 进行负片的计算 直接将原图片转换成负片
 * */
int NegativePPM(struct IMG * img)
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

/*
 * 计算PPM图片的灰度直方图，并保存到hist数组中
 * */
int HistPPM(int * hist,struct IMG * img)
{
    int i,j,k;
    //-----操作---------
    if(img->channel==5)//黑白处理
    {
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
#ifdef DEBUG
		//printf("%d ",img->img[i*img->sy+j]);
#endif
		hist[img->img[i*img->sy+j]]++;
	    }
	}
    }
    else if(img->channel==6)
    {
#ifdef DEBUG
	int color[3]={0};
	int cnt=0;
#endif
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		for(k=0;k<3;k++)
		{
		    hist[k*(img->maxv+1)/*分成三段*/ + img->img[i*img->sy*3+j*3+k] ]++;
		    //img->img[i*img->sy*3+j*3+k]=255-img->img[i*img->sy*3+j*3+k];
#ifdef DEBUG
		    color[ (img->img[i*img->sy*3+j*3+k]+k*(img->maxv+1) )/256]++;
		    cnt++;
#endif
		}
	    }
	}
#ifdef DEBUG
	printf("r=%d,g=%d,b=%d\n",color[0],color[1],color[2]);//验证rgb是否正确
	printf("cnt=%d\n",cnt);
#endif
    }
    //-----结束---------
    return 0;
}


/*
 * 将PPM图片的灰度直方图写到文件中
 * */
int WriteHist(int * hist,char * fname,struct IMG * img)
{
    int i;
    FILE * fp;
    char  pname[file_name_len];
    if(img->channel==5)
    {
	strcpy(pname,fname);
	strcat(pname,".csv");
	printf("write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("写入文件失败!\n");
	    exit(-1);
	}
	for(i=0;i<img->maxv;i++)
	{
	    fprintf(fp,"%d,%d\n",i,hist[i]);
	}
	printf("write complete!\n");
	fclose(fp);
    }
    else if(img->channel==6)
    {
	strcpy(pname,fname);
	strcat(pname,"_red.csv");
	printf("write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("写入文件失败!\n");
	    exit(-1);
	}
	for(i=0;i<img->maxv+1;i++)
	{
	    fprintf(fp,"%d,%d\n",i,hist[i]);
	}
	fclose(fp);
	
	strcpy(pname,fname);
	strcat(pname,"_green.csv");
	printf("write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("写入文件失败!\n");
	    exit(-1);
	}
	for(i=0;i<img->maxv+1;i++)
	{
	    fprintf(fp,"%d,%d\n",i,hist[(img->maxv+1)+i]);
	}
	fclose(fp);

	strcpy(pname,fname);
	strcat(pname,"_blue.csv");
	printf("write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("写入文件失败!\n");
	    exit(-1);
	}
	for(i=0;i<img->maxv+1;i++)
	{
	    fprintf(fp,"%d,%d\n",i,hist[(img->maxv+1)*2+i]);
	}
	fclose(fp);

	strcpy(pname,fname);
	strcat(pname,".csv");
	printf("write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("写入文件失败!\n");
	    exit(-1);
	}
	fprintf(fp,"Hist,Red,Green,Blue\n");
	for(i=0;i<img->maxv+1;i++)
	{
	    fprintf(fp,"%d,%d,%d,%d\n",i,hist[i],hist[(img->maxv+1)+i],hist[(img->maxv+1)*2+i]);
	}
	fclose(fp);
	
	printf("write complete!\n");
    }
    return 0;
}

/*
 * 计算一个PPM6彩色图片的分量图
 * return -1 表示图片为黑白图片不能计算分量图
 * 保存到fname中
 * */
int ComponentPPM(char * fname,struct IMG * img)
{
    struct IMG cimg[3];
    int i,j,k;
    if(img->channel==5)
    {
	return -1;
    }
    //3个分量
    for(i=0;i<3;i++)
    {
	cimg[i].ch='P';
	cimg[i].channel=5;
	cimg[i].maxv=img->maxv;
	cimg[i].sx=img->sx;
	cimg[i].sy=img->sy;
	//Create Image...
	cimg[i].img=(unsigned char *)malloc(img->sx*img->sy);
	if(cimg[i].img==NULL)
	{
	    printf("分配内存失败!\n");
	    exit(-1);
	}
    }

    //printf("This is the test code!\n");
    for(i=0;i<img->sx;i++)
    {
	for(j=0;j<img->sy;j++)
	{
	    for(k=0;k<3;k++)
	    {
		cimg[k].img[i*img->sy+j]=img->img[(i*img->sy+j)*3+k];
	    }
	}
    }

    //Write img
    char pname[file_name_len];

    strcpy(pname,fname);
    strcat(pname,"_red.ppm");
    printf("write to %s\n",pname);
    WritePPM(pname,&cimg[0]);

    strcpy(pname,fname);
    strcat(pname,"_green.ppm");
    printf("write to %s\n",pname);
    WritePPM(pname,&cimg[1]);

    strcpy(pname,fname);
    strcat(pname,"_blue.ppm");
    printf("write to %s\n",pname);
    WritePPM(pname,&cimg[2]);

    printf("write complete!\n");

    return 0;
}

/*
 * 映射函数数组生成
 * */
int Square_MapPPM(int * arrmap,int maxv)
{
    int i;
    for(i=0;i<maxv+1;i++)
    {
	// x=x^2
	arrmap[i]=255*((i*1.0)/255)*((i*1.0)/255);
#ifdef DEBUG
	//printf("arrmap[%d]=%d\n",i,arrmap[i]);
#endif
    }
    return 0;
}

/*
 * 映射函数数组生成
 * */
int Sqrt_MapPPM(int * arrmap,int maxv)
{
    int i;
    for(i=0;i<maxv+1;i++)
    {
	// x=x^(1/2)
	arrmap[i]=255*(sqrt((i*1.0)/255));
#ifdef DEBUG
	//printf("arrmap[%d]=%d\n",i,arrmap[i]);
#endif
    }
    return 0;
}

/*
 * 映射函数，输入一幅PPM图片和一个转换数组进行映射
 * */
int MapPPM(int * arrmap,struct IMG * img)
{
    int i,j,k;
    if(img->channel==5)
    {
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		img->img[i*img->sy+j]=arrmap[img->img[i*img->sy+j]];
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
		    img->img[(i*img->sy+j)*3+k]=arrmap[img->img[(i*img->sy+j)*3+k]];
		}
	    }
	}
    }
    return 0;
}

int main()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
#ifdef h2
    //lena
    strcpy(fname,"lena.ppm");
    readppm(fname,pimg);
    negativeppm(pimg);
    strcpy(fname,"lena_m.ppm");
    writeppm(fname,pimg);
    //parrots 
    strcpy(fname,"parrots.ppm");
    readppm(fname,pimg);
    negativeppm(pimg);
    strcpy(fname,"parrots_m.ppm");
    writeppm(fname,pimg);
#endif

#ifdef H3
    // hint
    // lena
    int * hist=NULL;
    int * array=NULL;
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    hist=(int *)calloc(sizeof(int),(pimg->maxv+1)*((pimg->channel-5)*2+1));//hist[] color:3*pimg->maxv
    if(hist==NULL)
    {
	printf("分配内存失败!\n");
	exit(-1);
    }
    printf("calloc=%d\n",sizeof(int)*(pimg->maxv+1)*((pimg->channel-5)*2+1));
    HistPPM(hist,pimg);
    strcpy(fname,"lena");
    WriteHist(hist,fname,pimg);
    
    //Parrots
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    hist=(int *)calloc(sizeof(int),(pimg->maxv+1)*((pimg->channel-5)*2+1));//Hist[] color:3*pimg->maxv 
    if(hist==NULL)
    {
	printf("分配内存失败!\n");
	exit(-1);
    }
    HistPPM(hist,pimg);
    strcpy(fname,"Parrots");
    WriteHist(hist,fname,pimg);

    //计算分量图
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    strcpy(fname,"Parrots");
    ComponentPPM(fname,pimg);
    free(hist);

    //计算映射函数
    //lena
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    array=(int *)calloc(sizeof(int),(pimg->maxv+1)*((pimg->channel-5)*2+1));//Hist[] color:3*pimg->maxv 
    Square_MapPPM(array,pimg->maxv);
    MapPPM(array,pimg);
    strcpy(fname,"lena_square.ppm");
    WritePPM(fname,pimg);
    Sqrt_MapPPM(array,pimg->maxv);
    MapPPM(array,pimg);
    strcpy(fname,"lena_sqrt.ppm");
    WritePPM(fname,pimg);
    //Parrots
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    array=(int *)calloc(sizeof(int),(pimg->maxv+1)*((pimg->channel-5)*2+1));//Hist[] color:3*pimg->maxv 
    Square_MapPPM(array,pimg->maxv);
    MapPPM(array,pimg);
    strcpy(fname,"Parrots_square.ppm");
    WritePPM(fname,pimg);
    Sqrt_MapPPM(array,pimg->maxv);
    MapPPM(array,pimg);
    strcpy(fname,"Parrots_sqrt.ppm");
    WritePPM(fname,pimg);

#endif

    return 0;
}


/*注意事项*/
/*
 * 1.图片的maxv是255 有些要注意+1
 * */
