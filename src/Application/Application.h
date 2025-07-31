#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QListWidgetItem>

#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "Analizator.h"

class Application {
public:
    Application(int& argc, char**& argv);
    int run();

private:
    QApplication qtApp;
    QMainWindow mainWindow;

    QString selectedFilePath;
    QTextEdit* outputDisplay = nullptr;
    QVBoxLayout* outputLayout = nullptr;

    Analizator analizator;

    void resetOutputContainer();
    void showPackets();
    void showPIDs();
    void listPAT();
    void listPMT();
    void listNIT();
    void listSDT();

    QWidget* createStreamSelector();
    QWidget* createOptionsMenu();
    QString ansiToHtml(const QString& text);
};

#endif // _APPLICATION_H