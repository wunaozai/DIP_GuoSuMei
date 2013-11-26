/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-10-18 14:30
 * @Author: 林明晓 -- 563804489@qq.com
 * @Description: 该文件定义了所有函数的实现
 * @GitHub: https://github.com/wunaozai/DIP_GuoSuMei
 *
 * @History: 2013年10月18日 14:32:40
 *	    2013年11月25日 23:27:18 增加实验三中的函数
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
    to->img=NULL;
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
	for(i=0;i<img->maxv+1;i++)
	    hist[i]=0;
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		hist[img->img[i*img->sx+j]]++;
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
    int x,y,tmp;
    int sum;
    int ii,jj;
    struct IMG image;
    int * arrsort;
    CopyPPM(img,&image);
    if(strcmp(ptml->name,"")==0)
    {
	printf("The Template Fun is ERROR\n");
	return -1;
    }
    if(img->channel==5)
    {
	printf("%s\n",ptml->name);
	//用于中值滤波
	arrsort=(int *)malloc(sizeof(int)*ptml->x*ptml->y)+1;
	x=ptml->x;
	y=ptml->y;
	for(i=x/2;i<img->sy-(x/2);i++) //该处的x/2是图像的边界
	{
	    for(j=y/2;j<img->sx-(y/2);j++)
	    {
		sum=0;k=0;
		for(ii=i-(x/2);ii<=i+(x/2);ii++)
		{
		    for(jj=j-(y/2);jj<=j+(x/2);jj++)
		    {
			if(ptml->name[0]=='M')
			{
			    arrsort[k]=image.img[img->sx*ii+jj];
			}
			else
			{
			    sum=sum+image.img[img->sx*ii+jj]*ptml->square[k];
			}
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
		    k=ptml->x*ptml->y;
		    for(ii=0;ii<k;ii++)
		    {
			for(jj=k-1;jj>ii;jj--)
			{
			    if(arrsort[jj]<arrsort[jj-1])
			    {
				tmp=arrsort[jj-1];
				arrsort[jj-1]=arrsort[jj];
				arrsort[jj]=tmp;
			    }
			}
		    }
		    img->img[img->sx*i+j]=arrsort[k/2];
		}
		else if(ptml->name[0]=='A')//均值滤波
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

int Ostu(struct IMG * img)
{
    int i,j;
    int * hist=NULL;
    int ip1,ip2,is1,is2;//分别表示 储存前景和背景的灰度总和和像素个数
    double w0,w1;//前景 后景像素比例
    double mean1,mean2;//前景背景平均灰度
    double *g,gmax;//最大类间方差
    int th;//阀值
    if(img->channel==5)
    {
	hist=(int *)malloc(sizeof(int)*img->sx*img->sy);
	g=(double *)malloc(sizeof(double)*img->maxv+1);
	if(hist==NULL)
	{
	    printf("Failed to malloc memory!\n");
	    return ERROR;
	}
	HistPPM(hist,img);
	for(i=0;i<img->maxv+1;i++)
	{
	    ip1=ip2=is1=is2=1;
	    for(j=0;j<i;j++)
	    {
		ip1 += hist[j]*j;
		is1 += hist[j];
	    }
	    mean1=ip1*1.0/is1;
	    w0=is1*1.0/(img->sx*img->sy);
	    //printf("mean1=%lf w0=%lf\n",mean1,w0);
	    for(j=i;j<img->maxv+1;j++)
	    {
		ip2 += hist[j]*j;
		is2 += hist[j];
	    }
	    mean2=ip2*1.0/is2;
	    w1=1-w0;
	    //printf("mean2=%lf w1=%lf\n",mean2,w1);
	    g[i]=w0*w1*(mean1-mean2)*(mean1-mean2);
	}
	//find the max of g[]
	gmax=0;
	for(i=0;i<img->maxv+1;i++)
	{
	    if(g[i]>gmax)
	    {
		gmax=g[i];
		th=i;
	    }
	}
	for(i=0;i<img->maxv+1;i++)
	    hist[i]=g[i];
	WriteHist(hist,"Ostu_ocr",img);
	printf("阀值是%d\n",th);
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		img->img[img->sx*i+j]=img->img[img->sx*i+j]>th?img->maxv:0;
	    }
	}
    }
    else if(img->channel==6)
    {
	printf("This color is not support.\n");
	return -1;
    }
    return 0;
}

int TransformMove(struct IMG * img, int right, int down)
{
    int i,j,k;
    struct IMG image;
    if(img->channel==5)
    {
	CopyPPM(img,&image);
	ClearPPM(img);
	for(i=0;i<image.sy;i++)
	{
	    for(j=0;j<image.sx;j++)
	    {
		//处理边界（可能因为平移后会出界）
		if(j+right<0)
		    continue;
		if(j+right>=img->sx)
		    continue;
		if(i+down<0)
		    continue;
		if(i+down>=img->sy)
		    continue;
		img->img[(i+down)*img->sx+(j+right)]=image.img[i*image.sx+j];
	    }
	}
    }
    else if(img->channel==6)
    {
	CopyPPM(img,&image);
	ClearPPM(img);
	for(i=0;i<image.sy;i++)
	{
	    for(j=0;j<image.sx;j++)
	    {
		//处理边界（可能因为平移后会出界）
		if(j+right<0)
		    continue;
		if(j+right>=img->sx)
		    continue;
		if(i+down<0)
		    continue;
		if(i+down>=img->sy)
		    continue;
		for(k=0;k<3;k++)
		{
		    img->img[((i+down)*img->sx+(j+right))*3+k]=image.img[(i*image.sx+j)*3+k];
		}
	    }
	}
    }
    return 0;
}
int TransformCirle(struct IMG * img, int angle)
{
    int i,j,k;
    struct IMG image;
    // C 语言中的sin cos 库函数是使用弧度的所以要转换
    double ang=angle*1.0*3.1415/180;
    if(img->channel==5)
    {
	CopyPPM(img,&image);
	ResizePPM(img,img->sx*3,img->sy*3,img->maxv,img->channel);
	ClearPPM(img);
	for(i=0;i<image.sy;i++)
	{
	    for(j=0;j<image.sx;j++)
	    {
		//img->img[(i+down)*img->sx+(j+right)]=image.img[i*img->sx+j];
		//Xnew = Xold*cos(a) - Yold*sin(a)
		//Ynew = Xold*sin(a) + Yold*cos(a)
		int jj=j*cos(ang)-i*sin(ang); //这个是x坐标
		int ii=j*sin(ang)+i*cos(ang); //这个是y坐标
		//处理边界（可能因为平移后会出界）
		jj=jj+img->sx/2;
		ii=ii+img->sy/2;
		img->img[ii*img->sx+jj]=image.img[i*image.sx+j];
	    }
	}
	//到这里其实就完成了旋转，但是由于画布一开始定义太多，所以有很多的冗余信息
	//下面就是去掉这些冗余信息
	int x1,x2,y1,y2;//画布中有效信息的四条线
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		if(img->img[i*img->sx+j]!=img->maxv/2)
		    goto aa;
	    }
	}
aa:
	y1=i;
	for(i=img->sy-1;i>=0;i--)
	{
	    for(j=0;j<img->sx;j++)
	    {
		if(img->img[i*img->sx+j]!=img->maxv/2)
		    goto bb;
	    }
	}
bb:
	y2=i;
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		if(img->img[j*img->sx+i]!=img->maxv/2)
		    goto cc;
	    }
	}
cc:
	x1=i;
	for(i=img->sx-1;i>=0;i--)
	{
	    for(j=0;j<img->sy;j++)
	    {
		if(img->img[j*img->sx+i]!=img->maxv/2)
		    goto dd;
	    }
	}
dd:
	x2=i;
	//平移
	TransformMove(img,-x1,-y1);
	int y=y2-y1;
	int x=x2-x1;
	struct IMG image2;
	image2.ch='P';
	image2.channel=5;
	image2.maxv=img->maxv;
	image2.sx=x;
	image2.sy=y;
	image2.img=(unsigned char *) malloc (x*y);
	for(i=0;i<y;i++)
	{
	    for(j=0;j<x;j++)
	    {
		image2.img[i*x+j]=img->img[i*img->sx+j];
	    }
	}
	CopyPPM(&image2,img);
	//下面将处理空穴问题
	for(i=1;i<img->sy-1;i++)
	{
	    for(j=1;j<img->sx-1;j++)
	    {
		if(img->img[i*img->sx+j]==img->maxv/2/*是空穴的地方*/)
		{
		    img->img[i*img->sx+j]=(image2.img[(i-1)*image2.sx+j]+image2.img[(i+1)*image2.sx+j]+image2.img[i*image2.sx+(j-1)]+image2.img[i*image2.sx+(j+1)])/4;
		    //img->img[i*img->sx+j]=255;
		}
	    }
	}
	//CopyPPM(&image2,img);
    }
    else if(img->channel==6)
    {
	CopyPPM(img,&image);
	ResizePPM(img,img->sx*3,img->sy*3,img->maxv,img->channel);
	ClearPPM(img);
	for(i=0;i<image.sy;i++)
	{
	    for(j=0;j<image.sx;j++)
	    {
		int jj=j*cos(ang)-i*sin(ang); //这个是x坐标
		int ii=j*sin(ang)+i*cos(ang); //这个是y坐标
		//处理边界（可能因为平移后会出界）
		jj=jj+img->sx/2;
		ii=ii+img->sy/2;
		for(k=0;k<3;k++)
		{
		    img->img[(ii*img->sx+jj)*3+k]=image.img[(i*image.sx+j)*3+k];
		}
	    }
	}
	//到这里其实就完成了旋转，但是由于画布一开始定义太多，所以有很多的冗余信息
	//下面就是去掉这些冗余信息
	int x1,x2,y1,y2;//画布中有效信息的四条线
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		if(img->img[(i*img->sx+j)*3]!=img->maxv/2)
		    goto ee;
	    }
	}
ee:
	y1=i;
	for(i=img->sy-1;i>=0;i--)
	{
	    for(j=0;j<img->sx;j++)
	    {
		if(img->img[(i*img->sx+j)*3]!=img->maxv/2)
		    goto ff;
	    }
	}
ff:
	y2=i;
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		if(img->img[(j*img->sx+i)*3]!=img->maxv/2)
		    goto gg;
	    }
	}
gg:
	x1=i;
	for(i=img->sx-1;i>=0;i--)
	{
	    for(j=0;j<img->sy;j++)
	    {
		if(img->img[(j*img->sx+i)*3]!=img->maxv/2)
		    goto hh;
	    }
	}
hh:
	x2=i;
	//平移
	TransformMove(img,-x1,-y1);
	int y=y2-y1;
	int x=x2-x1;
	struct IMG image2;
	image2.ch='P';
	image2.channel=6;
	image2.maxv=img->maxv;
	image2.sx=x;
	image2.sy=y;
	image2.img=(unsigned char *) malloc (x*y*3);
	for(i=0;i<y;i++)
	{
	    for(j=0;j<x;j++)
	    {
		for(k=0;k<3;k++)
		{
		    image2.img[(i*x+j)*3+k]=img->img[(i*img->sx+j)*3+k];
		}
	    }
	}
	CopyPPM(&image2,img);
	//下面将处理空穴问题
	for(i=1;i<img->sy-1;i++)
	{
	    for(j=1;j<img->sx-1;j++)
	    {
		if(img->img[(i*img->sx+j)*3]==img->maxv/2/*是空穴的地方*/)
		{
		    for(k=0;k<3;k++)
		    {
			//if(img->img[(i*img->sx+j)*3 + k]==img->maxv/2/*是空穴的地方*/)
			img->img[(i*img->sx+j)*3+k]=(image2.img[((i-1)*image2.sx+j)*3 + k]+image2.img[((i+1)*image2.sx+j)*3 + k]+image2.img[(i*image2.sx+(j-1))*3 + k]+image2.img[(i*image2.sx+(j+1))*3 + k])/4;
			//img->img[(i*img->sx+j)*3+k]=255;
		    }
		}
	    }
	}
    }
    return 0;
}
int ResizePPM(struct IMG * img, int sx, int sy, int vmax, int channel)
{
    int i,j,k;
    FreePPM(img);
    img->ch='P';img->sx=sx;img->sy=sy;img->maxv=vmax;img->channel=channel;
    if(img->channel==5)
    {
	img->img=(unsigned char *)malloc(img->sx*img->sy);
	if(img->img==NULL)
	{
	    printf("Failed to malloc memory!\n");
	    return ERROR;
	}
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		img->img[i*img->sx+j]=img->maxv/2;
	    }
	}
    }
    else if(img->channel==6)
    {
	img->img=(unsigned char *)malloc(img->sx*img->sy*3);
	if(img->img==NULL)
	{
	    printf("Failed to malloc memory!\n");
	    return ERROR;
	}
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		for(k=0;k<3;k++)
		{
		    img->img[(i*img->sx+j)*3+k]=img->maxv/2;
		}
	    }
	}
    }
    return 0;
}

int CreatePPM(struct IMG *img, int sx, int sy, int vmax, int channel)
{
    return 0;
}

int ClearPPM(struct IMG * img)
{
    int i,j,k;
    if(img->channel==5)
    {
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		img->img[i*img->sx+j]=img->maxv/2;
	    }
	}
    }
    else if(img->channel==6)
    {
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		for(k=0;k<3;k++)
		{
		    img->img[(i*img->sx+j)*3+k]=img->maxv/2;
		}
	    }
	}
    }
    return 0;
}

int MultiplePPM(struct IMG * img, double powerx, double powery)
{
    int i,j,k;
    struct IMG image;
    if(img->channel==5)
    {
	CopyPPM(img,&image);
	ResizePPM(img,(img->sx*powerx),(img->sy*powery),img->maxv,img->channel);
	ClearPPM(img);
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		//该方法是直接取整
		img->img[i*img->sx+j]=image.img[ (int)(i/powery)*image.sx + (int)(j/powerx)];

		//下面的方法是使用双线性插值 (有BUG)
		/*
		double ans=0;
		double x2,y2;//用于记录小数部分
		int x1,y1;//用于记录整数部分
		x1=j/powerx;
		y1=i/powery;
		x2=(j*1.0/powerx)-x1;
		y2=(i*1.0/powery)-y1;
		ans+=image.img[(y1*image.sx) + (x1)]*(x2*y2);
		ans+=image.img[(y1*image.sx) + (x1+1)]*((1-x2)*y2);
		ans+=image.img[((y1+1)*image.sx) + (x1)]*(x2*(1-y2));
		ans+=image.img[((y1+1)*image.sx) + (x1+1)]*((1-x2)*(1-y2));
		img->img[i*img->sx+j]=(int)ans;
		*/
	    }
	}
    }
    else if(img->channel==6)
    {
	CopyPPM(img,&image);
	ResizePPM(img,(img->sx*powerx),(img->sy*powery),img->maxv,img->channel);
	ClearPPM(img);
	for(i=0;i<img->sy;i++)
	{
	    for(j=0;j<img->sx;j++)
	    {
		//该方法是直接取整
		for(k=0;k<3;k++)
		{
		    img->img[(i*img->sx+j)*3+k]=image.img[ ((int)(i/powery)*image.sx + (int)(j/powerx))*3 + k];
		}
	    }
	}
    }
    return 0;
}

int LinearShrink (struct IMG * img, int flag)
{
    int i,j,k;
    struct IMG image;
    if(img->channel==5)
    {
	CopyPPM(img,&image);
	ClearPPM(img);
	for(i=0;i<img->sy;i++)
	{
	    for(j=i/2;j<img->sx-i/2;j++)
	    {
		k=(j-i/2)*(img->sx*1.0/(img->sx-i+1));
		//k=0;
		img->img[i*img->sx+j]=image.img[i*img->sx+k];
	    }
	}
    }
    else if(img->channel==6)
    {
	CopyPPM(img,&image);
	ClearPPM(img);
	for(i=0;i<img->sy;i++)
	{
	    for(j=i/2;j<img->sx-i/2;j++)
	    {
		k=(j-i/2)*(img->sx*1.0/(img->sx-i+1));
		int l;
		for(l=0;l<3;l++)
		{
		    img->img[(i*img->sx+j)*3 + l]=image.img[(i*img->sx+k)*3 + l];
		}
	    }
	}
    }
    return 0;
}

int TransformWave(struct IMG * img, int T, int A)
{
    return 0;
}
