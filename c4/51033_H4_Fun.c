/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-09-18 22:18
 * @Author: 林明晓 -- 563804489@qq.com
 * @Description: 
 *	把原来的程序分解成多个文件，便于管理
 * @History: 
 *	    2013年9月19日 01:33:31 + BitPlane + unBitPlane == 位运算好难啊。
 ****************************************/

#include "51033_H4.h"

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

int BitPlane(struct IMG * img,char * str,int bit)
{
    int i,j,k;
    int cnt;
    int temp;
    printf("准备加密的信息:%s\n",str);
    if(img->channel==5)
    {
	cnt=0;
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		/* //1的情况
		//img->img[i*img->sy+j]=(img->img[i*img->sy+j] | 1 )-1;//末尾置零
		temp=img->img[i*img->sy+j];
		temp=(temp|1)-1;
		//str[cnt/8]>>(cnt%8) & 1; //取最后一位
		temp=temp|(str[cnt/8]>>(cnt%8)&1);
		img->img[i*img->sy+j]=temp;
#ifdef DEBUG
		printf("---%d---%d\n",temp%2,cnt);
#endif
		*/
		temp=img->img[i*img->sy+j];
		temp=((temp|(1<<bit)))-(1<<bit);
		//str[cnt/8]>>(cnt%8) & 1; //取最后一位
		temp=temp| ( (str[cnt/8]>>(cnt%8)&1) <<bit);
		img->img[i*img->sy+j]=temp;
#ifdef DEBUG
		printf("---%d---%d\n",temp&(1<<bit),cnt);
#endif
		if(str[(cnt/8)-1]==0)
		{
		    //printf("遇到\\0结束符号\n");
		    return 0;
		}
		//第[cnt/8]个字符 
		//第 cnt%8 位
		cnt++;
	    }
	}
    }
    else if(img->channel==6)
    {
	cnt=0;
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		for(k=0;k<3;k++)
		{
		    temp=img->img[(i*img->sy+j)*3+k];
		    temp=((temp|(1<<bit)))-(1<<bit);
		    temp=temp| ( (str[cnt/8]>>(cnt%8)&1) <<bit);
		    img->img[(i*img->sy+j)*3+k]=temp;
#ifdef DEBUG
		    printf("---%d---%d\n",temp&(1<<bit),cnt);
#endif
		    if(str[(cnt/8)-1]==0)
		    {
			//printf("遇到\\0结束符号\n");
			return 0;
		    }
		    cnt++;
		}
	    }
	}
    }
    return 0;
}

int unBitPlane(struct IMG * img,char * str,int bit)
{
    int i,j,k;
    int temp;
    int cnt;
    memset(str,0,sizeof(str_info_len));//注意要初始化
    if(img->channel==5)
    {
	cnt=0;
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		/* //1位的时候
		//取最后一位
		temp=img->img[i*img->sy+j] & 1;//取最后一位
#ifdef DEBUG
		printf("---%d---\n",temp%2);
#endif
		str[cnt/8]= str[cnt/8] | temp<<(cnt%8);
		*/
		//取最后bit位
		temp=img->img[i*img->sy+j] & (1<<bit);//取最后bit位 
#ifdef DEBUG
		printf("---%d---\n",temp);
#endif
		str[cnt/8]= str[cnt/8] | (temp>>bit)<<(cnt%8);

		if(cnt%8==7)//在最后一个bit进行判断
		{
		    if(str[cnt/8]==0)
		    {
			//printf("遇到\\0结束符号\n");
			return 0;
		    }
		}
		cnt++;
	    }
	}
    }
    else if(img->channel==6)
    {
	cnt=0;
	for(i=0;i<img->sx;i++)
	{
	    for(j=0;j<img->sy;j++)
	    {
		for(k=0;k<3;k++)
		{
		    temp=img->img[(i*img->sy+j)*3+k] & (1<<bit);//取最后bit位 
#ifdef DEBUG
		    printf("---%d---\n",temp);
#endif
		    str[cnt/8]= str[cnt/8] | (temp>>bit)<<(cnt%8);
		    if(cnt%8==7)//在最后一个bit进行判断
		    {
			if(str[cnt/8]==0)
			{
			    return 0;
			}
		    }
		    cnt++;
		}
	    }
	}
    }
    return 0;
}
