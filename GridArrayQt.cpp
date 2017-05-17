#include "GridArrayQt.h"
#include "ui_GridArrayQt.h"


GridArrayQt::GridArrayQt(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GridArrayQt)
{
    ui->setupUi(this);

	//int t1,t2;
	//QPoint p1(0,7),p2(0,4);
	//t1 = QPoint::dotProduct(p1, p2);
	//t2 = 5;
	//QString str = QString("%1, %2").arg(t1).arg(t2);
	//QMessageBox::information(this,"test",str);

	SetInit();

	pDrawArea->setFocus();
}

GridArrayQt::~GridArrayQt()
{
	delete qCursorBE;
	delete qPixmapBE;
}

void GridArrayQt::createDrawArea()
{
	//DrawArea
	nLoadDrawArea = 1;
	pDrawArea = new QDrawArea(nGridW,nGridH,this);
	pDrawArea->setAutoFillBackground(true);
	pDrawArea->setBackgroundRole(QPalette::Dark);
	setCentralWidget(pDrawArea);			//将DrawArea作为显示区，DrawArea继承于QWidget
	
	//Cursor
	QCursor qCursor;
	switch (qPointCursor)
	{
	case Arrow:
		qCursor = QCursor(Qt::CrossCursor);
		break;
	case Point:
		qCursor = QCursor(Qt::CrossCursor);
		break;
	case Brush:
		qCursor = *qCursorBE;
		break;
	case Erase:
		qCursor = *qCursorBE;
		break;
	case Text:
		qCursor = QCursor(Qt::IBeamCursor);
		break;
	}
	pDrawArea->setPointCursor(qPointCursor,qCursor);	//初始化qPointCursor等数据
	pDrawArea->setCursorR(nCursorR);
	createFont();

	//connection
	connect(pDrawArea,&QDrawArea::Singals_ShowPoint,
		[&](QString str){statusMsg2->setText(str);});
}

void GridArrayQt::createCursor()
{
	//Cursor
	qPixmapBE = new QPixmap(2*nCursorR+1,2*nCursorR+1);
	qPixmapBE->fill(Qt::transparent);
	QPainter p(qPixmapBE);
	p.setPen(Qt::black);
	p.drawEllipse(QPoint(nCursorR,nCursorR),nCursorR,nCursorR);
	p.drawLine(nCursorR,nCursorR-10,nCursorR,nCursorR+10);
	p.drawLine(nCursorR-10,nCursorR,nCursorR+10,nCursorR);
	qCursorBE = new QCursor(*qPixmapBE);
}

void GridArrayQt::setupMenu()
{
	//Menu
#if(1)
	qMenu_File = new QMenu(tr("File &(F)"),this);
	qFile_New = new QAction(QIcon(":/icon/images/file/new.png"),tr("New"),this);
	qFile_Open = new QAction(QIcon(":/icon/images/file/open.png"),tr("Open"),this);
	qFile_Save = new QAction(QIcon(":/icon/images/file/save.png"),tr("Save"),this);
	qFile_Close = new QAction(QIcon(":/icon/images/file/close.png"),tr("Close"),this);
	connect(qFile_New,&QAction::triggered,
		[=]{
			if(nLoadDrawArea)
			{
				delete pDrawArea;
			}
			nGridW = ui->lineEdit_GridW->text().toInt();
			nGridH = ui->lineEdit_GridH->text().toInt();

			createDrawArea();
			setCtrlEnable(true);
	});
	connect(qFile_Close,&QAction::triggered,		
		[=]{
			if(nLoadDrawArea)
			{
				delete pDrawArea;
				nLoadDrawArea = 0;
				setCtrlEnable(false);
			}
	});
	connect(qFile_Open,&QAction::triggered,this,&GridArrayQt::openFile);
	connect(qFile_Save,&QAction::triggered,this,&GridArrayQt::saveGaqFile);
	qMenu_File->addAction(qFile_New);
	qMenu_File->addAction(qFile_Open);
	qMenu_File->addAction(qFile_Save);
	qMenu_File->addAction(qFile_Close);
#endif

#if(1)
	qMenu_Edit = new QMenu(tr("Edit &(E)"),this);
	qEdit_Clear = new QAction(QIcon(":/icon/images/file/clear.png"),tr("Clear"),this);
	qEdit_ZoomIn = new QAction(QIcon(":/icon/images/file/zoom_in.png"),tr("Zoom In"),this);
	qEdit_ZoomOut = new QAction(QIcon(":/icon/images/file/zoom_out.png"),tr("Zoom Out"),this);
	qEdit_RotateLeft = new QAction(QIcon(":/icon/images/file/rotateL.png"),tr("Rotate Left"),this);
	qEdit_RotateRight = new QAction(QIcon(":/icon/images/file/rotateR.png"),tr("Rotate Right"),this);
	qEdit_FlipH = new QAction(QIcon(":/icon/images/file/flipH.png"),tr("Flip Horizontal"),this);
	qEdit_FlipV = new QAction(QIcon(":/icon/images/file/flipV.png"),tr("Flip Verticalt"),this);
	qMenu_Edit->addAction(qEdit_Clear);
	qMenu_Edit->addAction(qEdit_ZoomIn);
	qMenu_Edit->addAction(qEdit_ZoomOut);
	qMenu_Edit->addAction(qEdit_RotateLeft);
	qMenu_Edit->addAction(qEdit_RotateRight);
	qMenu_Edit->addAction(qEdit_FlipH);
	qMenu_Edit->addAction(qEdit_FlipV);
		//注意：pDrawArea,&QDrawArea::drawClear),这样连接时，新建pDrawArea就会失去联系
	connect(qEdit_Clear,&QAction::triggered,[=]{pDrawArea->clearGrid();pDrawArea->drawGrid();});
	connect(qEdit_ZoomIn,&QAction::triggered,[=]{pDrawArea->zoomInGrid();});
	connect(qEdit_ZoomOut,&QAction::triggered,[=]{pDrawArea->zoomOutGrid();});
	connect(qEdit_RotateLeft,&QAction::triggered,[=]{pDrawArea->rotateLeftGrid();});
	connect(qEdit_RotateRight,&QAction::triggered,[=]{pDrawArea->rotateRightGrid();});
	connect(qEdit_FlipH,&QAction::triggered,[=]{pDrawArea->flipHorizontalGrid();});
	connect(qEdit_FlipV,&QAction::triggered,[=]{pDrawArea->flipVerticalGrid();});
#endif

#if(1)
	qView_LangCn = new QAction(tr("Chinese"),this);
	qView_LangEn = new QAction(tr("English"),this);
	qView_LangCn->setStatusTip(tr("It need to be restart"));
	qView_LangEn->setStatusTip(tr("It need to be restart"));
	QActionGroup *ag = new QActionGroup(this);
	ag->addAction(qView_LangCn);
	ag->addAction(qView_LangEn);
	qView_LangCn->setCheckable(true);
	qView_LangEn->setCheckable(true);
	if(bLanguage)
	{
		qView_LangCn->setChecked(true);
	}
	else
	{
		qView_LangEn->setChecked(true);
	}
	qMenu_View = new QMenu(tr("View (&V)"),this);
	qMenu_View->addAction(qView_LangCn);
	qMenu_View->addAction(qView_LangEn);
	qMenu_View->addSeparator();
	qMenu_View->addAction(ui->dockW_Ctrl->toggleViewAction());
	connect(qView_LangCn,&QAction::triggered,[=]{bLanguage = true;});
	connect(qView_LangEn,&QAction::triggered,[=]{bLanguage = false;});

	qMenu_Help = new QMenu(tr("Help &(H)"),this);
	qHelp_About = new QAction(tr("About"),this);
	qMenu_Help->addAction(qHelp_About);
	connect(qHelp_About,&QAction::triggered,
		[=]{
		QMessageBox::information(this, tr("About"), tr(_GRIDARRAY_QT_VERSION_));
	});
	QMenuBar *qMenuBar = menuBar();
	qMenuBar->addMenu(qMenu_File);
	qMenuBar->addMenu(qMenu_Edit);
	qMenuBar->addMenu(qMenu_View);
	qMenuBar->addMenu(qMenu_Help);
#endif
}

void GridArrayQt::setupToolbar()
{
	//ToolBar
#if(1)
	qToolBar = addToolBar(tr("File Bar"));		//File toolbar.
	qToolBar->addAction(qFile_New);
	qToolBar->addAction(qFile_Open);
	qToolBar->addAction(qFile_Save);
	qToolBar->addAction(qFile_Close);
	qToolBar->addSeparator();
	qToolBar->addAction(qEdit_Clear);
	qToolBar->addAction(qEdit_ZoomIn);
	qToolBar->addAction(qEdit_ZoomOut);
	qToolBar->addAction(qEdit_RotateLeft);
	qToolBar->addAction(qEdit_RotateRight);
	qToolBar->addAction(qEdit_FlipH);
	qToolBar->addAction(qEdit_FlipV);

	qToolBar_Tool = new QToolBar(tr("Tools Bar"),this);	//Tools toolbar.
	qTool_Arrow = new QAction(QIcon(":/icon/images/tools/arrow.png"),tr("Arrow"),this);
	qTool_Point = new QAction(QIcon(":/icon/images/tools/point.png"),tr("Point"),this);
	qTool_Brush = new QAction(QIcon(":/icon/images/tools/brush.png"),tr("Brush"),this);
	qTool_Erase = new QAction(QIcon(":/icon/images/tools/erase.png"),tr("Erase"),this);
	qTool_Text = new QAction(QIcon(":/icon/images/tools/text.png"),tr("Text"),this);
	connect(qTool_Arrow,&QAction::triggered,
		[=]{
			qPointCursor = Arrow;
			statusMsg1->setText(tr("Arrow"));
			pDrawArea->setPointCursor(qPointCursor,QCursor(Qt::ArrowCursor));
	});
	connect(qTool_Point,&QAction::triggered,
		[=]{
			qPointCursor = Point;
			statusMsg1->setText(tr("Point"));
			pDrawArea->setPointCursor(qPointCursor,QCursor(Qt::CrossCursor));
	});
	connect(qTool_Brush,&QAction::triggered,
		[=]{
			qPointCursor = Brush;
			statusMsg1->setText(tr("Brush"));
			pDrawArea->setPointCursor(qPointCursor,*qCursorBE);
			pDrawArea->setCursorR(nCursorR);
	});
	connect(qTool_Erase,&QAction::triggered,
		[=]{
			qPointCursor = Erase;
			statusMsg1->setText(tr("Erase"));
			pDrawArea->setPointCursor(qPointCursor,*qCursorBE);
			pDrawArea->setCursorR(nCursorR);
	});
	connect(qTool_Text,&QAction::triggered,
		[=]{
			qPointCursor = Text;
			statusMsg1->setText(tr("Text"));
			pDrawArea->setPointCursor(qPointCursor,QCursor(Qt::IBeamCursor));
			createFont();
	});
	qToolBar_Tool->addAction(qTool_Arrow);
	qToolBar_Tool->addAction(qTool_Point);
	qToolBar_Tool->addAction(qTool_Brush);
	qToolBar_Tool->addAction(qTool_Erase);
	qToolBar_Tool->addAction(qTool_Text);
	qToolBar_Tool->setOrientation(Qt::Vertical);
	qToolBar_Tool->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
	QMainWindow::addToolBar(Qt::LeftToolBarArea,qToolBar_Tool);

	qMenu_View->addAction(qToolBar->toggleViewAction());
	qMenu_View->addAction(qToolBar_Tool->toggleViewAction());
#endif
}

void GridArrayQt::setupDockwidget()
{
	//FileInfo group box
#if(1)
	int h1,h2,h3;
	h1 = 105;
	ui->groupFileInfo->setMaximumHeight(h1);
	ui->lineEdit_GridW->setText(QString("%1").arg(nGridW));
	ui->lineEdit_GridH->setText(QString("%1").arg(nGridH));
	connect(ui->hSlider_GridColor,&QSlider::valueChanged,
		[=](int value){pDrawArea->setGridGray(value); });
	ui->hSlider_GridColor->setRange(0,255);
	ui->hSlider_GridColor->setInvertedAppearance(true);
	ui->hSlider_GridColor->setValue(200);
#endif

	//Load group box
#if(1)
	h2 = 210;
	ui->groupLoadPic->setMaximumHeight(h2);
	connect(ui->btn_UnloadImg,&QPushButton::clicked,[=]{pDrawArea->unloadPicture();pDrawArea->drawGrid();});
	connect(ui->btn_LoadPic,&QPushButton::clicked,[=]{this->openFile(FILE_PIC);});
	connect(ui->btn_LoadAr,&QPushButton::clicked,[=]{this->openFile(FILE_TXT);});
#define setIMG_GRID()		pDrawArea->setImg2Grid(nImg2Grid_Type,nImg2Grid_Value)
	connect(ui->hSlider_Gray,&QSlider::valueChanged,
		[=](int value){
			nImg2Grid_Value = value;
			nImg2Grid_Type = IMG2GRID_Gray;
			setIMG_GRID();
			ui->radio_Gray->setChecked(true);
	});
	connect(ui->hSlider_Red,&QSlider::valueChanged,
		[=](int value){
			nImg2Grid_Value = value;
			nImg2Grid_Type = IMG2GRID_Red;
			setIMG_GRID();
			ui->radio_Red->setChecked(true);
	});
	connect(ui->hSlider_Green,&QSlider::valueChanged,
		[=](int value){
			nImg2Grid_Value = value;
			nImg2Grid_Type = IMG2GRID_Green;
			setIMG_GRID();
			ui->radio_Green->setChecked(true);
	});
	connect(ui->hSlider_Blue,&QSlider::valueChanged,
		[=](int value){
			nImg2Grid_Value = value;
			nImg2Grid_Type = IMG2GRID_Blue;
			setIMG_GRID();
			ui->radio_Blue->setChecked(true);
	});
	connect(ui->radio_Gray,&QRadioButton::toggled,				//toggled:切换信号
		[=](bool checked){
			if(checked)
			{
				nImg2Grid_Value = ui->hSlider_Gray->value();
				nImg2Grid_Type = IMG2GRID_Gray;
				setIMG_GRID();
			}
	});
	connect(ui->radio_Red,&QRadioButton::toggled,
		[=](bool checked){
			if(checked)
			{
				nImg2Grid_Value = ui->hSlider_Red->value();
				nImg2Grid_Type = IMG2GRID_Red;
				setIMG_GRID();
			}
	});
	connect(ui->radio_Green,&QRadioButton::toggled,
		[=](bool checked){
			if(checked)
			{
				nImg2Grid_Value = ui->hSlider_Green->value();
				nImg2Grid_Type = IMG2GRID_Green;
				setIMG_GRID();
			}
	});
	connect(ui->radio_Blue,&QRadioButton::toggled,
		[=](bool checked){
			if(checked)
			{
				nImg2Grid_Value = ui->hSlider_Blue->value();
				nImg2Grid_Type = IMG2GRID_Blue;
				setIMG_GRID();
			}
	});
	ui->hSlider_Gray->setRange(0,255);
	ui->hSlider_Red->setRange(0,255);
	ui->hSlider_Green->setRange(0,255);
	ui->hSlider_Blue->setRange(0,255);
	switch (nImg2Grid_Type)
	{
	case IMG2GRID_Gray:
		ui->hSlider_Gray->setValue(nImg2Grid_Value);
		break;
	case IMG2GRID_Red:
		ui->hSlider_Red->setValue(nImg2Grid_Value);
		break;
	case IMG2GRID_Green:
		ui->hSlider_Green->setValue(nImg2Grid_Value);
		break;
	case IMG2GRID_Blue:
		ui->hSlider_Blue->setValue(nImg2Grid_Value);
		break;
	default:
		ui->radio_Gray->setChecked(true);
		break;
	}
#endif

	//Text group box
#if(1)
	h3 = 110;
	ui->groupText->setMaximumHeight(h3);
	ui->fontComboBox->setCurrentText(sFontName);
	QStringList str;
	str<<"6" <<"8" <<"9" <<"10" <<"11" <<"12"
		<<"14"<<"18"<<"20"<<"24" <<"30" <<"36"
		<<"48"<<"60"<<"72";
	gcombox_FontSize = new GCombox(this);
	gcombox_FontSize->addItems(str);
	ui->gridLayout_5->addWidget(gcombox_FontSize,1,1);		//add the new combox to text groupbox.
	gcombox_FontSize->setCurrentText(QString("%1").arg(nFontSize));

	QString btnStr = "QPushButton{\
						border-radius: 5px;\
						border: 2px outset gray;}\
					 QPushButton:pressed{\
						background-color: rgba(90,230,230,120);\
						border-style: inset;}\
					 QPushButton:checked{\
						background-color: rgba(90,230,230,120);\
						border-style: inset;}";
	ui->btn_Accept->setIcon(QIcon(":/icon/images/font/ok.png"));
	ui->btn_Accept->setIconSize(QSize(22,22));
	ui->btn_Accept->setFixedSize(30,30);
	ui->btn_Accept->setStyleSheet(btnStr);
	ui->btn_Bold->setCheckable(true);
	ui->btn_Bold->setChecked(bFontBold);
	ui->btn_Bold->setIcon(QIcon(":/icon/images/font/bold.png"));
	ui->btn_Bold->setIconSize(QSize(22,22));
	ui->btn_Bold->setFixedSize(30,30);
	ui->btn_Bold->setStyleSheet(btnStr);
	ui->btn_Italic->setCheckable(true);
	ui->btn_Italic->setChecked(bFontItalic);
	ui->btn_Italic->setIcon(QIcon(":/icon/images/font/italic.png"));
	ui->btn_Italic->setIconSize(QSize(22,22));
	ui->btn_Italic->setFixedSize(30,30);
	ui->btn_Italic->setStyleSheet(btnStr);
	ui->btn_Underline->setCheckable(true);
	ui->btn_Underline->setChecked(bFontUnderline);
	ui->btn_Underline->setIcon(QIcon(":/icon/images/font/underline.png"));
	ui->btn_Underline->setIconSize(QSize(22,22));
	ui->btn_Underline->setFixedSize(30,30);
	ui->btn_Underline->setStyleSheet(btnStr);
	connect(ui->btn_Accept,&QPushButton::clicked,[=]{pDrawArea->convertText();pDrawArea->drawGrid();});
	connect(ui->fontComboBox,&QFontComboBox::currentFontChanged,
		[=]{sFontName = ui->fontComboBox->currentText();createFont();});
	connect(gcombox_FontSize,&GCombox::currentTextChanged,
		[=]{nFontSize = gcombox_FontSize->currentText().toInt();createFont();});
	connect(ui->btn_Bold,&QPushButton::toggled,
		[=]{bFontBold = ui->btn_Bold->isChecked();createFont();});
	connect(ui->btn_Italic,&QPushButton::toggled,
		[=]{bFontItalic = ui->btn_Italic->isChecked();createFont();});
	connect(ui->btn_Underline,&QPushButton::toggled,
		[=]{bFontUnderline = ui->btn_Underline->isChecked();createFont();});
#endif

	//set dockWidget
	ui->groupBox->setMaximumHeight(h1+h2+h3);
	ui->dockW_Ctrl->setWindowTitle(tr("Control Box"));
	ui->dockW_Ctrl->setMaximumWidth(270);
	ui->dockW_Ctrl->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	QMainWindow::addDockWidget(Qt::LeftDockWidgetArea,ui->dockW_Ctrl);
	ui->dockW_Ctrl->setWidget(ui->groupBox);
}

void GridArrayQt::setCtrlEnable(bool value)
{
	qFile_Save->setEnabled(value);
	qFile_Close->setEnabled(value);
	//qMenu_Edit->setEnabled(value);
	qEdit_Clear->setEnabled(value);
	qEdit_ZoomIn->setEnabled(value);
	qEdit_ZoomOut->setEnabled(value);
	qEdit_RotateLeft->setEnabled(value);
	qEdit_RotateRight->setEnabled(value);
	qEdit_FlipH->setEnabled(value);
	qEdit_FlipV->setEnabled(value);

	qToolBar_Tool->setEnabled(value);

	ui->hSlider_GridColor->setEnabled(value);
	ui->groupLoadPic->setEnabled(value);
	ui->groupText->setEnabled(value);
}

void GridArrayQt::createFont()
{
	QFont *font = new QFont(sFontName);
	font->setPointSize(nFontSize);
	font->setBold(bFontBold);
	font->setItalic(bFontItalic);
	font->setUnderline(bFontUnderline);
	pDrawArea->setTextFont(*font);
	delete font;
}


// 初始化设置
void GridArrayQt::SetInit()
{
	this->setFocusPolicy(Qt::StrongFocus);	//receive keyboard event.
	this->setWindowTitle(tr("GridArray"));
	this->setWindowIcon(QIcon(":icon/images/icon64.png"));
	this->setAcceptDrops(true);				//set drap and drop.

	//Data
	if(!loadSettings())
	{
		nGridW = 128;
		nGridH = 64;
		qPointCursor = QPointCursor::Point;
		nCursorR = 20;
		bLanguage = true;
	}

	createCursor();

	createDrawArea();

	setupMenu();

	setupToolbar();

	setupDockwidget();

	//Status Bar
	statusMsg1 = new QLabel(tr("Ready"));
	statusMsg1->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	statusMsg1->setMinimumWidth(150);
	QPoint point = this->mapFromGlobal(this->cursor().pos());
	statusMsg2 = new QLabel(QString("[ %1 , %2 ]").arg(point.x()).arg(point.y()));
	statusMsg2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	statusMsg2->setMinimumWidth(150);
	QStatusBar *qStatusBar = statusBar(); 
	qStatusBar->addWidget(statusMsg1);
	qStatusBar->addWidget(statusMsg2);
	qStatusBar->setAutoFillBackground(true);
}


bool GridArrayQt::loadSettings()
{
	QSettings qSettings("Settings.ini",QSettings::IniFormat);

	qSettings.setIniCodec("utf-8");
	nGridW = qSettings.value("Grid/GridWidth",128).toInt();
	nGridH = qSettings.value("Grid/GridHeight",64).toInt();

	nCursorR = qSettings.value("Cursor/CursorR",20).toInt();
	qPointCursor = (QPointCursor)qSettings.value("Cursor/PointCursor",(int)QPointCursor::Point).toInt();
	
	nImg2Grid_Type = qSettings.value("Picture/Type",IMG2GRID_Gray).toInt();
	nImg2Grid_Value = qSettings.value("Picture/Value",0).toInt();

	sFontName = qSettings.value("Font/FontName").toString();
	nFontSize = qSettings.value("Font/FontSize").toInt();
	bFontBold = qSettings.value("Font/FontBold").toBool();
	bFontItalic = qSettings.value("Font/FontItalic").toBool();
	bFontUnderline= qSettings.value("Font/FontUnderline").toBool();

	bLanguage = qSettings.value("Language/Lang").toBool();

	return true;
}

void GridArrayQt::saveSettings()
{
	QSettings qSettings("Settings.ini",QSettings::IniFormat);
	qSettings.setIniCodec("utf-8");
	if(qSettings.isWritable())
	{
		qSettings.clear();

		qSettings.beginGroup("Grid");
		qSettings.setValue("GridWidth",nGridW);
		qSettings.setValue("GridHeight",nGridH);
		qSettings.endGroup();

		qSettings.beginGroup("Cursor");
		qSettings.setValue("CursorR",nCursorR);
		qSettings.setValue("PointCursor",qPointCursor);
		qSettings.endGroup();

		qSettings.beginGroup("Picture");
		qSettings.setValue("Type",nImg2Grid_Type);
		qSettings.setValue("Value",nImg2Grid_Value);
		qSettings.endGroup();

		qSettings.beginGroup("Font");
		qSettings.setValue("FontName",sFontName);
		qSettings.setValue("FontSize",nFontSize);
		qSettings.setValue("FontBold",bFontBold);
		qSettings.setValue("FontItalic",bFontItalic);
		qSettings.setValue("FontUnderline",bFontUnderline);
		qSettings.endGroup();

		qSettings.beginReadArray("Language");
		qSettings.setValue("Lang",bLanguage);
		qSettings.endGroup();
	}
}

void GridArrayQt::saveGaqFile()
{
	QString str = QFileDialog::getSaveFileName(this,
												tr("Save File"),
												"array.gaq",
												tr("Gaq File(*.gaq)")); 
	if(!str.isEmpty())
	{
		QFile file(str);
		if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			writeGaqFile(&file);
		}
		else
		{
			QMessageBox::information(this,tr("Error"),tr("Open file failed!"));
		}
	}
}

bool GridArrayQt::readGaqFile(QFile *file)
{
	int imageSize;
	int fileHead;
	int gw,gh;
	bool *gp = nullptr;
	int fileEnd;

	fileHead = file->size();
	file->seek(fileHead - 4);			//seek function change the postion relative to start.
	file->read((char*)&imageSize,4);		//read the image size;
	file->seek(imageSize);				//skip image data.

	file->read((char*)&fileHead,4);		//read grid point data.
	file->read((char*)&gw,4);
	file->read((char*)&gh,4);
	gp = new bool[gw*gh];
	file->read((char*)gp,gw*gh);
	file->read((char*)&fileEnd,4);
	file->close();

	if(fileHead + fileEnd)				//check error.
	{
		return false;
	}

	//set drawArea grid data.
	pDrawArea->unloadPicture();			//unload the picture after read the gaq file.
	nGridW = gw;
	nGridH = gh;
	pDrawArea->setGridArea(gw,gh,gp);
	pDrawArea->drawGrid();

	delete[] gp;

	return true;
}

void GridArrayQt::writeGaqFile(QFile *file)
{
	int fileHead;
	int gw,gh;
	bool *gp;
	int fileEnd;

	//get data
	gw = pDrawArea->gridWidth();
	gh = pDrawArea->gridHeight();
	gp = pDrawArea->gridpAllPoint();

	//get picture.
	QImage image(gw,gh,QImage::Format_ARGB32);
	image.fill(Qt::gray);
	QPainter pp(&image);
	for(int i = 0; i < gh; i ++)
	{
		for(int j = 0; j <  gw; j ++)
		{
			if(*(gp+i*gw+j))
			{
				image.setPixel(j,i,qRgb(255,0,0));
			}
		}
	}
	//image.save("array.png","png",0);

	//get picture file data.
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);			//open for write buffer.
	image.save(&buffer,"png",0);				//write the image data to buffer in PNG format;
	int imgLen = ba.size();

	//wirte gaq file.
	fileHead = 4 + 4 + gw*gh;
	fileEnd = ~fileHead + 1;
	file->write(ba.data(),imgLen);				//write image data to gaq file.
	file->write((char*)&fileHead,4);
	file->write((char*)&gw,4);
	file->write((char*)&gh,4);
	file->write((char*)gp,gw*gh);
	file->write((char*)&fileEnd,4);
	file->write((char*)&imgLen,4);			//The end of gaq file is image size, 
											//convenient for skip the image data when read gaq file.
	file->close();
	//delete gp;	//不能释放内存，否则会与pDrawArea的pbGridAllPoint给释放了
}

void GridArrayQt::openFile(byte type = FILE_FALSE)
{
	QFileDialog fileDlg;
	QString str;
	fileDlg.setWindowTitle(tr("Open File"));
	fileDlg.setDirectory(QString("./Data"));
	fileDlg.setViewMode(QFileDialog::Detail);
	fileDlg.setFileMode(QFileDialog::ExistingFile);
	if(type == FILE_PIC)
	{
		fileDlg.setNameFilter(tr("Pic file(*.jpg *.png)"));
	}
	else if(type == FILE_TXT)
	{
		fileDlg.setNameFilter(tr("Txt file(*.txt)"));
	}
	else
	{
		fileDlg.setNameFilter(tr("All file(*.*);;\
								 Gaq file(*.gaq);;\
								 Pic file(*.jpg *.png);;\
								 Txt file(*.txt)"));
	}

	if(QDialog::Accepted == fileDlg.exec())
	{
		str = fileDlg.selectedFiles().at(0);
		if(!str.isEmpty())
		{
			if(!loadFile(judgeType(str),str))
			{
				statusMsg1->setText(tr("Error File"));
			}
		}
	}
}

byte GridArrayQt::judgeType(const QString &str)
{
	QString ext = QFileInfo(str).suffix();

	if(0 == QString::compare(ext,QString("gaq"),Qt::CaseInsensitive))	//return 0 when s1 equal s2.
	{
		return FILE_GAQ;
	}
	else if(0 == QString::compare(ext,QString("jpg"),Qt::CaseInsensitive)
		||0 == QString::compare(ext,QString("png"),Qt::CaseInsensitive))
	{
		return FILE_PIC;
	}
	else if(0 == QString::compare(ext,QString("txt"),Qt::CaseInsensitive))
	{
		return FILE_TXT;
	}
	else
	{
		return FILE_FALSE;
	}
}

bool GridArrayQt::loadFile(byte type, const QString &str)
{
	if(FILE_FALSE == type)				//check the right file.
	{
		QMessageBox::information(this,tr("Error"),tr("GridArray can't open this type file!"));		
		return false;
	}

	if(!nLoadDrawArea)					//check pDrawArea status.
	{
		createDrawArea();
		setCtrlEnable(true);
	}

	//judge the file type.
	QFile file;
	switch (type)
	{
	case FILE_GAQ:
		file.setFileName(str);
		if(file.open(QIODevice::ReadOnly))
		{
			if(!readGaqFile(&file))
				QMessageBox::information(this,tr("Error"),tr("The gaq file is invalid!"));
		}
		else
		{
			QMessageBox::information(this,tr("Error"),tr("Open gaq file failed!"));
		}
		break;


	case FILE_PIC:
		pDrawArea->loadPicture(str,nImg2Grid_Type,nImg2Grid_Value);
		break;


	case FILE_TXT:
		file.setFileName(str);
		if(file.open(QIODevice::ReadOnly))
		{
			pDrawArea->loadArray(&file);
		}
		else
		{
			QMessageBox::information(this,tr("Error"),tr("Open file Failed!"));
		}
		break;
	}
	return true;
}

//*************************************************************************************//

// 键盘事件
void GridArrayQt::keyPressEvent(QKeyEvent *e)
{
	int key = e->key();
	switch (key)
	{
	case Qt::Key_A:
		qTool_Arrow->trigger();
		break;
	case Qt::Key_P:
		qTool_Point->trigger();
		break;
	case Qt::Key_B:
		qTool_Brush->trigger();
		break;
	case Qt::Key_E:
		qTool_Erase->trigger();
		break;
	case Qt::Key_T:
		qTool_Text->trigger();
		break;

	case Qt::Key_BracketLeft:
		if(qPointCursor == Brush || qPointCursor == Erase)
		{
			if(nCursorR > 5)
				nCursorR --;
			delete qCursorBE;
			delete qPixmapBE;

			createCursor();

			pDrawArea->setPointCursor(qPointCursor,*qCursorBE);
			pDrawArea->setCursorR(nCursorR);
		}
		break;
	case Qt::Key_BracketRight:
		if(qPointCursor == Brush || qPointCursor == Erase)
		{
			if(nCursorR < 100)
				nCursorR ++;
			delete qCursorBE;
			delete qPixmapBE;

			createCursor();

			pDrawArea->setPointCursor(qPointCursor,*qCursorBE);
			pDrawArea->setCursorR(nCursorR);
		}
		break;
	}
}

void GridArrayQt::keyReleaseEvent(QKeyEvent *e)
{
	int key = e->key();
	switch (key)
	{
	}
}

void GridArrayQt::dragEnterEvent(QDragEnterEvent *e)
{
	if(e->mimeData()->hasText())
	{
		e->acceptProposedAction();			//set the drop action to be the proposed action.
	}	
}

void GridArrayQt::dropEvent(QDropEvent *e)
{
	QString str;
	str = e->mimeData()->text().mid(8);			//mid(8) : cat the first 8 char("file:///").
	judgeType(str);
	if(!loadFile(judgeType(str),str))
	{
		statusMsg1->setText(tr("Error File"));
	}
}

void GridArrayQt::closeEvent(QCloseEvent *e)
{
	saveSettings();
}





//=====================================================================================//
//=====================================================================================//


//class GCombox
GCombox::GCombox(QWidget *parent)
    :QComboBox(parent)
{
	this->setEditable(true);
}

void GCombox::wheelEvent(QWheelEvent *e)
{
	if(e->delta() > 0)
	{
		int value = this->currentText().toInt();
		value ++;
		this->setCurrentText(QString("%1").arg(value));
	}
	else if(e->delta() < 0)
	{
		int value = this->currentText().toInt();
		if(value > 1)
		{
			value --;
			this->setCurrentText(QString("%1").arg(value));
		}
	}
}
