#include "QDrawArea.h"

QDrawArea::QDrawArea(QWidget *parent)
	: QWidget(parent)
{
	nGridW = 128;
	nGridH = 64;

	pbGridAllPoint = new bool[nGridW*nGridH];
	memset(pbGridAllPoint,0,nGridW*nGridH);

	setInit();
	drawGrid();
}

QDrawArea::QDrawArea(int gw,int gh,QWidget *parent)		//默认参只能在声明或定义处写，不能两处同时写
	:nGridW(gw),nGridH(gh),QWidget(parent)
{
	pbGridAllPoint = new bool[nGridW*nGridH];
	memset(pbGridAllPoint,0,nGridW*nGridH);

	setInit();
	drawGrid();
}

QDrawArea::~QDrawArea()
{
	delete qImg;
	delete pbGridAllPoint;

	if(bLoadedPic)
	{
		delete qLoadImg;
	}

	if(bCreatedFontArea)
	{
		delete qEdit;
	}
}


void QDrawArea::setInit()
{
	this->setCursor(Qt::CrossCursor);
	this->setMouseTracking(true);	//true: the widgets will receive mouseMoveEvent though no buttons are pressed.
	this->setAcceptDrops(true);		//set drap and drop.

	//Data
	nGridSize = 7;
	nGridGray = 200;
	nImgWidth = 1000;
	nImgHeight = 600;
	nGridX0 = 20;
	nGridY0 = 20;

	nCursorR = 20;
	bLoadedPic = false;
	bMouseDrawing = false;
	bMouseMoving = false;
	bCreatedFontArea = false;
	bRectText = false;

	qImg = new QImage(nImgWidth, nImgHeight, QImage::Format_RGB32);

	//Pop menu
	pPopMenu = new QMenu();
	qPop_ExportAr = new QAction(tr("Export Array"),this);
	pPopMenu->addAction(qPop_ExportAr);
	connect(qPop_ExportAr,&QAction::triggered,this,&QDrawArea::exportArray);
}

void QDrawArea::setGridArea(int gw,int gh,bool *gp)
{
	nGridW = gw;
	nGridH = gh;

	delete pbGridAllPoint;
	pbGridAllPoint = new bool[nGridW*nGridH];
	memcpy(pbGridAllPoint, gp, nGridW*nGridH);
}

void QDrawArea::setGridGray(int g)
{
	nGridGray = (byte)g;
	drawGrid();
}
			
void QDrawArea::setImg2Grid(byte type, int value)
{
	if(bLoadedPic)
	{
		clearGrid();
		convertPicture(qLoadImg,type,(byte)value);
		drawGrid();
	}
}


void QDrawArea::loadPicture(const QString &str,byte type,int value)
{
	if(bLoadedPic)
		delete qLoadImg;
	bLoadedPic = true;
	qLoadImg = new QImage();
	if(qLoadImg->load(str))
	{
		bLoadedPic =  true;
		clearGrid();
		convertPicture(qLoadImg,type,(byte)value);
		drawGrid();
	}
	else
	{
		QMessageBox::information(this,tr("Error"),tr("Invalid picture!"));
	}
}

void QDrawArea::unloadPicture()
{
	if(bLoadedPic)
	{
		bLoadedPic = false;
		delete qLoadImg;
	}
}

void QDrawArea::convertPicture(QImage *image,int type, byte value)
{
	int i,j;
	int picw,pich;					//图像宽高
	byte tmp;
	QRgb qClr;
	picw = image->width();
	pich = image->height();
	picw = qMin(nGridW,picw);
	pich = qMin(nGridH,pich);
	for(i = 0; i < pich; i ++)					//row
	{
		for(j = 0; j < picw; j ++)				//col
		{
			qClr = image->pixel(j,i);			//Returns the color of the pixel at coordinates (x, y). x is col and y is row.
			switch (type)
			{
			case IMG2GRID_Gray:
				tmp = (qRed(qClr) + qGreen(qClr) + qBlue(qClr))/3;
				break;
			case IMG2GRID_Red:
				tmp = qRed(qClr);
				break;
			case IMG2GRID_Green:
				tmp = qGreen(qClr);
				break;
			case IMG2GRID_Blue:
				tmp = qBlue(qClr);
				break;
			}
			if(tmp > value)
				*(pbGridAllPoint + i *nGridW + j) = true;
		}
	}
}


//rect create.
void QDrawArea::createFontArea(QPoint p1,QPoint p2)
{
	if(bCreatedFontArea)
		delete qEdit;
	bCreatedFontArea = true;

	qEdit = new QTextCtrl(QRect(p1.x(),p1.y(),p2.x()- p1.x(),p2.y()-p1.y()),this);
	qEdit->setFont(qFont);
	qEdit->show();
}

//double click create.
void QDrawArea::createFontArea()
{
	if(bCreatedFontArea)
		delete qEdit;
	bCreatedFontArea = true;

	nGridSize = 1;
	drawGrid();

	qEdit = new QTextCtrl(QRect(nGridX0,nGridY0,nGridW+1,nGridH+1),this);
	qEdit->setFont(qFont);
	qEdit->show();
}

void QDrawArea::convertText()
{
	if(bCreatedFontArea)
	{
		QImage *image = qEdit->textToImage();
		//image->save("arimage.png","png",0);
		//(this->grab(qEdit->geometry())).save("arpixmap.png","png",100);
		QRect rect = qEdit->getTextRect();		//target rect.

		//get the new array of image of text.
		int trow,tcol;
		bool *tPoint;					//new array of image of text.
		int sr,sc;						//compare start point.
		int t0,t1;
		t0 = qMax(rect.left(), nGridX0); 
		t1 = qMin(rect.right(), nGridX0+nGridW*nGridSize);
		t0 = ((t0 - nGridX0)/nGridSize) * nGridSize + nGridX0;			
		t1 = ((t1 - nGridX0)/nGridSize) * nGridSize + nGridX0;
		if(rect.right() > t1)
		{
			t1 += nGridSize;
		}
		tcol = (t1 - t0) / nGridSize;
		sc = (t0 - nGridX0) / nGridSize;	

		t0 = qMax(rect.top(), nGridY0);
		t1 = qMin(rect.bottom(), nGridY0+nGridH*nGridSize);
		t0 = ((t0 - nGridY0)/nGridSize) * nGridSize + nGridY0;	
		t1 = ((t1 - nGridY0)/nGridSize) * nGridSize + nGridY0;
		if(rect.bottom() > t1)
		{
			t1 += nGridSize;
		}
		trow = (t1 - t0) / nGridSize;
		sr = (t0 - nGridY0) / nGridSize;
		tPoint = new bool[trow*tcol];
		memset(tPoint,false,trow*tcol);

		//calculate the value of tPoint array.
		int i,j,ii,jj;
		int cnt = 0,k = nGridSize*nGridSize/2;
		QRgb qClr;
		for(i = 0; i < trow; i ++)
		{
			for(j = 0; j < tcol; j++)
			{
				cnt = 0;
				t0 = (i+1)*nGridSize;
				t1 = (j+1)*nGridSize;
				for(ii = i*nGridSize; ii < t0 ; ii ++)
				{
					for(jj = j*nGridSize; jj < t1; jj ++)
					{
						if(ii >= image->height() || jj >= image->width())
							continue;
						qClr = image->pixel(jj,ii);
						if((qRed(qClr) + qGreen(qClr))/2 < 35)			//the target(text) color is blue.
						{
							cnt ++;
						}
					}
				}
				if(cnt > k)
					*(tPoint + i*tcol + j) = true;
			}
		}

		//set the value of pbGridAllPoint.
		k = nGridW;
		for(i = 0; i < trow; i ++)
		{
			for(j = 0; j < tcol; j++)
			{
				ii = sr + i;
				jj = sc + j;
				*(pbGridAllPoint + ii*k + jj) = *(tPoint + i*tcol + j);
			}
		}

		bCreatedFontArea = false;
		this->setCursor(Qt::IBeamCursor);				//reset the cursor.

		delete qEdit;
		delete []tPoint;
	}
}

void QDrawArea::setTextFont(QFont font)
{
	qFont = font;
	if(bCreatedFontArea)
		qEdit->setFont(qFont);
}



void QDrawArea::exportArray()
{
	QFile		file;
	QString		strFilePath;
	//Get file name
	strFilePath = QFileDialog::getSaveFileName(this,
		tr("Export Ar"),
		"Array",
		tr("Txt File(*.txt)"));	//static function
	//Write file
	if(!strFilePath.isEmpty())
	{
		int i,j,k = 0;
#if(1)
		//用数组表示所有点的状态
		//Get array
		unsigned char tmp;
		int y,yy;
		int gx = nGridW / 8;								//确定列需要多少个字节
		if(nGridW%8)
		{
			gx ++;
		}
		int gy = nGridH;									//列用字节表示，故行数不变
		unsigned char *pGridAr = new unsigned char[gx*gy];
		memset(pGridAr,0,gx*gy);
		for(int i = 0; i <  nGridH; i ++)
		{
			for(int j = 0; j < nGridW; j ++)
			{
				if(*(pbGridAllPoint + i*nGridW + j))
				{
					y = j / 8;									//确定列的第几个字节
					yy = j % 8;									//确定字节的第几位
					tmp = *(pGridAr + i*gx + y);
					SetBit(tmp, yy);							//从左向右遍历，左边的像素点放在字节的低位
					*(pGridAr + i*gx + y) = tmp;
				}
			}
		}

		//output array
		file.setFileName(strFilePath);
		if(file.open(QIODevice::WriteOnly))
		{
			QTextStream fin;
			fin.setDevice(&file);

			fin<<QString("const unsigned char Array[%1][%2] = {\n").arg((int)gy).arg((int)gx);	//gy为行数，gx为列数
			for(i = 0; i < gy; i ++)
			{
				for(j = 0; j < gx; j++)
				{
					fin<<QString("%1,").arg((int)(*(pGridAr + i*gx + j)),4,10);
				}
				fin<<"\n";
			}
			fin<<QString("\n};\n");
			file.close();

			delete[]pGridAr;
		}
		else 
		{
			QMessageBox::information(this,tr("Error"),tr("Open file Failed!"));
		}
#else

		int num = 0;
		for(i = 0; i < nGridH; i ++)			//行
		{
			for(j = 0; j < nGridW; j ++)		//列
			{
				if(*(pbGridAllPoint + i*nGridW + j))
				{
					num++;
				}
			}
		}

		//no point
		if(!num)
		{
			QMessageBox::information(this,tr("Error"),tr("Point num is zero!"));
			return;
		}

		//用两个数组分别表示标记的点行列坐标
		//Get array
		int *GridX = new int[num];
		int *GridY = new int[num];
		memset(GridX,0,num);
		memset(GridY,0,num);
		for(i = 0; i < nGridH; i ++)
		{
			for(j = 0; j < nGridW; j ++)
			{
				if(*(pbGridAllPoint + i*nGridW + j))
				{
					GridX[k] = i;
					GridY[k] = j;
					k++;
				}
			}
		}

		//output array
		file.setFileName(strFilePath);
		if(file.open(QIODevice::WriteOnly))
		{
			QTextStream fin;
			fin.setDevice(&file);

			fin<<QString("const short unsinged int ArrayNum = %1;\n").arg(num);
			fin<<QString("const unsigned char ArrayX[%1] = {\n").arg(num);
			for(k = 0,i = 0; i < num; i ++)
			{
				fin<<QString("%1,").arg(*(GridX+i),4);
				k ++;
				if(k%10 == 0)
				{
					fin<<"\n";
				}
			}
			fin<<QString("\n};\n");

			fin<<QString("const unsigned char ArrayY[%1] = {\n").arg(num);
			for(k = 0,i = 0; i < num; i ++)
			{
				fin<<QString("%1,").arg(*(GridY+i),4);
				k ++;
				if(k%10 == 0)
				{
					fin<<"\n";
				}
			}
			fin<<QString("\n};\n");
		}
		delete []GridX;
		delete []GridY;
#endif
	}
}

void QDrawArea::loadArray(QFile *file)
{
	QTextStream fout;
	fout.setDevice(file);

	char ch;
	int i,j,k = 0;
	int gx = -1,gy = -1;									//gx is col, and gy is row
	unsigned char *gp;

	//find the gx and gy
	while (!fout.atEnd())
	{
		fout>>ch;
		if(ch == '[')
		{
			k++;
			if(k == 1)
			{fout>>gy;}			//row
			else if(k == 2)
			{fout>>gx;break;}	//col
		}
	}

	//find the unsinged char array.
	if(gx > 0 && gy > 0)
	{
		k = -1;
		i = 0;
		j = gx*gy;
		gp = new unsigned char[j];
		while (!fout.atEnd())
		{
			if(k < 0)
			{
				fout>>ch;
				if(ch == '{')
				{
					k++;
				}
			}
			else
			{
				fout>>i;
				fout>>ch;
				*(gp + k) = (unsigned char)i;
				k++;
				if(k >= j)
					break;
			}
		}
	}
	else
	{
		QMessageBox::information(this,tr("Error"),tr("The array in the txt file is invalid!"));
		return;
	}

	//find the pbGridAllPoint.
	delete pbGridAllPoint;
	nGridW = gx * 8;		//It may be that the actual nGridW < gx*8,
							//and we set nGridW = gx*8.  
	nGridH = gy;
	pbGridAllPoint = new bool[nGridW*nGridH];

	unsigned char tmp;
	for(i = 0; i < nGridH; i ++)
	{
		for(j = 0; j < nGridW; j++)
		{
			k = j / 8;
			tmp = *(gp + i * gx + k);
			k = j % 8;
			if(BitIsHigh(tmp,k))		//从左向右遍历，左边的像素点放在字节的低位
			{
				*(pbGridAllPoint + i * nGridW + j) = true;
			}
			else
			{
				*(pbGridAllPoint + i * nGridW + j) = false;
			}
		}
	}

	delete[]gp;

	unloadPicture();									//unload the picture after loading array.
	drawGrid();
}


void QDrawArea::drawGrid()
{
	QPainter *pp = new QPainter(qImg);
	QBrush	brush(Qt::red);						//grid point color;
	QColor  clr;
	int i,j;
	bool flag;									//true: draw line.
	if(nGridGray >= 255 || nGridSize < 4)
	{
		flag = false;
	}
	else
	{
		flag = true;
	}

	//Image
	qImg->fill(Qt::white);
	pp->setPen(qRgb(nGridGray,nGridGray,nGridGray));
	pp->setBrush(brush);
	if(flag)
	{
		//draw all lines.
		for(i = 0; i <= nGridH; i ++)
		{
			pp->drawLine(nGridX0,nGridY0+nGridSize*i,nGridX0+nGridW*nGridSize,nGridY0+nGridSize*i);
		}
		for(i = 0; i <= nGridW; i ++)
		{
			pp->drawLine(nGridX0+nGridSize*i,nGridY0,nGridX0+nGridSize*i,nGridY0+nGridH*nGridSize);
		}
	}
	else
	{
		//only draw the out most rect.
		pp->setPen(Qt::black);
		pp->drawLine(nGridX0,nGridY0,nGridX0+nGridW*nGridSize,nGridY0);
		pp->drawLine(nGridX0,nGridY0+nGridSize*nGridH,nGridX0+nGridW*nGridSize,nGridY0+nGridSize*nGridH);
		pp->drawLine(nGridX0,nGridY0,nGridX0,nGridY0+nGridSize*nGridH);
		pp->drawLine(nGridX0+nGridW*nGridSize,nGridY0,nGridX0+nGridW*nGridSize,nGridY0+nGridSize*nGridH);
	}

	for(i = 0; i < nGridH; i ++)
	{
		for(j = 0; j < nGridW; j ++)
		{
			if(*(pbGridAllPoint + i*nGridW + j))
			{
				if(!flag)
				{
					pp->setPen(Qt::red);
				}
				pp->setBrush(brush);
				pp->drawRect(nGridX0 + j*nGridSize, nGridY0 + i*nGridSize,nGridSize,nGridSize);
			}
			else
			{
				if(bLoadedPic)							//when the picture is loaded, display it.
				{
					clr = qLoadImg->pixel(j,i);
					if(!flag)
					{
						pp->setPen(clr);
					}
					pp->setBrush(QBrush(clr));
					pp->drawRect(nGridX0 + j*nGridSize, nGridY0 + i*nGridSize,nGridSize,nGridSize);
				}
			}
		}
	}

	delete pp;

	this->setFocus();			//get the focus, convenient for zoom(space).
	update();
}

void QDrawArea::calcGrid(QPoint p1,QPoint p2)
{
	int px,py;
	int xlen = 0,ylen = 0;			//the step size.
	int n,k;

	n = 0;							//count the step drawed.
	px = p1.x();
	py = p1.y();
	if(qPointCursor == Point)
	{
		k = qSqrt((p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y())) / (nGridSize);
		if(k)
		{
			xlen = (p2.x() - p1.x())/k;
			ylen = (p2.y() - p1.y())/k;
		}

		while (1)
		{
			//draw line as grid point.
			if(px > nGridX0 && py > nGridY0
				&& px < nGridX0 + nGridW*nGridSize
				&& py < nGridY0 + nGridH*nGridSize)
			{
				int i = (py - nGridY0) / nGridSize;		//行数
				int j = (px - nGridX0) / nGridSize;		//列数
				*(pbGridAllPoint + i * nGridW + j) = true;
			}

			px += xlen;
			py += ylen;
			n ++;
			if(n > k)
				break;
		}
	}
	else if(qPointCursor == Brush ||qPointCursor == Erase)
	{
		k = qSqrt((p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y())) / (nCursorR);
		if(k)
		{
			xlen = (p2.x() - p1.x())/k;
			ylen = (p2.y() - p1.y())/k;
		}

		while (1)
		{
			//计算包含于圆圈内有像素点
			int i,j,ii,jj;
			for(i = px-nCursorR; i <= px+nCursorR; i += nGridSize)
			{
				for(j = py-nCursorR; j <= py+nCursorR; j += nGridSize)
				{
					if(i <= nGridX0 || j <= nGridY0
						|| i >= nGridX0+nGridW*nGridSize || j >= nGridY0+nGridH*nGridSize
						|| ((i-px)*(i-px)+(j-py)*(j-py) > nCursorR*nCursorR))
					{
						continue;
					}
					ii = (j-nGridY0) / nGridSize;			//行数
					jj = (i-nGridX0) / nGridSize;			//列数
					if(qPointCursor == Brush)
					{
						*(pbGridAllPoint + ii * nGridW + jj) = true;
					}
					else if(qPointCursor == Erase)
					{
						*(pbGridAllPoint + ii * nGridW + jj) = false;
					}
				}
			}

			px += xlen;
			py += ylen;
			n ++;
			if(n > k)
				break;
		}
	}

	//QPoint point = this->mapFromGlobal(this->cursor().pos());
	//if(qPointCursor == Point)
	//{
	//	int px = point.x();
	//	int py = point.y();
	//	if(px > nGridX0 && py > nGridY0
	//		&& px < nGridX0 + nGridW*nGridSize
	//		&& py < nGridY0 + nGridH*nGridSize)
	//	{
	//		int i = (py - nGridY0) / nGridSize;		//行数
	//		int j = (px - nGridX0) / nGridSize;		//列数
	//		*(pbGridAllPoint + i * nGridW + j) = true;
	//	}
	//}
	//else
	//{
	//	//计算包含于圆圈内有像素点
	//	int i,j,ii,jj;
	//	for(i = point.x()-nCursorR; i <= point.x()+nCursorR; i += nGridSize)
	//	{
	//		for(j = point.y()-nCursorR; j <= point.y()+nCursorR; j += nGridSize)
	//		{
	//			if(i <= nGridX0 || j <= nGridY0
	//				|| i >= nGridX0+nGridW*nGridSize || j >= nGridY0+nGridH*nGridSize
	//				|| ((i-point.x())*(i-point.x())+(j-point.y())*(j-point.y()) > nCursorR*nCursorR))
	//			{
	//				continue;
	//			}
	//			ii = (j-nGridY0) / nGridSize;			//行数
	//			jj = (i-nGridX0) / nGridSize;			//列数
	//			if(qPointCursor == Brush)
	//			{
	//				*(pbGridAllPoint + ii * nGridW + jj) = true;
	//			}
	//			else if(qPointCursor == Erase)
	//			{
	//				*(pbGridAllPoint + ii * nGridW + jj) = false;
	//			}
	//		}
	//	}
	//}
}

void QDrawArea::clearGrid()
{
	memset(pbGridAllPoint,0,nGridW*nGridH);
}

void QDrawArea::zoomInGrid()
{
	if(nGridSize < 30)
	{
		nGridSize ++;
		drawGrid();
	}
}

void QDrawArea::zoomOutGrid()
{
	if(nGridSize > 1)
	{
		nGridSize --;
		drawGrid();
	}
}

void QDrawArea::rotateLeftGrid()
{
	int i,j,ii,jj;
	int gw = nGridH;
	int gh = nGridW;
	bool *rp = new bool[gw*gh];				//Rotated point.

	//rotate data
	ii = gh - 1;								//旋转后Grid的行
	jj = 0;									//旋转后Grid的列
	for(i = 0; i < nGridH; i ++)		//按旋转前的Grid遍历
	{
		for(j = 0; j < nGridW; j ++)
		{
			*(rp + ii*gw + jj) = *(pbGridAllPoint + i*nGridW + j);
			ii --;							//rp的列遍历
		}
		jj ++;
		ii = gh - 1;
	}
	nGridW = gw;
	nGridH = gh;
	memcpy(pbGridAllPoint,rp,nGridW*nGridH);

	delete[] rp;

	drawGrid();
}

void QDrawArea::rotateRightGrid()
{
	int i,j,ii,jj;
	int gw = nGridH;
	int gh = nGridW;
	bool *rp = new bool[gw*gh];				//Rotated point.

	//rotate data
	ii = 0;									//旋转后Grid的行
	jj = gw - 1;								//旋转后Grid的列
	for(i = 0; i < nGridH; i ++)		//按旋转前的Grid遍历
	{
		for(j = 0; j < nGridW; j ++)
		{
			*(rp + ii*gw + jj) = *(pbGridAllPoint + i*nGridW + j);
			ii ++;							//rp的列遍历
		}
		jj --;
		ii = 0;
	}
	nGridW = gw;
	nGridH = gh;
	memcpy(pbGridAllPoint,rp,nGridW*nGridH);
	delete[] rp;

	drawGrid();
}

void QDrawArea::flipHorizontalGrid()
{
	int i,j,k;
	bool *fp = new bool[nGridH*nGridW];	//Fliped point.

	//Flip data
	for(i = 0; i < nGridH; i ++)		//按翻转前的Grid遍历
	{
		for(j = 0; j < nGridW; j ++)
		{
			k = nGridW - 1 - j;
			*(fp + i*nGridW + k) = *(pbGridAllPoint + i*nGridW + j);
		}
	}
	memcpy(pbGridAllPoint,fp,nGridW*nGridH);
	delete[] fp;

	drawGrid();
}

void QDrawArea::flipVerticalGrid()
{
	int i,j,k;
	bool *fp = new bool[nGridH*nGridW];	//Fliped point.

	//Flip data
	k = nGridH - 1;
	for(i = 0; i < nGridH; i ++)		//按翻转前的Grid遍历
	{
		for(j = 0; j < nGridW; j ++)
		{
			*(fp + k*nGridW + j) = *(pbGridAllPoint + i*nGridW + j);
		}
		k --;
	}
	memcpy(pbGridAllPoint,fp,nGridW*nGridH);
	delete[] fp;

	drawGrid();
}

//*************************************************************************************//

namespace
{
	QPoint lastPoint;
	QPoint thisPoint;
	QPoint rectPoint;
}
void QDrawArea::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.drawImage(0,0,*qImg);

	if(bRectText)
		p.drawImage(rectPoint,*rectImg);
}

//Mouse
void QDrawArea::mousePressEvent(QMouseEvent *e)
{
	if(e->button() & Qt::LeftButton)
	{
		thisPoint = e->pos();
		lastPoint = thisPoint;
		bMouseDrawing = true;						//mouse is moving for drawing or moving the grid.

		if(qPointCursor == Arrow)
		{
			QPoint point = thisPoint;
			int px = point.x();
			int py = point.y();
			if(px > nGridX0 && py > nGridY0
				&& px < nGridX0 + nGridW*nGridSize
				&& py < nGridY0 + nGridH*nGridSize)
			{
				int i = (py - nGridY0) / nGridSize;		//行数
				int j = (px - nGridX0) / nGridSize;		//列数
				*(pbGridAllPoint + i * nGridW + j) = !(*(pbGridAllPoint + i * nGridW + j));
			}
			drawGrid();
		}
		else if(qPointCursor == Text)
		{
			if(!bCreatedFontArea)
			{
				rectPoint = thisPoint;			//set "rectPoint" as the start point of drawing rectImg. 
				lastPoint = thisPoint;			//remember the rect start point.
			}
			else
			{
				//if((thisPoint-pdragPos).manhattanLength() > QApplication::startDragDistance())
				//这里假定在文本框外单击时，就认要执行 drag and drop operation，
				//所以不放在 mouseMoveEvent 判定 startDragDistance()。

				//it is drag event, and start the drag evet.
				//the evnet order: start drag evet -> dragEnterEvent - > dragMoveEvnet -> dragLeaveEvent or dropEvent.
				QDrag *drag = new QDrag(this);
				QMimeData *mimeData = new QMimeData();
				mimeData->setData("DrawArea_Editor",		//set the type of the data to be draged,"DrawArea/Editor". 
					QByteArray::number(thisPoint.x())		//pass the point with string type.
					+ " " 
					+ QByteArray::number(thisPoint.y()));
				drag->setMimeData(mimeData);

				QPixmap pixmap(qEdit->size());
				pixmap.fill(QColor(0,0,0,0));
				qEdit->render(&pixmap);
				drag->setPixmap(pixmap);			//when dragMove, it shows pixmap.
				drag->setHotSpot(thisPoint-qEdit->pos());

				drag->exec(Qt::MoveAction);		//starting drag and drop operation only can be in Qt::MoveAction.
				delete drag;
				delete mimeData;

				bMouseDrawing = false;			//when it is drag and drop operation, the mouse is to be not moving.
			}
		}
	}
	else if(e->button() & Qt::RightButton)
	{
		//pPopMenu->exec(e->pos());				//pos():relative to its parent object(here it's QDrawArea).
		pPopMenu->exec(e->globalPos());			//globalPos():relative to the screen.
	}
	else if (e->button() & Qt::MidButton)
	{
		thisPoint = e->pos();
		lastPoint = thisPoint;
		bMouseMoving = true;

		this->setCursor(Qt::ClosedHandCursor);
	}
}

void QDrawArea::mouseMoveEvent(QMouseEvent *e)
{
	thisPoint = e->pos();
	if (bMouseDrawing)
	{
		//for drawing graph
		if(qPointCursor == Point
			||qPointCursor == Brush
			||qPointCursor == Erase)
		{
			calcGrid(thisPoint,lastPoint);
			lastPoint = thisPoint;
			drawGrid();
		}
		else if(qPointCursor == Text)
		{
			//judge that whether the two point consist a rect.
			if((thisPoint - lastPoint).manhattanLength() > 30)
			{
				if(bRectText)
					delete rectImg;
				bRectText = true;
				int rectw = thisPoint.x() - lastPoint.x();
				int recth = thisPoint.y() - lastPoint.y();
				if(rectw < 0)
				{
					rectPoint.setX(thisPoint.x());
					rectw = -rectw;
				}
				if(recth < 0)
				{
					rectPoint.setY(thisPoint.y());
					recth = -recth;
				}
				rectImg = new QImage(rectw,recth,QImage::Format_ARGB32_Premultiplied);
				rectImg->fill(QColor(0,0,255,30));
				update();

			}
			else
			{
				if(bRectText)
				{
					delete rectImg;
					bRectText = false;
				}
			}
		}
	}
	else if (bMouseMoving)
	{
		//for moving graph
		nGridX0 += thisPoint.x() - lastPoint.x();
		nGridY0 += thisPoint.y() - lastPoint.y();
		lastPoint = thisPoint;
		drawGrid();
	}
	else
	{
		//move the editor.
		if(qPointCursor == Text && bCreatedFontArea)
		{
			QRect rect = qEdit->geometry();
			if(!rect.contains(thisPoint))
			{
				this->setCursor(Qt::SizeAllCursor);
			}
		}
	}

	QString str = QString("[%1 , %2]  Size: %3")
						.arg(thisPoint.x(),-4)
						.arg(thisPoint.y(),-4)
						.arg(nGridSize);
	emit Singals_ShowPoint(str);	//Show point coordinate;
}

void QDrawArea::mouseReleaseEvent(QMouseEvent *e)
{
 	if(e->button() & Qt::LeftButton)
	{
		thisPoint = e->pos();

		//for drawing graph
		if(qPointCursor == Point
			|| qPointCursor == Brush
			|| qPointCursor == Erase)
		{
			calcGrid(thisPoint,lastPoint);
			lastPoint = thisPoint;
			drawGrid();
		}
		else if(qPointCursor == Text)
		{
			if(bRectText)
			{
				//create the font area via a rect.
				delete rectImg;
				bRectText = false;
				if(thisPoint.x() < lastPoint.x())
				{
					int t = thisPoint.x();
					thisPoint.setX(lastPoint.x());
					lastPoint.setX(t);
				}
				if(thisPoint.y() < lastPoint.y())
				{
					int t = thisPoint.y();
					thisPoint.setY(lastPoint.y());
					lastPoint.setY(t);
				}
				createFontArea(lastPoint,thisPoint);
			}
		}

		bMouseDrawing = false;
	}
	else if (e->button() & Qt::MidButton)
	{
		bMouseMoving = false;
		this->setCursor(qCursor);
	}
}

void QDrawArea::mouseDoubleClickEvent(QMouseEvent *e)
{
	if(e->button() & Qt::LeftButton)
	{
		if(qPointCursor == Text)
		{
			createFontArea();
		}
	}
}

void QDrawArea::wheelEvent(QWheelEvent *e)
{
	e->accept();

	if (e->delta() > 0)
	{
		zoomInGrid();
	}
	else
	{
		zoomOutGrid();
	}
	QString str = QString("[%1 , %2]  Size: %3")
		.arg(thisPoint.x(), -4)
		.arg(thisPoint.y(), -4)
		.arg(nGridSize);
	emit Singals_ShowPoint(str);
}

//drag and drop.
void QDrawArea::dragEnterEvent(QDragEnterEvent *e)
{
	if(e->mimeData()->hasFormat("DrawArea_Editor"))
	{
		//if(e->source() == this)				//there is unnecessary to judge whether (source() == this)
		//{
			e->setDropAction(Qt::MoveAction);	//override the proposed action with Qt::MoveAction.
			e->accept();
			qEdit->hide();						//when draging, hiding the original qEdit.
		//}
	}
	else
	{
		e->ignore();
	}
}

void QDrawArea::dropEvent(QDropEvent *e)
{
	if(e->mimeData()->hasFormat("DrawArea_Editor"))
	{
		const QMimeData *mimeData = e->mimeData();
		QByteArray byteAr = mimeData->data("DrawArea_Editor");	//get the data who's type is "DrawArea/Editer".
		QList<QByteArray> ListByteAr = byteAr.split(' ');		//get the point data, which was splited by ' '.
		QPoint point;
		if(ListByteAr.size() == 2)
		{
			point.rx() = ListByteAr.first().toInt();
			point.ry() = ListByteAr.last().toInt();
		}
		point = e->pos() - point;

		QRect rect = qEdit->geometry();							//move the editor.
		rect.moveLeft(rect.x() + point.x());
		rect.moveTop(rect.y() + point.y());
		qEdit->setGeometry(rect);				//it just need to move, so it's no need to create new qEdit.
		qEdit->show();

		e->acceptProposedAction();
	}
	else
	{
		e->ignore();
	}
}
