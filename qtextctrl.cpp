#include "qtextctrl.h"

QTextCtrl::QTextCtrl(QWidget *parent)
	: QWidget(parent)
{
	this->setMouseTracking(true);

	qRect = this->geometry();

	bMouseMoving = false;

	setTextEdit();
}

QTextCtrl::QTextCtrl(QRect rect,QWidget *parent)
	: QWidget(parent)
{
	this->setMouseTracking(true);

	qRect.setLeft(rect.left() - EDIT_OFFSET);
	qRect.setRight(rect.right() + EDIT_OFFSET);
	qRect.setTop(rect.top() - EDIT_OFFSET);
	qRect.setBottom(rect.bottom() + EDIT_OFFSET);
	this->setGeometry(qRect);

	bMouseMoving = false;

	setTextEdit();
}

QTextCtrl::~QTextCtrl()
{
	delete qTextEdit;
}

void QTextCtrl::setTextEdit()
{
	// set the text edit
	qTextEdit = new QTextEdit(this);
	qTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	qTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	/*
	//qTextEdit->setTextColor(Qt::blue);
	QPalette palette; 									//set the color can use: setPalette, setStyle or setStyleSheet.
	palette.setColor(QPalette::Base,Qt::transparent);	//QPalette::Base,指文本输入窗口部件(比如QTextEdit等)的背景色
	palette.setColor(QPalette::Text,Qt::blue);			//QPalette::Text,文本输入窗口部件的前景色
	palette.setColor(QPalette::WindowText,Qt::blue);	//QPalette::WindowText,文字前景颜色
	qTextEdit->setPalette(palette);
	*/

	//qTextEdit->setStyleSheet("background-color:transparent");
	qTextEdit->setStyleSheet("background-color: rgba(255,0,0,0);\
							  border-width: 1px;\
							  border-style: solid;\
							  border-color: transparent;\
							  color:blue;");	//border-color: rgb(233,34,255)

	QRect rect(EDIT_OFFSET,EDIT_OFFSET,qRect.width()-2*EDIT_OFFSET,qRect.height()-2*EDIT_OFFSET);
	qTextEdit->setGeometry(rect);
	qTextEdit->show();

	//qTextEdit->installEventFilter(this);
}

void QTextCtrl::updateShow()
{
	this->setGeometry(qRect);
	QRect rect(EDIT_OFFSET,EDIT_OFFSET,qRect.width()-2*EDIT_OFFSET,qRect.height()-2*EDIT_OFFSET);
	qTextEdit->setGeometry(rect);
}

QImage* QTextCtrl::textToImage()
{
	QSize size = qTextEdit->size();
	QImage *image = new QImage(size,QImage::Format_ARGB32_Premultiplied);

	qTextEdit->render(image);

	return image;
}

QRect QTextCtrl::getTextRect()
{
	QRect rect = this->geometry();
	rect.setTopLeft(rect.topLeft() + QPoint(EDIT_OFFSET,EDIT_OFFSET));
	rect.setBottomRight(rect.bottomRight() - QPoint(EDIT_OFFSET,EDIT_OFFSET));
	return rect;
}

void QTextCtrl::setFont(QFont font)
{
	qTextEdit->setFont(font);
}

//*************************************************************************************//


//bool QTextCtrl::eventFilter(QObject *obj, QEvent *e)
//{
//	//if you want to filter the event out, return true. (filter out:滤掉)
//	
//	if(obj == qTextEdit)
//	{
//		if(e->type() == QEvent::MouseMove)
//		{
//			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
//
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		return QTextCtrl::eventFilter(obj, e);
//	}
//}

namespace
{
	QPoint lastPoint;
	QPoint thisPoint;
}
void QTextCtrl::paintEvent(QPaintEvent *)
{
	//draw the control box with 8 points.
	int x1 = qRect.width() - RECT_WIDTH;
	int y1 = qRect.height() - RECT_WIDTH;

	QPainter p(this);
	p.setPen(qRgb(233,34,255));
	p.drawLine(EDIT_OFFSET,EDIT_OFFSET, x1+EDIT_OFFSET,EDIT_OFFSET);
	p.drawLine(x1+EDIT_OFFSET,EDIT_OFFSET,x1+EDIT_OFFSET,y1+EDIT_OFFSET);
	p.drawLine(x1+EDIT_OFFSET,y1+EDIT_OFFSET,EDIT_OFFSET,y1+EDIT_OFFSET);
	p.drawLine(EDIT_OFFSET,y1+EDIT_OFFSET,EDIT_OFFSET,EDIT_OFFSET);

	QImage img(RECT_WIDTH,RECT_WIDTH,QImage::Format_ARGB32_Premultiplied);
	img.fill(Qt::black);
	p.drawImage(0,0,img);
	p.drawImage(x1,0,img);
	p.drawImage(0,y1,img);
	p.drawImage(x1,y1,img);

	p.drawImage(x1/2,0,img);
	p.drawImage(0,y1/2,img);
	p.drawImage(x1/2,y1,img);
	p.drawImage(x1,y1/2,img);
}

#if(1)
// mouse event
void QTextCtrl::mousePressEvent(QMouseEvent *e)
{
	if(e->button() & Qt::LeftButton)
	{
		bMouseMoving = true;
		this->setFocus();

		//remember the left top point of this's geometry.
		//这里记住的是定点，即拖拽点的相反位置点
		switch (dpos)
		{
		case QTextCtrl::dpLeftTop:
		case QTextCtrl::dpMidTop:
			lastPoint = this->geometry().bottomRight();	
			break;
		case QTextCtrl::dpRightBottom:
		case QTextCtrl::dpMidBottom:
			lastPoint = this->geometry().topLeft();	
			break;

		case QTextCtrl::dpRightTop:
		case QTextCtrl::dpMidRight:
			lastPoint = this->geometry().bottomLeft();	
			break;
		case QTextCtrl::dpLeftBottom:
		case QTextCtrl::dpMidLeft:
			lastPoint = this->geometry().topRight();	
			break;

		default:
			bMouseMoving = false;
			break;
		}
	}
}

void QTextCtrl::mouseMoveEvent(QMouseEvent *e)
{
	if(bMouseMoving)
	{
		thisPoint = this->mapToParent(e->pos());
		if((thisPoint - lastPoint).manhattanLength() > 50)
		{
			int x0,x1,y0,y1;
			if(lastPoint.x() < thisPoint.x())
			{
				x0 = lastPoint.x();
				x1 = thisPoint.x();
			}
			else
			{
				x0 = thisPoint.x();
				x1 = lastPoint.x();
			}
			if(lastPoint.y() < thisPoint.y())
			{
				y0 = lastPoint.y();
				y1 = thisPoint.y();
			}
			else
			{
				y0 = thisPoint.y();
				y1 = lastPoint.y();
			}

			switch (dpos)
			{
			case QTextCtrl::dpLeftTop:			
			case QTextCtrl::dpRightTop:
			case QTextCtrl::dpLeftBottom:
			case QTextCtrl::dpRightBottom:
				qRect.setLeft(x0);
				qRect.setTop(y0);
				qRect.setRight(x1);
				qRect.setBottom(y1);
				break;
			
			case QTextCtrl::dpMidTop:
			case QTextCtrl::dpMidBottom:
				qRect.setTop(y0);
				qRect.setBottom(y1);
				break;

			case QTextCtrl::dpMidLeft:
			case QTextCtrl::dpMidRight:
				qRect.setLeft(x0);
				qRect.setRight(x1);
				break;
			}
			updateShow();
		}
	}
	else
	{
		QPoint point = e->pos();
		QRect rect(10*EDIT_OFFSET,10*EDIT_OFFSET,qRect.width()-20*EDIT_OFFSET,qRect.height()-20*EDIT_OFFSET);
		if((rect.contains(point)))
		{
			this->setCursor(Qt::IBeamCursor);
		}
		else
		{
			int row,col;
			int len;
			len = qRect.height()/3;
			row = point.y()/len;
			if(row >= 3) row = 2;
			len	= qRect.width()/3;
			col = point.x()/len;
			if(col >=  3) col = 2;
			dpos = dragPos(row*10 + col);

			switch (dpos)
			{
			case QTextCtrl::dpLeftTop:
			case QTextCtrl::dpRightBottom:
				this->setCursor(Qt::SizeFDiagCursor);
				break;

			case QTextCtrl::dpRightTop:
			case QTextCtrl::dpLeftBottom:
				this->setCursor(Qt::SizeBDiagCursor);
				break;

			case QTextCtrl::dpMidTop:
			case QTextCtrl::dpMidBottom:
				this->setCursor(Qt::SizeVerCursor);
				break;

			case QTextCtrl::dpMidLeft:
			case QTextCtrl::dpMidRight:
				this->setCursor(Qt::SizeHorCursor);
				break;
			}
		}
	}
}

void QTextCtrl::mouseReleaseEvent(QMouseEvent *e)
{
	if((e->button() & Qt::LeftButton)
		&& bMouseMoving )
	{
		bMouseMoving = false;
	}
}

#else

// mouse event
QPoint startPoint;
void QTextCtrl::mousePressEvent(QMouseEvent *e)
{
	if(e->button() & Qt::LeftButton)
	{
		bMouseMoving = true;
		this->setFocus();
		startPoint = e->globalPos();			//globalPos():relative to the screen.
	}
}

void QTextCtrl::mouseMoveEvent(QMouseEvent *e)
{
	if(bMouseMoving)
	{
		QPoint offset = e->globalPos() - startPoint;
		//if(offset.manhattanLength() < 50)
		//	return;
		QRect rect = this->geometry();

		switch (dpos)
		{
		case QTextCtrl::dpLeftTop:
			rect.setTopLeft(rect.topLeft() + offset); 
			break;
		case QTextCtrl::dpMidTop:
			rect.setTop(rect.top() + offset.y()); 
			break;
		case QTextCtrl::dpRightTop:
			rect.setTopRight(rect.topRight() + offset); 
			break;
		case QTextCtrl::dpMidLeft:
			rect.setLeft(rect.left() + offset.x()); 
			break;
		case QTextCtrl::dpMidRight:
			rect.setRight(rect.right() + offset.x()); 
			break;
		case QTextCtrl::dpLeftBottom:
			rect.setBottomLeft(rect.bottomLeft() + offset); 
			break;
		case QTextCtrl::dpMidBottom:
			rect.setBottom(rect.bottom() + offset.y()); 
			break;
		case QTextCtrl::dpRightBottom:
			rect.setBottomRight(rect.bottomRight() + offset); 
			break;
		}
		this->setGeometry(rect);

		qRect = this->geometry();
		QRect rect(EDIT_OFFSET,EDIT_OFFSET,qRect.width()-2*EDIT_OFFSET,qRect.height()-2*EDIT_OFFSET);
		qTextEdit->setGeometry(rect);

		startPoint = e->globalPos();
	}
	else
	{
		QPoint point = e->pos();
		QRect rect(10*EDIT_OFFSET,10*EDIT_OFFSET,qRect.width()-20*EDIT_OFFSET,qRect.height()-20*EDIT_OFFSET);
		if((rect.contains(point)))
		{
			this->setCursor(Qt::IBeamCursor);
		}
		else
		{
			int row,col;
			int len;
			len = qRect.height()/3;
			row = point.y()/len;
			if(row >= 3) row = 2;
			len	= qRect.width()/3;
			col = point.x()/len;
			if(col >=  3) col = 2;
			dpos = dragPos(row*10 + col);

			switch (dpos)
			{
			case QTextCtrl::dpLeftTop:
			case QTextCtrl::dpRightBottom:
				this->setCursor(Qt::SizeFDiagCursor);
				break;

			case QTextCtrl::dpRightTop:
			case QTextCtrl::dpLeftBottom:
				this->setCursor(Qt::SizeBDiagCursor);
				break;

			case QTextCtrl::dpMidTop:
			case QTextCtrl::dpMidBottom:
				this->setCursor(Qt::SizeVerCursor);
				break;

			case QTextCtrl::dpMidLeft:
			case QTextCtrl::dpMidRight:
				this->setCursor(Qt::SizeHorCursor);
				break;
			}
		}
	}
}

void QTextCtrl::mouseReleaseEvent(QMouseEvent *e)
{
	if((e->button() & Qt::LeftButton)
		&& bMouseMoving )
	{
		bMouseMoving = false;
	}
}
#endif