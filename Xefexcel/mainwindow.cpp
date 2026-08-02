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

			std::optional<size_t> statusCode = fileExplorer.analyzeFolder();

			ui->statusLabel->setText(statusCode == std::nullopt ? QString("Wrong folder path") : statusCode == 0 ? QString("No XML files in folder") : QString("XML files count: %1").arg(*statusCode));
			
		});
}

MainWindow::~MainWindow()
{
	delete ui;
}
