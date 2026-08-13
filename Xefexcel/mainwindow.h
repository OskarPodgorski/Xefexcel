#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "fileExplorer.h"
#include <optional>
#include "XMLParser.h"
#include "excelExporter.h"
#include <QDragEnterEvent>
#include <QDropEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

private:

	Ui::MainWindow* ui;
	FileExplorer fileExplorer;
	XMLParser xmlParser;
	ExcelExporter excelExporter;

protected:

	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

};
#endif // MAINWINDOW_H
