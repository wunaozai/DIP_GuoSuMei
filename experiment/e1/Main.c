/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-10-18 16:09
 * @Author: 林明晓 -- 563804489@qq.com
 * @Description: 主程序，用于实现实验的逻辑结构
 * @GitHub: https://github.com/wunaozai/DIP_GuoSuMei
 *
 * @History: 
 ****************************************/

#include "051033.h"

void Clear();
int main()
{
    int choose=0;
    while(1)
    {
	Clear();
	display();
	choose=Getchoose();
	if(choose==0)
	{
	    break;
	}
	switch(choose)
	{
	    case 1:
		H1();
		break;
	    case 2:
		H2();
		break;
	    case 3:
		H3();
		break;
	    case 4:
		H4();
		break;
	    default:
		break;
	}
	getchar();
    }
    return 0;
}

int H1()
{
    int id;
    int result=ERROR;
    Clear();
    E1();
    id=Getchoose();
    if(id==1)
    {
	result=H1_1();//做直方图均衡化
    }
    else if(id==2)
    {
	result=H1_2();//做边缘检测
    }
    else if(id==3)
    {
	result=H1_3();//中值滤波
    }
    else if(id==4)
    {
	result=H1_4();//模糊图像
    }
    else if(id==5)
    {
	result=H1_5();//图像瑞化
    }
    else if(id==6)
    {

    }
    return result;
}

int H1_1()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    //lena
    strcpy(fname,"lena.ppm");
    //strcpy(fname,"Text.ppm");
    ReadPPM(fname,pimg);
    int * hist;
    hist=(int *)malloc(sizeof(int)*(pimg->maxv+1)*((pimg->channel-5)*2+1));
    HistPPM(hist,pimg);
    strcpy(fname,"lena_hist");
    //strcpy(fname,"Text_hist");
    WriteHist(hist,fname,pimg);
    //直方图完成
    //均衡化处理
    HistEqualization(pimg,hist);
    strcpy(fname,"lena_hist_equalization.ppm");
    //strcpy(fname,"Text_hist_equalization.ppm");
    WritePPM(fname,pimg);
    strcpy(fname,"lena_hist_equalization");
    //strcpy(fname,"Text_hist_equalization");
    WriteHist(hist,fname,pimg);
    free(hist);
    return 0;
}

int H1_2()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    struct Template tml;//定义模版
    struct Template * ptml=&tml;
    int * arr=NULL;
    char ch[1024];
    char * pch=ch;
    //lena-------------------------
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    arr=(int *)malloc(sizeof(int)*3*3);//申请一个3*3的模版矩阵
    //边缘检测
    FreePPM(pimg);
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    strcpy(ch,"Edge Detection边缘检测Sobel算子");
    /* -1  0  1
     * -2  0  2
     * -1  0  1
     * */
    arr[0]=-1; arr[1]=0; arr[2]=1;
    arr[3]=-2; arr[4]=0; arr[5]=2;
    arr[6]=-1; arr[7]=0; arr[8]=1;
    ptml->square=arr;
    ptml->name=pch;
    ptml->x=3;ptml->y=3;
    ptml->max=4;
    Convolution(pimg,ptml);
    strcpy(fname,"边缘检测后的lena.ppm");
    WritePPM(fname,pimg);
    free(arr);
    //lena--------------------------

}
int H1_3()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    struct Template tml;//定义模版
    struct Template * ptml=&tml;
    int * arr=NULL;
    char ch[1024];
    char * pch=ch;
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    arr=(int *)malloc(sizeof(int)*3*3);//申请一个3*3的模版矩阵
    //中值滤波
    FreePPM(pimg);
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    strcpy(ch,"Median filtering 中值滤波，平滑过度");
    arr[0]=1; arr[1]=1; arr[2]=1;
    arr[3]=1; arr[4]=1; arr[5]=1;
    arr[6]=1; arr[7]=1; arr[8]=1;
    ptml->square=arr;
    ptml->name=pch;
    ptml->x=3;ptml->y=3;
    ptml->max=9;
    Convolution(pimg,ptml);
    strcpy(fname,"中值滤波模糊效果lena.ppm");
    WritePPM(fname,pimg);
    free(arr);
    return 0;
}
int H1_4()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    struct Template tml;//定义模版
    struct Template * ptml=&tml;
    int * arr=NULL;
    char ch[1024];
    char * pch=ch;
    arr=(int *)malloc(sizeof(int)*3*3);//申请一个3*3的模版矩阵
    //模糊图像
    strcpy(fname,"Text.ppm");
    ReadPPM(fname,pimg);
    strcpy(ch,"Median filtering 中值滤波，平滑模糊");
    arr[0]=1; arr[1]=1; arr[2]=1;
    arr[3]=1; arr[4]=1; arr[5]=1;
    arr[6]=1; arr[7]=1; arr[8]=1;
    ptml->square=arr;
    ptml->name=pch;
    ptml->x=3;ptml->y=3;
    ptml->max=9;
    Convolution(pimg,ptml);
    //锐化图像
    strcpy(ch,"Harpening 锐化模版导入成功");
    arr[0]=-1; arr[1]=-1; arr[2]=-1;
    arr[3]=-1; arr[4]=9; arr[5]=-1;
    arr[6]=-1; arr[7]=-1; arr[8]=-1;
    ptml->square=arr;
    ptml->name=pch;
    ptml->x=3;
    ptml->y=3;
    ptml->max=1;
    Convolution(pimg,ptml);
    
    //模糊图像
    strcpy(fname,"Text.ppm");
    ReadPPM(fname,pimg);
    strcpy(ch,"Median filtering 中值滤波，平滑模糊");
    arr[0]=1; arr[1]=1; arr[2]=1;
    arr[3]=1; arr[4]=1; arr[5]=1;
    arr[6]=1; arr[7]=1; arr[8]=1;
    ptml->square=arr;
    ptml->name=pch;
    ptml->x=3;ptml->y=3;
    ptml->max=9;
    Convolution(pimg,ptml);
    //锐化图像
    strcpy(ch,"Harpening 锐化模版导入成功");
    arr[0]=-1; arr[1]=-1; arr[2]=-1;
    arr[3]=-1; arr[4]=9; arr[5]=-1;
    arr[6]=-1; arr[7]=-1; arr[8]=-1;
    ptml->square=arr;
    ptml->name=pch;
    ptml->x=3;
    ptml->y=3;
    ptml->max=1;
    Convolution(pimg,ptml);
    strcpy(fname,"除噪后的Text.ppm");
    WritePPM(fname,pimg);
    free(arr);
    return 0;
}
int H1_5()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    struct Template tml;//定义模版
    struct Template * ptml=&tml;
    int * arr=NULL;
    char ch[1024];
    char * pch=ch;
    //lena-------------------------
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    arr=(int *)malloc(sizeof(int)*3*3);//申请一个3*3的模版矩阵
    //锐化模版
    strcpy(ch,"Harpening 锐化模版导入成功");
    /*  0  -1  0
     * -1  5  -1
     *  0  -1  0
     * */
    //arr[0][0]=0;arr[0][1]=-1;arr[0][2]=0;
    //arr[1][0]=-1;arr[1][1]=5;arr[1][2]=-1;
    //arr[2][0]=0;arr[2][1]=-1;arr[2][2]=0;
    arr[0]=0; arr[1]=-1; arr[2]=0;
    arr[3]=-1; arr[4]=5; arr[5]=-1;
    arr[6]=0; arr[7]=-1; arr[8]=0;
    ptml->square=arr;
    ptml->name=pch;
    ptml->x=3;
    ptml->y=3;
    ptml->max=1;
    Convolution(pimg,ptml);
    strcpy(fname,"锐化后的lena.ppm");
    WritePPM(fname,pimg);
    WritePPM(fname,pimg);
    return 0;
}

int H2()
{
    int id;
    Clear();
    E2();
    id=Getchoose();
    return 0;
}

int H3()
{
    int id;
    Clear();
    E3();
    id=Getchoose();
    return 0;
}

int H4()
{
    int id;
    Clear();
    E4();
    id=Getchoose();
    return 0;
}

void Clear()
{
    if(SYSTEM==1)
    {
	system("clear");
    }
    else
    {
	system("cls");
    }
    return ;
}

int Getchoose()
{
    int id;
    printf("Please input your choice:");
    scanf("%d%*c",&id);
    return id;
}
