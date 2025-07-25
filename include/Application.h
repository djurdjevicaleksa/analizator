
#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QString>
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