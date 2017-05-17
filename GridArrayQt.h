#ifndef GRIDARRAYQT_H
#define GRIDARRAYQT_H

#include <QtWidgets/QMainWindow>

#include <QPainter>
#include <QImage>
#include <QStatusBar>
#include <QSettings>
#include <QMenu>
#include <QPoint>
#include <QBuffer>
#include <QByteArray>
#include <QMessageBox>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QAction>
#include <QLabel>
#include <QComboBox>
#include <QIcon>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSettings>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QWheelEvent>
#include <QTranslator>

#include "common.h"
#include "QDrawArea.h"

namespace Ui {
class GridArrayQt;
}

//class GCombox
class GCombox : public QComboBox
{
	Q_OBJECT

public:
	GCombox(QWidget *parent = 0);

	void wheelEvent(QWheelEvent *e);			//change the fontsize little by mouse wheel.
};


//class GridArrayQt.
class GridArrayQt : public QMainWindow
{
	Q_OBJECT

public:
	GridArrayQt(QWidget *parent = 0);
	~GridArrayQt();

	void SetInit();
	void createDrawArea();
	void createCursor();
	void setupMenu();
	void setupToolbar();
	void setupDockwidget();
	void setCtrlEnable(bool value);			//set ctrls disable when no file opened.

	bool loadSettings();
	void saveSettings();
	void openFile(byte type);
	byte judgeType(const QString &);		//judge the type of the file loaded. 
	bool loadFile(byte, const QString&);	//load gaq file, picture file or txt file.
	void saveGaqFile();
	bool readGaqFile(QFile *file);
	void writeGaqFile(QFile *file);

	void createFont();

    // Event
	void closeEvent(QCloseEvent *);
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

private:
	bool bLanguage;										//true for chinese.

	//Grid size
	int nGridW;
	int nGridH;

	//Graph
	QDrawArea *pDrawArea;
	short int nLoadDrawArea;

	//Picture to grid
	byte nImg2Grid_Type;								//The ways to convert image to grid.
	byte nImg2Grid_Value;

	//Cursor
	QPointCursor qPointCursor;								
	short unsigned int nCursorR;
	QCursor *qCursorBE;
	QPixmap *qPixmapBE;									//For Brush and Erase PointCursor.

	//Font
	GCombox *gcombox_FontSize;
	QString sFontName;
	int nFontSize;
	bool bFontBold;
	bool bFontItalic;
	bool bFontUnderline;

	//Menu
	QMenu *qMenu_File;
	QMenu *qMenu_Edit;
	QMenu *qMenu_View;
	QMenu *qMenu_Help;
	QAction *qFile_New;
	QAction *qFile_Open;
	QAction *qFile_Save;
	//QAction *qFile_SaveAs;
	QAction *qFile_Close;
	QAction *qEdit_Clear;
	QAction *qEdit_ZoomIn;
	QAction *qEdit_ZoomOut;
	QAction *qEdit_RotateLeft;
	QAction *qEdit_RotateRight;
	QAction *qEdit_FlipH;
	QAction *qEdit_FlipV;
	QAction *qView_LangCn;
	QAction *qView_LangEn;
	QAction *qHelp_About;

	//Toolbar
	QToolBar *qToolBar;
	QToolBar *qToolBar_Tool;
	QAction *qTool_Arrow;
	QAction *qTool_Point;
	QAction *qTool_Brush;
	QAction *qTool_Erase;
	QAction *qTool_Text;

	//Status Bar
	QLabel *statusMsg1;
	QLabel *statusMsg2;                              

    Ui::GridArrayQt *ui;
};

#endif // GRIDARRAYQT_H
