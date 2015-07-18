#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("GiderosMobile");
	QCoreApplication::setOrganizationDomain("giderosmobile.com");
	QCoreApplication::setApplicationName("GiderosStudio");

	QSettings::setDefaultFormat(QSettings::IniFormat);

	QApplication a(argc, argv);

	QDir dir = QCoreApplication::applicationDirPath();
#if defined(Q_OS_MAC)
	dir.cdUp();
#endif
	QDir::setCurrent(dir.absolutePath());

	MainWindow w;
	w.show();
    int result = a.exec();

    return result;
}
