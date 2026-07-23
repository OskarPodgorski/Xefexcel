#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->convertButton, &QPushButton::clicked, this, [this]()
		{
			ui->resultLabel->setText("Konwersja zakoñczona!");
		});

	connect(ui->urlLine, &QLineEdit::textChanged, this,
		[this](const QString& text)
		{
			fileExplorer.insertFolderPath(text.toStdString());
			ui->statusLabel->setText(text);
			std::cout << fileExplorer.getFolderPath() << std::endl;
		});
}

MainWindow::~MainWindow()
{
	delete ui;
}
