#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->urlLine, &QLineEdit::textChanged, this,
		[this](const QString& text)
		{
			fileExplorer.insertFolderPath(text.toStdString());

			std::optional<size_t> statusCode = fileExplorer.analyzeFolder();

			if (statusCode == std::nullopt)
			{
				ui->statusLabel->setText(QString("Wrong folder path"));
				ui->convertButton->setEnabled(false);
			}
			else if (statusCode == 0)
			{
				ui->statusLabel->setText(QString("No XML files in folder"));
				ui->convertButton->setEnabled(false);
			}
			else
			{
				ui->statusLabel->setText(QString("XML files count: %1").arg(*statusCode));
				ui->convertButton->setEnabled(true);
			}
		});

	connect(ui->convertButton, &QPushButton::clicked, this, [this]()
		{
			xmlParser.parseFolder(fileExplorer.getFolderPath());

			ui->resultLabel->setText("Konwersja zakoñczona!");
		});
}

MainWindow::~MainWindow()
{
	delete ui;
}
