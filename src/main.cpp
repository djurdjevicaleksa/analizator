#include <QApplication>
#include <QMainWindow>
#include <QDir>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QTextEdit>
#include <QDebug>
#include <QFileInfo>

#include <unordered_map>
#include <vector>
#include <sstream>
#include <iostream>

// Pretpostavljam da imaš ove includeove i klase
#include "../include/TSParser.h"
#include "../include/SDTParser.h"
#include "../include/NITParser.h"
#include "../include/PatPmtParser.h"

// Globalna promenljiva za izabranu putanju
QString selectedFilePath; 

// Predeklaracija
QWidget* createStreamSelector(QMainWindow* mainWindow);  

// Globalni pokazivač na output za ispis rezultata
QTextEdit* outputDisplay = nullptr;
QVBoxLayout* outputLayout = nullptr;

TSParser ts_parser;
TsPaketParser pat_pmt_parser;
std::vector<TSPacket> ts_packets;
std::unordered_map<uint16_t, std::vector<TSPacket>> grouped_packets;

// Mapa za prikaz prilagođenih imena PID-ova
std::unordered_map<uint16_t, std::string> custom_pid_names;

void resetOutputContainer() {
    if (!outputLayout || !outputDisplay) return;

    while (outputLayout->count() > 1) {
        QLayoutItem* item = outputLayout->takeAt(0);
        if (item) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
    }

    outputDisplay->clear();
}

void showPackets() {
    resetOutputContainer();
}

void showPIDs() {
    resetOutputContainer();
    if (outputDisplay) {
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::ostringstream oss;
        std::cout.rdbuf(oss.rdbuf());

        ts_parser.printGroupedPackets(grouped_packets, custom_pid_names);

        std::cout.rdbuf(oldCoutBuf);
        outputDisplay->setPlainText(QString::fromStdString(oss.str()));
    }
}

void listPAT() {
    resetOutputContainer();
    
}

void listPMT() {
    resetOutputContainer();
    
}

void listNIT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (grouped_packets.count(NetworkInformationSection::NIT_PID) == 0) {
        outputDisplay->setPlainText("Nema pronađenih NIT tabela.");
        return;
    }

    std::vector<NetworkInformationSection> nit_tables =
        NIT::parse(grouped_packets.at(NetworkInformationSection::NIT_PID));

    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < nit_tables.size(); ++i) {
        listWidget->addItem("NIT tabela #" + QString::number(i + 1));
    }

    QObject::connect(listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        int index = listWidget->row(item);

        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        if (index >= 0 && static_cast<size_t>(index) < nit_tables.size()) {
            nit_tables[index].print();
        }

        std::cout.rdbuf(oldCoutBuf);

        outputDisplay->setPlainText(QString::fromStdString(oss.str()));
    });

    outputLayout->insertWidget(0, listWidget);
}

void listSDT() {
    resetOutputContainer();
    if (!outputLayout || !outputDisplay) return;

    if (grouped_packets.count(ServiceDescriptionSection::SDT_PID) == 0) {
        outputDisplay->setPlainText("Nema pronađenih SDT tabela.");
        return;
    }

    std::vector<ServiceDescriptionSection> sdt_tables =
        SDT::parse(grouped_packets.at(ServiceDescriptionSection::SDT_PID));

    QListWidget* listWidget = new QListWidget();
    for (size_t i = 0; i < sdt_tables.size(); ++i) {
        listWidget->addItem("SDT tabela #" + QString::number(i + 1));
    }

    QObject::connect(listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        int index = listWidget->row(item);

        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        if (index >= 0 && static_cast<size_t>(index) < sdt_tables.size()) {
            sdt_tables[index].print();
        }

        std::cout.rdbuf(oldCoutBuf);

        outputDisplay->setPlainText(QString::fromStdString(oss.str()));
    });

    outputLayout->insertWidget(0, listWidget);
}

// Glavni meni posle izbora fajla (levi: opcije, desni: izlaz)
QWidget* createOptionsMenu(QMainWindow* mainWindow) {
    QWidget* container = new QWidget(mainWindow);
    QHBoxLayout* mainLayout = new QHBoxLayout(container);

    // Leva strana
    QWidget* leftWidget = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    // Parsiranje input strima i grupisanje paketa po PID-ovima
    ts_packets = ts_parser.parseTransportStream(selectedFilePath.toUtf8().constData());
    grouped_packets = ts_parser.groupPacketsByPID(ts_packets);

    // Popunjavanje custom_pid_names na osnovu PMT-a
    for (const auto& program : pat_pmt_parser.getPrograms()) {
        custom_pid_names[program.pmt_pid] = "PMT (Program Map Table)";

        for (const auto& stream : program.streams) {
            std::string type_str;
            switch (stream.type) {
                case StreamType::VIDEO:    type_str = "Video Stream"; break;
                case StreamType::AUDIO:    type_str = "Audio Stream"; break;
                case StreamType::SUBTITLE: type_str = "Subtitle Stream"; break;
                case StreamType::DATA:     type_str = "Data Stream"; break;
                default:                   type_str = "Unknown Stream"; break;
            }

            custom_pid_names[stream.pid] = type_str + " (" + stream.codec + ")";
        }
    }

    // Naziv izabranog fajla
    QString baseName = QFileInfo(selectedFilePath).fileName();
    QLabel* streamName = new QLabel("Odabrani strim: " + baseName);
    streamName->setAlignment(Qt::AlignCenter);
    streamName->setStyleSheet("font-weight: bold; font-size: 14px;");
    streamName->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    streamName->setMinimumHeight(30);
    leftLayout->addWidget(streamName);

    struct Option {
        QString name;
        void (*func)();
    };

    Option options[] = {
        { "Listaj PID-ove", showPIDs },
        { "PAT tabele", listPAT },
        { "PMT tabele", listPMT },
        { "NIT tabele", listNIT },
        { "SDT tabele", listSDT },
        { "Listaj pakete", showPackets }
    };

    for (const auto& option : options) {
        QPushButton* button = new QPushButton(option.name);
        leftLayout->addWidget(button);

        QObject::connect(button, &QPushButton::clicked, [option]() {
            option.func();
        });
    }

    // Dugme za povratak
    QPushButton* backButton = new QPushButton("← Nazad");
    leftLayout->addWidget(backButton);
    QObject::connect(backButton, &QPushButton::clicked, [mainWindow]() {
        QWidget* selector = createStreamSelector(mainWindow);
        mainWindow->setCentralWidget(selector);
    });

    // Desna strana — prikaz rezultata
    outputDisplay = new QTextEdit();
    outputDisplay->setReadOnly(true);

    outputLayout = new QVBoxLayout();
    QWidget* outputContainer = new QWidget();
    outputContainer->setLayout(outputLayout);
    outputLayout->addWidget(outputDisplay);

    // Postavljanje rasporeda
    mainLayout->addWidget(leftWidget, 1);    // Leva strana zauzima 1 deo prostora
    mainLayout->addWidget(outputContainer, 2); // Desna strana 2 dela (šira)

    return container;
}

// Meni za izbor TS fajla
QWidget* createStreamSelector(QMainWindow* mainWindow) {
    QWidget* selectorWidget = new QWidget(mainWindow);
    QVBoxLayout* layout = new QVBoxLayout(selectorWidget);

    QLabel* label = new QLabel("Dostupni TS strimovi");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-weight: bold; font-size: 18px;");
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    label->setMinimumHeight(50);
    layout->addWidget(label);

    QListWidget* fileList = new QListWidget();

    fileList->setStyleSheet(R"(
        QListWidget {
            background-color: #fafafa;
            border: 1px solid #dddddd;
            border-radius: 8px;
            padding: 8px;
            font-family: "Segoe UI", sans-serif;
            font-size: 13px;
        }
    
        QListWidget::item {
            padding: 10px;
            border: 1px solid transparent;
            border-radius: 5px;
            margin: 2px;
        }
    
        QListWidget::item:selected {
            color: black;
        }
    
        QListWidget::item:hover {
            background-color:rgb(178, 211, 243);
            color: black;
        }
    )");

    layout->addWidget(fileList);

    QString folderPath = "../ts";
    QDir dir(folderPath);
    QStringList tsFiles = dir.entryList(QStringList() << "*.ts", QDir::Files);
    for (const QString& file : tsFiles) {
        fileList->addItem(file);
    }

    QObject::connect(fileList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
        if (item) {
            selectedFilePath = dir.absoluteFilePath(item->text());
            QWidget* optionsMenu = createOptionsMenu(mainWindow);
            mainWindow->setCentralWidget(optionsMenu);
        }
    });

    return selectorWidget;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("TSAnalyzer");
    mainWindow.resize(1280, 720);

    QWidget* selector = createStreamSelector(&mainWindow);
    mainWindow.setCentralWidget(selector);

    mainWindow.show();
    return app.exec();
}
