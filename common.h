#ifndef COMMON_H
#define COMMON_H


// Version
#define	_GRIDARRAY_QT_VERSION_ "Create picture array for OLED screen.\n\nVersion 1.2.1"



/* include */
#include <QDebug>
#include <qmath.h>

/* data */
typedef unsigned char byte;

//支持的文件格式
#define FILE_FALSE		0x00			//不支持
#define FILE_GAQ		0x01
#define FILE_PIC		0x02
#define FILE_TXT		0x03

#pragma  pack(1)
struct GAQFILE
{
	byte *image;
	int gw;
	int gh;
	bool *gp;
	int imageSize;
};
#pragma pack()
/*
GAQ文件格式：
ImageData: ImageSize bytes
FileHead: 4 bytes						[File size, not include itself or FileEnd]
GridWidth: 4 bytes
GridHeight: 4 bytes
GridPoint: GridWidth*GridHeight byte
FileEnd: 4 bytes						[FileEnd = ~FileHead +1]
ImageSize: 4 bytes						[The image size data]
*/


//光标
enum QPointCursor{Arrow,Point,Brush,Erase,Text};

//图像转格子点方法
//#define IMG2GRID_TEXT		0x00		//convert text to picture and then to grid.
#define IMG2GRID_Gray		0x01
#define IMG2GRID_Red		0x02
#define IMG2GRID_Green		0x03
#define IMG2GRID_Blue		0x04



/* define */
//#define MAX(a,b)			(((a) > (b))?(a):(b))
//#define MIN(a,b)			(((a) < (b))?(a):(b))

#define SetBit(b,bit)		((b) |= 1<<(bit))
#define ClrBit(b,bit)		((b) &= ~(1<<(bit)))
#define BitIsHigh(b,bit)	(0 != ((b)&(1<<(bit))))
#define BitIsLow(b,bit)		(0 == ((b)&(1<<(bit))))

#endif // !COMMON_H
