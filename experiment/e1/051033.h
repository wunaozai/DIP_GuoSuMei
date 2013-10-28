/****************************************
 * Copyright (C) 2013-2013 All rights reserved.
 * @Version: 1.0
 * @Created: 2013-10-17 20:05
 * @Author: 林明晓 -- 563804489@qq.com
 * @Description: 这是实验的头文件，包含所有函数的声明，和结构体定义。
 * @GitHub: https://github.com/wunaozai/DIP_GuoSuMei
 *
 * @History: 
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**
 * @宏定义
 * file_name_len 定义文件名的长度
 * str_info_len 定义字符串的最大长度
 **/
#ifndef LEN
#define LEN 0

#ifdef linux
#define SYSTEM 1
#endif
#ifdef WIN32
#define SYSTEM 2
#endif

#define file_name_len 1024
#define str_info_len 1024
#define ERROR -1
#endif

/**
 * @图片的格式
 * ch:教学使用的是ppm格式的图片，该值一般为P
 * channel:通道 5表示黑白图片 6表示彩色图片
 * sx: 图片的横坐标最大长度
 * sy: 图片的纵坐标最大长度
 * maxv: 图片的灰度值
 * img: 图片的数据，实验时要动态申请内存
 **/
struct IMG
{
    char ch;
    int channel;
    int sx,sy;
    int maxv;
    unsigned char * img;
};

/**
 * @图像的卷积，模版函数的定义,Template 模版
 * x: 模版函数的x方向的个数
 * y: 模版函数的y方向的个数
 * square: 模版方框的数据
 * name: 用于保存该模版的名字，附加作用
 * max: 表示该模版可能存在max倍的灰度值，所以可能要出去
 **/
struct Template
{
    int x;
    int y;
    int * square;
    char * name;
    int max;
};


/**
 * @ 函数名: ReadPPM
 * @ 函数作用: 读取PPM图片
 * @ 参数: 要读取PPM图片文件的文件名 PPM文件的指针
 * @ 返回值: 返回0表示正常处理 其他的返回表示出错
 * @ 描述: 成功读取图片后会保存到img里面去
 **/
int ReadPPM(char * fname,struct IMG * img);

/**
 * @ 函数名: WritePPM
 * @ 函数作用: 将IMG中的PPM图片文件写入到文件系统中
 * @ 参数: 要写入PPM图片文件的文件名 PPM文件的指针
 * @ 返回值: 返回0表示正常处理 其他的返回表示出错
 * @ 描述: 成功保存完图片后，img里面的信息还在
 **/
int WritePPM(char * fname,struct IMG * img);

/**
 * @ 函数名: FreePPM
 * @ 函数作用: 对PPM图片进行手动释放内存
 * @ 参数: 要释放图片内存的指针
 * @ 返回值: 0表示正常清除
 * @ 描述: 
 **/
int FreePPM(struct IMG * img);

/**
 * @ 函数名: CopyPPM
 * @ 函数作用: 对图片进行复制
 * @ 参数: 要进行复制的源图片，复制后的目标
 * @ 返回值: 0表示正常复制
 * @ 描述: 由于程序中要很多操作都是直接对源图片进行修改的
 *         所以要保留源图片时，要进行图片的复制。
 **/
int CopyPPM(struct IMG * from, struct IMG * to);

/**
 * @ 函数名: HistEqualization
 * @ 函数作用: 直方图均衡化
 * @ 参数: 参数1图像，参数2 hist数组
 * @ 返回值: 0表示均衡化成功
 * @ 描述: 图像要有值，hist可以不用,指针返回处理后的图像，而hist返回处理后图像的直方图
 **/
int HistEqualization(struct IMG * img,int * hist);

/**
 * @ 函数名: HistPPM
 * @ 函数作用: 计算PPM图片的灰度直方图，并保存到hist数组中
 * @ 参数: hist数组用于保存灰度直方图，img是要进行操作的图片
 * @ 返回值: 0表示正常统计
 * @ 描述: 
 **/
int HistPPM(int * hist,struct IMG * img);

/**
 * @ 函数名: WriteHist
 * @ 函数作用: 将PPM图片的灰度直方图写到文件中
 * @ 参数: 参数1 要输出到文件的hist数组  参数2 输出文件的文件名(该处的文件名不用后缀) 参数3 hist所对应的那个PPM图片
 * @ 返回值: 0表示正常写入
 * @ 描述: 主要使用PPM图片中的颜色信息，用于判断hist是1倍maxv还是3倍maxv
 **/
int WriteHist(int * hist,char * fname,struct IMG * img);

/**
 * @ 函数名: Convolution
 * @ 函数作用: 图像的卷积 Image convolution
 * @ 参数: 输入是一副图像，和一个模版结构体
 * @ 返回值: 0表示正常处理
 * @ 描述: img直接修改，如果要保存原图片可以使用CopyPPM
 **/
int Convolution(struct IMG * img,struct Template * temple);

/**
 * @ 函数名: Ostu
 * @ 函数作用: 使用Ostu算法对图像进行二值化
 * @ 参数: 输入一副图像
 * @ 返回值: 0表示正常处理
 * @ 描述: img直接修改
 **/
int Ostu(struct IMG * img);


/**
 * @ 函数名: 
 * @ 函数作用: 
 * @ 参数: 
 * @ 返回值: 
 * @ 描述: 
 **/

