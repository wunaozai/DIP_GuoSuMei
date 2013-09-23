/****************************************
* Copyright (C) 2013-2013 All rights reserved.
* @Version: 1.0
* @Created: 2013-09-04 01:04
* @Author: 林明晓 -- 563804489@qq.com
* @Description: 
*
* @History: 
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int x=256,y=64;
    int i,j;
    char name[16];
    char *Img;
    FILE * fp;
    //printf("请输入x(256)方向像素点:");
    //scanf("%d",&x);
    //printf("请输入y(64)方向像素点:");
    //scanf("%d",&y);
    //sprintf(name,"x_%d_y_%d.raw",x,y);
    strcpy(name,"51033_H1.raw");
    fp=fopen(name,"wb");
    if(fp==NULL)
    {
	printf("文件创建失败！\n");
	exit(-1);
    }
    Img=malloc(256*64);
    if(Img==NULL)
    {
	printf("分配内存失败。\n");
	exit(-1);
    }
    for(i=0;i<64;i++)
    {
	for(j=0;j<256;j++)
	{
	    Img[i*256+j]=j;
	}
    }
    fwrite(Img,256,64,fp);
    fclose(fp);
    printf("创建%s文件成功！\n",name);
    return 0;
}
