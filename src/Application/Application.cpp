#include "Application.h"
#include "src/Application/Parsers/DataTypes/TSPacket.h"

Application::Application(int& argc, char**& argv) : qtApp(argc, argv) {}

int Application::run() {
    mainWindow.setWindowTitle("Analizator");
    mainWindow.resize(1920, 1080);
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

void Application::showPIDs() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (this->analizator.ts_by_pid.size() == 0){
        outputDisplay->setPlainText("U strimu nisu pronadjeni validni PID-ovi.");
        return;
    }

    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    this->analizator.printGroupedPackets();

    std::cout.rdbuf(oldCoutBuf);
    outputDisplay->setPlainText(QString::fromStdString(oss.str()));

}

void Application::listPAT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    
    this->analizator.printPAT();

    std::cout.rdbuf(oldCoutBuf);
    outputDisplay->setPlainText(QString::fromStdString(oss.str()));
}

void Application::listPMT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (this->analizator.program_infos.size() == 0){
        outputDisplay->setPlainText("U strimu nije pronadjena nijedna PMT tabela.");
        return;
    }

    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < this->analizator.program_infos.size(); ++i)
        listWidget->addItem("Program #" + QString::number(i + 1));

    listWidget->setStyleSheet(
        "QListWidget {"
        "  background-color: #f9f9fb;"
        "  font-family: Consolas, 'Courier New', monospace;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  color:rgb(52, 152, 219);"
        "  border: 1px solid #d0d0d0;"
        "  border-radius: 3px;"
        "  padding: 4px;"
        "}"
        "QListWidget::item {"
        "  padding: 4px;"
        "  margin: 4px;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #d6eaff;"
        "  color: black;"
        "}"
    );

    QObject::connect(listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        int index = listWidget->row(item);
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        this->analizator.printPMT(static_cast<size_t>(index));

        std::cout.rdbuf(oldCoutBuf);
        outputDisplay->setPlainText(QString::fromStdString(oss.str()));
    });

    outputLayout->insertWidget(0, listWidget);
}

void Application::listNIT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (this->analizator.nit_tables.size() == 0){
        outputDisplay->setPlainText("U strimu nije pronadjena nijedna NIT tabela.");
        return;
    }
        
    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < this->analizator.nit_tables.size(); ++i)
        listWidget->addItem("NIT tabela #" + QString::number(i + 1));

    listWidget->setStyleSheet(
        "QListWidget {"
        "  background-color: #f9f9fb;"
        "  font-family: Consolas, 'Courier New', monospace;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  color:rgb(52, 152, 219);"
        "  border: 1px solid #d0d0d0;"
        "  border-radius: 3px;"
        "  padding: 4px;"
        "}"
        "QListWidget::item {"
        "  padding: 4px;"
        "  margin: 4px;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #d6eaff;"
        "  color: black;"
        "}"
    );

    QObject::connect(listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        int index = listWidget->row(item);
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        this->analizator.printNIT(static_cast<size_t>(index));

        std::cout.rdbuf(oldCoutBuf);
        outputDisplay->setPlainText(QString::fromStdString(oss.str()));
    });

    outputLayout->insertWidget(0, listWidget);
}

void Application::listSDT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (this->analizator.sdt_tables.size() == 0){
        outputDisplay->setPlainText("U strimu nije pronadjena nijedna SDT tabela.");
        return;
    }

    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < this->analizator.sdt_tables.size(); ++i)
        listWidget->addItem("SDT tabela #" + QString::number(i + 1));

    listWidget->setStyleSheet(
        "QListWidget {"
        "  background-color: #f9f9fb;"
        "  font-family: Consolas, 'Courier New', monospace;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  color:rgb(52, 152, 219);"
        "  border: 1px solid #d0d0d0;"
        "  border-radius: 3px;"
        "  padding: 4px;"
        "}"
        "QListWidget::item {"
        "  padding: 4px;"
        "  margin: 4px;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #d6eaff;"
        "  color: black;"
        "}"
    );

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

void Application::showPackets() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (this->analizator.ts_packets.size() == 0){
        outputDisplay->setPlainText("Ovaj strim ne sadrzi validne TS pakete.");
        return;
    }

    QListWidget* listWidget = new QListWidget();

    for (std::size_t i = 0; i < this->analizator.ts_packets.size(); ++i) {
        std::string packet_name = "TS Packet [" + std::to_string(i) + "]";

        switch(this->analizator.ts_packets[i].getAdaptationFieldControl()) {
            case 0: break;
            case 1: packet_name += " (PLD)"; break;
            case 2: packet_name += " (ADP)"; break;
            case 3: packet_name += " (PLD, ADP)"; break;
        }

        listWidget->addItem(QString::fromStdString(packet_name));
    }

    listWidget->setStyleSheet(
        "QListWidget {"
        "  background-color: #f9f9fb;"
        "  font-family: Consolas, 'Courier New', monospace;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  color:rgb(52, 152, 219);"
        "  border: 1px solid #d0d0d0;"
        "  border-radius: 6px;"
        "  padding: 4px;"
        "}"
        "QListWidget::item {"
        "  padding: 4px;"
        "  margin: 2px;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #d6eaff;"
        "  color: black;"
        "}"
    );

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


QWidget* Application::createOptionsMenu() {
    QWidget* container = new QWidget(&mainWindow);
    QHBoxLayout* mainLayout = new QHBoxLayout(container);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(30);

    // Leva strana: dugmad i naziv strima
    QWidget* leftWidget = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(15);
    leftLayout->setContentsMargins(10, 10, 10, 10);

    QString baseName = QFileInfo(selectedFilePath).fileName();
    QLabel* streamName = new QLabel("Odabrani strim:\n" + baseName);
    streamName->setAlignment(Qt::AlignCenter);
    streamName->setStyleSheet("font-weight: bold; font-size: 16px; color: #333;");
    streamName->setWordWrap(true);
    streamName->setMinimumHeight(50);
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

    QString buttonStyle =
        "QPushButton {"
        "  background-color:rgb(52, 152, 219);"
        "  color: white;"
        "  font-size: 18px;"
        "  padding: 8px;"
        "  border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #2980b9;"
        "}";

    for (const auto& option : options) {
        QPushButton* button = new QPushButton(option.name);
        button->setStyleSheet(buttonStyle);
        QObject::connect(button, &QPushButton::clicked, [=]() {
            (this->*option.func)();
        });
        leftLayout->addWidget(button);
    }

    QPushButton* backButton = new QPushButton("← Nazad");
    backButton->setStyleSheet(buttonStyle);
    leftLayout->addStretch();
    leftLayout->addWidget(backButton);
    QObject::connect(backButton, &QPushButton::clicked, [=]() {
        mainWindow.setCentralWidget(createStreamSelector());
    });

    // Desna strana: prikaz rezultata
    outputDisplay = new QTextEdit();
    outputDisplay->setReadOnly(true);
    outputDisplay->setStyleSheet(
        "QTextEdit {"
        "  background-color: #f9f9fb;"
        "  font-family: Consolas, 'Courier New', monospace;"
        "  font-size: 16px;"
        "  color: #2c3e50;"
        "  border: 1px solid #d0d0d0;"
        "  border-radius: 8px;"
        "  padding: 12px;"
        "  selection-background-color: #d6eaff;"
        "}"
    );

    outputLayout = new QVBoxLayout();
    outputLayout->setContentsMargins(0, 0, 0, 0);
    outputLayout->setSpacing(0);

    QWidget* outputContainer = new QWidget();
    outputContainer->setLayout(outputLayout);
    
    outputLayout->addWidget(outputDisplay);

    mainLayout->addWidget(leftWidget, 1);        // leva kolona 1/3
    mainLayout->addWidget(outputContainer, 2);   // desna kolona 2/3

    return container;
}

QWidget* Application::createStreamSelector() {
    QWidget* selectorWidget = new QWidget(&mainWindow);
    QVBoxLayout* layout = new QVBoxLayout(selectorWidget);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* label = new QLabel("📺 Dostupni TS strimovi");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "font-size: 20px;"
        "font-weight: bold;"
        "color: #2c3e50;"
        "padding-bottom: 10px;"
    );
    layout->addWidget(label);

    QListWidget* fileList = new QListWidget();
    fileList->setStyleSheet(
        "QListWidget {"
        "  border: 2px solid #ccc;"
        "  border-radius: 10px;"
        "  font-size: 18px;"
        "  padding: 10px;"
        "  background-color: #fafafa;"
        "}"
        "QListWidget::item {"
        "  margin: 6px 0;"         
        "  padding: 8px 10px;"     
        "  border-radius: 5px;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #e0f0ff;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #3498db;"
        "  color: white;"
        "}"
    );
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