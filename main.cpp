/* 
 * GridArrayQt
 * Author: YeHuoHangXing
 * Description: This is for creating picture array of OLED screen.
 * Qt version: Qt5.7.0
 * 
 */

#include "GridArrayQt.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForLocale(codec);

	QApplication a(argc, argv);

	//set the language.
	bool lang;
	QTranslator translator;
	QSettings qSettings("Settings.ini",QSettings::IniFormat);
	qSettings.setIniCodec("utf-8");
	lang = qSettings.value("Language/Lang").toBool();
	if(lang)
	{
		translator.load("lang_zh_cn.qm");
		qApp->installTranslator(&translator);
	}

	GridArrayQt w;
	w.show();
	return a.exec();
}
