/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-09-18 22:18
 * @Author: 林明晓 -- 563804489@qq.com
 * @Description: 
 *	把原来的程序分解成多个文件，便于管理
 *	2013年10月15日 00:02:26 增加H4作业函数
 * @History: 
 ****************************************/

#include "51033_H4.h"
//  51033_H4_Fun.c
//#define DEBUG 1

int H2();
int H3();
int H3_1();
int H3_2();
int H4();

int main()
{
    //H2();
    //H3();
    //H3_1();
    //H3_2();
    //H3_3();
    H4();
    return 0;
}

//函数定义
int H4()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    //lena
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    int * hist;
    hist=(int *)malloc(sizeof(int)*(pimg->maxv+1)*((pimg->channel-5)*2+1));
    HistPPM(hist,pimg);
    strcpy(fname,"lena_hist");
    WriteHist(hist,fname,pimg);
    //直方图完成
    //均衡化处理
    HistEqualization(pimg,hist);
    strcpy(fname,"lena_hist_equalization.ppm");
    WritePPM(fname,pimg);
    strcpy(fname,"lena_hist_equalization");
    WriteHist(hist,fname,pimg);

    free(hist);
    return 0;
}

int H3_3()
{
    /*实现图片的复制，因为以前对图片的操作都是在原图片进行的操作。所以可以用这个函数进行图片的复制*/
    char fname[file_name_len];
    struct IMG image;
    struct IMG image2;
    struct IMG * pimg=&image;
    struct IMG * pimg2=&image2;
    //lena
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    CopyPPM(pimg,pimg2);
    WritePPM("lena_copy.ppm",pimg2);
    //Parrots
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    CopyPPM(pimg,pimg2);
    WritePPM("Parrots_copy.ppm",pimg2);
    return 0;
}

/*H3_2*/
int H3_2()
{
    //实现二值化
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    //lena
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    BinaryZationPPM(pimg,2);
    WritePPM("lena_binaryzation.ppm",pimg);
    //Parrots
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    BinaryZationPPM(pimg,4);
    WritePPM("Parrots_binaryzation.ppm",pimg);
    return 0;
}
/*提高H3_1*/
int H3_1()
{
    //实现BitPlane保存信息
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    char str[str_info_len];
    int bit=7;//第1比特用来当作水印信息,因为低频率人眼不敏感
    //使用bit=7时，打开图片可以看到图片最上面的图片颜色信息有明显的痕迹
    //lena
    strcpy(str,"Lena!This is the DIP course.");
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    BitPlane(pimg,str,bit);
    WritePPM("lena_bitplane.ppm",pimg);
    str[0]=0;//清空
    unBitPlane(pimg,str,bit);
    printf("加密信息是: %s \n",str);

    //Parrots
    strcpy(str,"Parrots!This is the DIP course.");
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    BitPlane(pimg,str,bit);
    WritePPM("Parrots_bitplane.ppm",pimg);
    str[0]=0;//清空
    unBitPlane(pimg,str,bit);
    printf("加密信息是: %s \n",str);
    return 0;
}

/*作业二*/
int H2()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;
    //lena
    strcpy(fname,"lena.ppm");
    ReadPPM(fname,pimg);
    NegativePPM(pimg);
    strcpy(fname,"lena_m.ppm");
    WritePPM(fname,pimg);
    //parrots 
    strcpy(fname,"Parrots.ppm");
    ReadPPM(fname,pimg);
    NegativePPM(pimg);
    strcpy(fname,"Parrots_m.ppm");
    WritePPM(fname,pimg);
    return 0;
}

/*作业三*/
int H3()
{
    char fname[file_name_len];
    struct IMG image;
    struct IMG * pimg=&image;

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
    free(array);
    return 0;
}

/*注意事项*/
/*
 * 1.图片的maxv是255 有些要注意+1
 * 2.位操作要加强啊，bitPlane操作起来有点力不从心啊，用了将近4个小时
 * 3.H3()和H3_1()不能同时运行，一运行H3_1()的加密信息就不准。有待时间修改
 * 4.那个BitPlane在vc下运行有问题，而vs2005以上是没有问题
 * */
