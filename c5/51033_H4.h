/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-09-18 22:18
 * @Author: 林明晓 -- 563804489@qq.com
 * @Description: 
 *	把原来的程序分解成多个文件，便于管理
 *	51033_H4.h
 *	2013年10月16日 22:24:50 增加图像的锐化和边缘检测 和 FreePPM函数
 * @History: 
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//The const number

//定义文件名的长度
#define file_name_len 1024
//由于这个大小问题错了好多次
#define str_info_len 1024

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
 * 图像的卷积，模版函数的定义
 * Template 模版
 * */
struct Template
{
    int x;
    int y;
    int * square;//模版方框的数据
    char * name;//用于保存该模版的名字，附加作用
    int max;//表示该模版可能存在max倍的灰度值，所以可能要出去
};

/*
 * 读取PPM图片，并返回到IMG中
 * */
int ReadPPM(char * fname,struct IMG * img);

/*
 * 将IMG中的PPM图片文件写入到文件系统中
 * */
int WritePPM(char * fname,struct IMG * img);

/*
 * 对PPM图片进行手动释放内存
 * */
int FreePPM(struct IMG * img);

/*
 * 进行负片的计算 直接将原图片转换成负片
 * */
int NegativePPM(struct IMG * img);

/*
 * 计算PPM图片的灰度直方图，并保存到hist数组中
 * */
int HistPPM(int * hist,struct IMG * img);

/*
 * 将PPM图片的灰度直方图写到文件中
 * */
int WriteHist(int * hist,char * fname,struct IMG * img);

/*
 * 计算一个PPM6彩色图片的分量图
 * return -1 表示图片为黑白图片不能计算分量图
 * 保存到fname中
 * */
int ComponentPPM(char * fname,struct IMG * img);

/*
 * 映射函数数组生成
 * */
int Square_MapPPM(int * arrmap,int maxv);

/*
 * 映射函数数组生成
 * */
int Sqrt_MapPPM(int * arrmap,int maxv);

/*
 * 映射函数，输入一幅PPM图片和一个转换数组进行映射
 * */
int MapPPM(int * arrmap,struct IMG * img);

/*
 * BitPlane 图片的水印隐藏
 * 参数 原图片，要加密的文字，bit位处
 * 注意 直接操作原图片
 * */
int BitPlane(struct IMG * img,char * str,int bit);

/*
 * unBitPlane 对已经加入水印的图片进行解密
 * 参数 已加密图片，加密信息保存位置，bit位
 * */
int unBitPlane(struct IMG * img,char * str,int bit);

/*
 * BinaryZationPPM 图片的多值化
 * 参数 PPM图片，val值化
 * */
int BinaryZationPPM(struct IMG * img,int val);

/*
 * CopyPPM 对图片进行复制
 * 参数struct IMG * from,struct IMG * to
 * */
int CopyPPM(struct IMG * from, struct IMG * to);

/*
 * HistEqualization : Histogram Equalization
 * 直方图均衡化 参数1图像，参数2 hist数组
 * 图像要有值，hist可以不用
 * 返回值：返回处理后的图像，而hist返回处理后图像的直方图
 * */
int HistEqualization(struct IMG * img,int * hist);

/*
 * 图像的卷积 Image convolution
 * 输入是一副图像，和一个模版
 * */
int Convolution(struct IMG * img,struct Template * temple);
