#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMimeData>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setAcceptDrops(true);

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
			const std::string folderPath = fileExplorer.getFolderPath();

			const std::vector<XMLData> invoices = xmlParser.parseFolder(folderPath);

			if (invoices.empty())
			{
				ui->resultLabel->setText("Error. Correct invoices not found!");

				return;
			}

			const bool exported = excelExporter.exportInvoices(invoices, folderPath);

			ui->resultLabel->setText(exported ? "Success, created _XEFEXEL.xlsx" : "Error creating XLSX!");
		});
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasUrls())
	{
		event->acceptProposedAction();
	}
}

void MainWindow::dropEvent(QDropEvent* event)
{
	const QList<QUrl> urls = event->mimeData()->urls();

	if (urls.isEmpty())
	{
		return;
	}

	const QString path = urls.first().toLocalFile();

	ui->urlLine->setText(path);
}
