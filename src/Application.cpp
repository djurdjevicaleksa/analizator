#include "Application.h"

#include <QDir>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFileInfo>
#include <QListWidgetItem>

#include <sstream>
#include <iostream>

#include "Analizator.h"

Application::Application(int& argc, char**& argv) : qtApp(argc, argv) {}

int Application::run() {
    mainWindow.setWindowTitle("Analizator");
    mainWindow.resize(1280, 720);
    mainWindow.setCentralWidget(createStreamSelector());
    mainWindow.show();
    return qtApp.exec();
}

void Application::resetOutputContainer() {
    if (!outputLayout || !outputDisplay) return;
    while (outputLayout->count() > 1) {
        QLayoutItem* item = outputLayout->takeAt(0);
        if (item) {
            if (QWidget* widget = item->widget()) widget->deleteLater();
            delete item;
        }
    }
    outputDisplay->clear();
}

void Application::showPackets() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < this->analizator.ts_packets.size(); ++i)
        listWidget->addItem("TS paket #" + QString::number(i + 1));

    QObject::connect(listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        int index = listWidget->row(item);
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        this->analizator.printTSPacket(static_cast<size_t>(index));

        std::cout.rdbuf(oldCoutBuf);

        QString coloredHtml = ansiToHtml(QString::fromStdString(oss.str()));
        outputDisplay->setHtml(coloredHtml);
    });

    outputLayout->insertWidget(0, listWidget);
    
}

void Application::listPAT() {
    resetOutputContainer();
    // Placeholder
}

void Application::listPMT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < this->analizator.program_infos.size(); ++i)
        listWidget->addItem("Program #" + QString::number(i + 1));

    QObject::connect(listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        int index = listWidget->row(item);
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        this->analizator.printProgramInfo(static_cast<size_t>(index));

        std::cout.rdbuf(oldCoutBuf);
        outputDisplay->setPlainText(QString::fromStdString(oss.str()));
    });

    outputLayout->insertWidget(0, listWidget);
}

void Application::showPIDs() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    this->analizator.printGroupedPackets();

    std::cout.rdbuf(oldCoutBuf);
    outputDisplay->setPlainText(QString::fromStdString(oss.str()));

}

void Application::listNIT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (this->analizator.nit_tables.size() == 0){
        outputDisplay->setPlainText("Nema NIT tabela.");
        return;
    }
        
    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < this->analizator.nit_tables.size(); ++i)
        listWidget->addItem("NIT tabela #" + QString::number(i + 1));

    QObject::connect(listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        int index = listWidget->row(item);
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        this->analizator.printNIT(static_cast<size_t>(index));  // To be implemented

        std::cout.rdbuf(oldCoutBuf);
        outputDisplay->setPlainText(QString::fromStdString(oss.str()));
    });

    outputLayout->insertWidget(0, listWidget);
}

void Application::listSDT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (this->analizator.sdt_tables.size() == 0){
        outputDisplay->setPlainText("Nema SDT tabela.");
        return;
    }

    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < this->analizator.sdt_tables.size(); ++i)
        listWidget->addItem("SDT tabela #" + QString::number(i + 1));

    QObject::connect(listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        int index = listWidget->row(item);
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        this->analizator.printSDT(index);

        std::cout.rdbuf(oldCoutBuf);
        outputDisplay->setPlainText(QString::fromStdString(oss.str()));
    });

    outputLayout->insertWidget(0, listWidget);
}

QWidget* Application::createOptionsMenu() {
    QWidget* container = new QWidget(&mainWindow);
    QHBoxLayout* mainLayout = new QHBoxLayout(container);

    QWidget* leftWidget = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    QString baseName = QFileInfo(selectedFilePath).fileName();
    QLabel* streamName = new QLabel("Odabrani strim: " + baseName);
    streamName->setAlignment(Qt::AlignCenter);
    streamName->setStyleSheet("font-weight: bold; font-size: 14px;");
    streamName->setMinimumHeight(30);
    leftLayout->addWidget(streamName);

    struct Option {
        QString name;
        void (Application::*func)();
    };

    Option options[] = {
        { "Listaj PID-ove", &Application::showPIDs },
        { "PAT tabele", &Application::listPAT },
        { "PMT tabele", &Application::listPMT },
        { "NIT tabele", &Application::listNIT },
        { "SDT tabele", &Application::listSDT },
        { "Listaj pakete", &Application::showPackets }
    };

    for (const auto& option : options) {
        QPushButton* button = new QPushButton(option.name);
        QObject::connect(button, &QPushButton::clicked, [=]() {
            (this->*option.func)();
        });
        leftLayout->addWidget(button);
    }

    QPushButton* backButton = new QPushButton("← Nazad");
    leftLayout->addWidget(backButton);
    QObject::connect(backButton, &QPushButton::clicked, [=]() {
        mainWindow.setCentralWidget(createStreamSelector());
    });

    outputDisplay = new QTextEdit();
    outputDisplay->setReadOnly(true);

    outputLayout = new QVBoxLayout();
    QWidget* outputContainer = new QWidget();
    outputContainer->setLayout(outputLayout);
    outputLayout->addWidget(outputDisplay);

    mainLayout->addWidget(leftWidget, 1);
    mainLayout->addWidget(outputContainer, 2);

    return container;
}

QWidget* Application::createStreamSelector() {
    QWidget* selectorWidget = new QWidget(&mainWindow);
    QVBoxLayout* layout = new QVBoxLayout(selectorWidget);

    QLabel* label = new QLabel("Dostupni TS strimovi");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-weight: bold; font-size: 18px;");
    layout->addWidget(label);

    QListWidget* fileList = new QListWidget();
    layout->addWidget(fileList);

    QStringList args = QCoreApplication::arguments();
    const char* rootPath = args[1].toUtf8().constData();

    QDir dir(rootPath);
    for (const QString& file : dir.entryList(QStringList() << "*.ts", QDir::Files)) {
        fileList->addItem(file);
    }

    QObject::connect(fileList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
        if (item) {
            selectedFilePath = dir.absoluteFilePath(item->text());
            analizator = std::move(Analizator(selectedFilePath.toUtf8().constData()));
            mainWindow.setCentralWidget(createOptionsMenu());
        }
    });

    return selectorWidget;
}

QString Application::ansiToHtml(const QString& text) {
    QString html = text;
    // Blue
    html.replace("\033[34m", "<span style='color:blue;'>");
    // Green
    html.replace("\033[32m", "<span style='color:green;'>");
    // Reset colour
    html.replace("\033[0m", "</span>");
    // New line character
    html.replace("\n", "<br>");
    return html;
}
