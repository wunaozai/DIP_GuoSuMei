/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-10-18 14:30
 * @Author: 林明晓 -- 563804489@qq.com
 * @Description: 该文件定义了所有函数的实现
 * @GitHub: https://github.com/wunaozai/DIP_GuoSuMei
 *
 * @History: 2013年10月18日 14:32:40
 ****************************************/

#include "051033.h"

/********************ReadPPM****************************/
int ReadPPM(char * fname,struct IMG * img)
{
    FILE * fp=NULL;
    fp=fopen(fname,"rb");
    if(fp==NULL)
    {
	printf("Can't open the file -->%s<-- \n",fname);
	return ERROR;
    }
    fscanf(fp,"%c%d%d%d%d%*c",&img->ch,&img->channel,&img->sx,&img->sy,&img->maxv);
#ifdef DEBUG
    printf("FLG=%c\n",img->ch);
    printf("%d %d %d %d\n",img->channel,img->sx,img->sy,img->maxv);
#endif
    if(img->ch!='P' && (img->channel!=5 || img->channel!=6))
    {
	printf("The file -->%s<-- is not PPM format!\n");
	return ERROR;
    }
    if(img->channel==5)
    {
	img->img=(unsigned char *)malloc(img->sx*img->sy);
	if(img->img == NULL)
	{
	    printf("Failed to malloc memory!\n");
	    return ERROR;
	}
	fread(img->img,img->sx*img->sy,1,fp);
    }
    else if(img->channel==6)
    {
	img->img=(unsigned char *)malloc(img->sx*img->sy*3);
	if(img->img == NULL)
	{
	    printf("Failed to malloc memory!\n");
	    return ERROR;
	}
	fread(img->img,img->sx*img->sy*3,1,fp);
    }
    fclose(fp);
    return 0;
}

/***********************WritePPM*******************************/
int WritePPM(char * fname,struct IMG * img)
{
    FILE * fp=NULL;
    fp=fopen(fname,"wb");
    if(fp==NULL)
    {
	printf("The file can't write to -->%s<--\n",fname);
	return ERROR;
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

/********************FreePPM*********************************/
int FreePPM(struct IMG * img)
{
    img->ch=0;
    img->channel=0;
    free(img->img);
    img->maxv=0;
    img->sx=0;
    img->sy=0;
    return 0;
}

/*******************CopyPPM***********************************/
int CopyPPM(struct IMG * from,struct IMG * to)
{
    int i,j,k;
    to->ch=from->ch;
    to->channel=from->channel;
    to->maxv=from->maxv;
    to->sx=from->sx;
    to->sy=from->sy;
    if(from->channel==5)
    {
	to->img=(unsigned char *)malloc(to->sx*to->sy);
	for(i=0;i<from->sx;i++)
	{
	    for(j=0;j<from->sy;j++)
	    {
		to->img[i*from->sy+j]=from->img[i*from->sy+j];
	    }
	}
    }
    else if(from->channel==6)
    {
	to->img=(unsigned char *)malloc(to->sx*to->sy*3);
	for(i=0;i<from->sx;i++)
	{
	    for(j=0;j<from->sy;j++)
	    {
		for(k=0;k<3;k++)
		{
		    to->img[(i*from->sy+j)*3+k]=from->img[(i*from->sy+j)*3+k];
		}
	    }
	}
    }
    return 0;
}


/**********************HistEqualization***********************/
int HistEqualization(struct IMG * img,int * hist)
{
    int i,j,k;
    double * pdhist;//这个分别表示归一化后的直方图,累积直方图
    int * pihist;//映射，取整
    if(img->channel==5)
    {
	if(hist==NULL)
	{
	    hist=(int *)malloc(sizeof(int)*(img->maxv+1));
	}
	else
	{
	    for(i=0;i<img->maxv+1;i++)
	    {
		hist[i]=0;
	    }
	}
	HistPPM(hist,img);
	//下面是处理部分
	pdhist=(double *)malloc(sizeof(double)*(img->maxv+1));
	pihist=(int *)malloc(sizeof(int)*(img->maxv+1));
	k=0;
	for(i=0;i<img->maxv+1;i++)
	{
	    pdhist[i]=hist[i]*1.0/(img->sx*img->sy);//归一化直方图
	}
	for(i=1;i<img->maxv+1;i++)
	{
	    pdhist[i]=pdhist[i]+pdhist[i-1];//累积直方图
	}
	//printf("max==%lf\n",pdhist[img->maxv]);//这个数应该是1.00
	for(i=0;i<img->maxv+1;i++)
	{
	    pihist[i]=(int)(pdhist[i]*(img->maxv+1)-1);//映射，取整
	    pihist[i]=(int)(pdhist[i]*(img->maxv));//映射，取整
	}
	memset(hist,0,sizeof(hist));
	//处理图片
	for(i=0;i<img->maxv+1;i++)
	{
	    hist[i]=0;
	}
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		int t=pihist[img->img[(img->sx)*i+j]];
		img->img[(img->sx)*i+j]=t;
		hist[t]++;
	    }
	}
    }
    else if(img->channel==6)
    {
	printf("Color image is not support!\n");
    }
    free(pdhist);
    free(pihist);
    return 0;
}

/**********************HistPPM*************************/
int HistPPM(int * hist,struct IMG * img)
{
    int i,j,k;
    if(img->channel==5)//黑白处理
    {
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
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
    return 0;
}

/***********************WriteHist***************************/
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
	    printf("The file can't write to -->%s<--\n",fname);
	    return ERROR;
	}
	for(i=0;i<img->maxv;i++)
	{
	    fprintf(fp,"%d,%d\n",i,hist[i]);
	}
	printf("-->%s<-- Write complete!\n",fname);
	fclose(fp);
    }
    else if(img->channel==6)
    {
	strcpy(pname,fname);
	strcat(pname,"_red.csv");
	printf("Write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("The file can't write to -->%s<--\n",fname);
	    return ERROR;
	}
	for(i=0;i<img->maxv+1;i++)
	{
	    fprintf(fp,"%d,%d\n",i,hist[i]);
	}
	fclose(fp);
	
	strcpy(pname,fname);
	strcat(pname,"_green.csv");
	printf("Write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("The file can't write to -->%s<--\n",fname);
	    return ERROR;
	}
	for(i=0;i<img->maxv+1;i++)
	{
	    fprintf(fp,"%d,%d\n",i,hist[(img->maxv+1)+i]);
	}
	fclose(fp);

	strcpy(pname,fname);
	strcat(pname,"_blue.csv");
	printf("Write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("The file can't write to -->%s<--\n",fname);
	    return ERROR;
	}
	for(i=0;i<img->maxv+1;i++)
	{
	    fprintf(fp,"%d,%d\n",i,hist[(img->maxv+1)*2+i]);
	}
	fclose(fp);

	strcpy(pname,fname);
	strcat(pname,".csv");
	printf("Write to %s\n",pname);
	fp=fopen(pname,"wb");
	if(fp==NULL)
	{
	    printf("The file can't write to -->%s<--\n",fname);
	    return ERROR;
	}
	fprintf(fp,"Hist,Red,Green,Blue\n");
	for(i=0;i<img->maxv+1;i++)
	{
	    fprintf(fp,"%d,%d,%d,%d\n",i,hist[i],hist[(img->maxv+1)+i],hist[(img->maxv+1)*2+i]);
	}
	fclose(fp);
	
	printf("Write complete!\n");
    }
    return 0;
}

int Convolution(struct IMG * img,struct Template * ptml)
{
    //注意该函数对偶数的模版可能有错误
    int i,j,k;
    int x,y;
    int sum;
    int ii,jj;
    struct IMG image;
    CopyPPM(img,&image);
    if(strcmp(ptml->name,"")==0)
    {
	printf("The Template Fun is ERROR\n");
	return -1;
    }
    if(img->channel==5)
    {
	printf("%s\n",ptml->name);
	x=ptml->x;
	y=ptml->y;
	for(i=x/2;i<img->sx-(x/2);i++) //该处的x/2是图像的边界
	{
	    for(j=y/2;j<img->sy-(y/2);j++)
	    {
		sum=0;k=0;
		for(ii=i-(x/2);ii<=i+(x/2);ii++)
		{
		    for(jj=j-(y/2);jj<=j+(x/2);jj++)
		    {
			sum=sum+image.img[img->sx*ii+jj]*ptml->square[k];
			k++;
		    }
		}
		if(ptml->name[0]=='H')//这个表示是锐化
		{
		    if(sum<0)
			sum=0;
		    if(sum>255)
			sum=255;
		    img->img[img->sx*i+j]=sum;
		}
		else if(ptml->name[0]=='E')//表示求边缘
		{
		    if(sum<0)
			sum=(-sum)/ptml->max;
		    else
			sum=sum/ptml->max;
		    img->img[img->sx*i+j]=sum;
		}
		else if(ptml->name[0]=='M')
		{
		    img->img[img->sx*i+j]=sum/ptml->max;
		}
	    }
	}
    }
    else if(img->channel==6)
    {
	printf("The imges is error!\n");
	return -1;
    }
    return 0;
}

