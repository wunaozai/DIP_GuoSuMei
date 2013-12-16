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
    freopen("in.txt","r",stdin);
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
	result=H1_6();//Ostu二值化
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
    strcpy(fname,"PepperNoise.ppm");
    ReadPPM(fname,pimg);
    arr=(int *)malloc(sizeof(int)*3*3);//申请一个3*3的模版矩阵
    //中值滤波
    FreePPM(pimg);
    strcpy(fname,"PepperNoise.ppm");
    ReadPPM(fname,pimg);
    strcpy(ch,"Median filtering 中值滤波");
    arr[0]=1; arr[1]=1; arr[2]=1;
    arr[3]=1; arr[4]=1; arr[5]=1;
    arr[6]=1; arr[7]=1; arr[8]=1;
    ptml->square=arr;
    ptml->name=pch;
    ptml->x=3;ptml->y=3;
    ptml->max=9;
    Convolution(pimg,ptml);
    strcpy(fname,"中值滤波模糊效果PepperNoise.ppm");
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
    strcpy(fname,"ocr.ppm");
    ReadPPM(fname,pimg);
    strcpy(ch,"Average filtering 均值滤波，平滑模糊");
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
    strcpy(fname,"除噪后的ocr.ppm");
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

int H1_6()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    strcpy(fname,"lena.ppm");
    //strcpy(fname,"Text.ppm");
    //strcpy(fname,"ocr.ppm");
    ReadPPM(fname,pimg);
    Ostu(pimg);
    strcpy(fname,"Ostu_lena.ppm");
    //strcpy(fname,"Ostu_Text.ppm");
    //strcpy(fname,"Ostu_ocr.ppm");
    WritePPM(fname,pimg);
    return 0;
}

int H2()
{
    int id;
    int result=ERROR;
    Clear();
    E2();
    id=Getchoose();
    if(id==1)
    {
	result=H2_1();//以图像中心为原点旋转α+倍率β的变换
    }
    else if(id==2)
    {
	result=H2_2();//图像纵方向线性缩小
    }
    else if(id==3)
    {
	result=H2_3();//图像横方向波澜平移
    }
    else if(id==4)
    {
	result=H2_4();//几何校正
    }
    return result;
}

int H2_1()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    TransformMove(pimg,50,50);
    strcpy(fname,"平移后的lena.ppm");
    WritePPM(fname,pimg);
    TransformMove(pimg,-50,-50);
    strcpy(fname,"剪切后的lena.ppm");
    WritePPM(fname,pimg);
    //彩色的
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    TransformMove(pimg,50,50);
    strcpy(fname,"平移后的Parrots.ppm");
    WritePPM(fname,pimg);
    TransformMove(pimg,-50,-50);
    strcpy(fname,"剪切后的Parrots.ppm");
    WritePPM(fname,pimg);
    //旋转
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    TransformCirle(pimg,30);//注意是角度
    strcpy(fname,"旋转后的lena.ppm");
    WritePPM(fname,pimg);
    //彩色
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    TransformCirle(pimg,30);//注意是角度
    strcpy(fname,"旋转后的Parrots.ppm");
    WritePPM(fname,pimg);
    //缩放倍数
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    MultiplePPM(pimg,2,1);
    strcpy(fname,"缩放后的lena.ppm");
    WritePPM(fname,pimg);
    //彩色
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    MultiplePPM(pimg,2,1);
    strcpy(fname,"缩放后的Parrots.ppm");
    WritePPM(fname,pimg);
    return 0;
}

int H2_2()
{
    /*图像纵方向线性缩小*/
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    LinearShrink(pimg,1);
    strcpy(fname,"纵方向线性缩小后的lena.ppm");
    WritePPM(fname,pimg);
    //彩色
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    LinearShrink(pimg,1);
    strcpy(fname,"纵方向线性缩小后的Parrots.ppm");
    WritePPM(fname,pimg);
    return 0;
}

int H2_3()//图像横方向波澜平移
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    TransformWave(pimg,5,15);
    strcpy(fname,"横方向波澜平移lena.ppm");
    WritePPM(fname,pimg);
    //彩色
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    TransformWave(pimg,10,5);
    strcpy(fname,"横方向波澜平移Parrots.ppm");
    WritePPM(fname,pimg);
    return 0;
}

int H2_4()
{
    return 0;
}


int H3()
{
    int id;
    int result;
    Clear();
    E3();
    id=Getchoose();
    if(id==1)
    {
	result=H3_1();//给定ChromeKey，进行图像合成
    }
    else if(id==2)
    {
	result=H3_2();//ChromeKey 自动确定，再进行图像合成
    }
    else if(id==3)
    {
	result=H3_3();//空域LSB 内嵌入水印
    }
    else if(id=4)
    {
	result=H3_4();//利用位图的复杂度嵌入数字水印
    }
    return result;
}

int H3_1()//ChromeKey自动确定，进行图像合成
{
    char fname[file_name_len];
    struct IMG fimage,bimage,srcimage;
    struct IMG * fimg=&fimage;
    struct IMG * bimg=&bimage;
    struct IMG * srcimg=&srcimage;
    strcpy(fname,"bing_03.ppm");
    ReadPPM(fname,fimg);
    strcpy(fname,"bing_04.ppm");
    ReadPPM(fname,bimg);
    struct ChromeKey key;
    key.R=key.G=key.B=-1;
    key.flag=6;//表示彩色
    key.D=5;
    CompositePPM(srcimg,fimg,bimg,&key);
    strcpy(fname,"合成后的bing.ppm");
    WritePPM(fname,srcimg);
    return 0;
}

int H3_4()//ChromeKey 自动确定，再进行图像合成
{
    return 0;
}

int H3_2()//空域LSB 内嵌入水印
{
    return 0;
}

int H3_3()//利用位图的复杂度嵌入数字水印
{
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
