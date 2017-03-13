#ifndef QDRAWAREA_H
#define QDRAWAREA_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QBitmap>
#include <QCursor>
#include <QPoint>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include <QDrag>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>

#include "common.h"
#include "qtextctrl.h"

//class QDrawArea
class QDrawArea : public QWidget
{
	Q_OBJECT

public:
	QDrawArea(QWidget *parent = 0);
	QDrawArea(int gw,int gh,QWidget *parent = 0);
	~QDrawArea();

	int gridWidth()
		{return nGridW;}
	int gridHeight()
		{return nGridH;}
	bool *gridpAllPoint()
		{return pbGridAllPoint;}
	void setGridArea(int gw,int gh,bool *gp);			//when set the pbGridAllPoint, it need the nGridW and nGridH.

	void setCursorR(short unsigned int r)				//set brush and erase cursor size
		{nCursorR = r;}
	void setPointCursor(QPointCursor pc,const QCursor &c)
	{
		qPointCursor = pc; qCursor = c; this->setCursor(c);
	}													//set point cursor status.
	void setGridGray(int g);							//set grid line color(gray).
	void setImg2Grid(byte type,int value);

	void setInit();
	void exportArray();									//输出数组
	void loadArray(QFile*);								//load grid point data from byte array.

	void loadPicture(const QString&, byte, int);
	void unloadPicture();
	void convertPicture(QImage*,int,byte);				//convert image to grid.

	void setTextFont(QFont font);						//set the text font to convert to grid.
	void createFontArea(QPoint p1,QPoint p2);			//create the area to type text.
	void createFontArea();
	void convertText();

	void clearGrid();									//clear all the point of grid.
	void drawGrid();
	void calcGrid(QPoint p1,QPoint p2);					//Calc the point of the grid.
	void zoomInGrid();									//Zoom in.
	void zoomOutGrid();									//Zoom out.
	void rotateLeftGrid();								//Rotate left(anti-clockwise).
	void rotateRightGrid();								//Rotate right(clockwise).
	void flipHorizontalGrid();							//水平翻转
	void flipVerticalGrid();							//垂直翻转

	// 重载事件
	void paintEvent(QPaintEvent *);
	void wheelEvent(QWheelEvent *);                     //set the GridSize with wheel
	void mousePressEvent(QMouseEvent *);                //Get the point of mouse
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

private:
	//Key status
	bool bMouseDrawing;									//Mouse is moving for drawing.
	bool bMouseMoving;									//Mouse is moving for moving. 

	//DrawGraph
	int nGridW;											//the num of lines in grid width.							
	int nGridH;
	int nGridSize;
	byte nGridGray;										//Grid line color(Gray).
	int nGridX0;
	int nGridY0;
	int nImgWidth;
	int nImgHeight;
	bool *pbGridAllPoint;
	QImage *qImg;

	//Cursor
	QPointCursor qPointCursor;
	QCursor	qCursor;
	short unsigned int nCursorR;

	//Load picture for graph
	bool bLoadedPic;
	QImage *qLoadImg;

	//Text for graph
	bool bCreatedFontArea;								//the qEdit has been created.
	QTextCtrl *qEdit;
	QFont qFont;
	bool bRectText;										//drag a rect for creating font area.
	QImage *rectImg;

	//Pop Menu
	QMenu   *pPopMenu;
	QAction *qPop_ExportAr;

signals:
	void Singals_ShowPoint(QString &str);
};

#endif // QDRAWAREA_H
