/* 
 * File: qtextctrl.h and qtextctrl.cpp
 * Class name: QTextCtrl
 * Author: YeHuoHangXing
 * Description: This class create a Text Edit Ctrl who's size can changed by draging 
 * the 8 control points.
 * Qt version: Qt5.5.1
 * Data: 2016.01.09
 * 
 */


#ifndef QTEXTCTRL_H
#define QTEXTCTRL_H

#include <QtWidgets/QWidget>
#include <QTextEdit>
#include <QPainter>
#include <QImage>
#include <QSize>
#include <QRect>
#include <QPoint>
#include <QMouseEvent>

//editor control point
#define EDIT_OFFSET			2
#define RECT_WIDTH			5		//control rect size.

class QTextCtrl : public QWidget
{
	Q_OBJECT

public:
	QTextCtrl(QWidget *parent = 0);
	QTextCtrl(QRect rect,QWidget *parent = 0);
	~QTextCtrl();

	void setTextEdit();
	void updateShow();
	void setFont(QFont font);								//set the text font.
	QImage *textToImage();	
	QRect getTextRect();

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	//bool eventFilter(QObject *obj, QEvent *event);

private:
	enum dragPos
	{
		dpLeftTop		= 00,
		dpMidTop		= 01,
		dpRightTop		= 02,

		dpMidLeft		= 10,
		dpMidRight		= 12,

		dpLeftBottom	= 20,
		dpMidBottom		= 21,
		dpRightBottom	= 22
	};
	dragPos dpos;
	//bool bDragEnable;
	bool bMouseMoving;

	QTextEdit *qTextEdit;
	QRect qRect;
};

#endif // QTEXTCTRL_H
