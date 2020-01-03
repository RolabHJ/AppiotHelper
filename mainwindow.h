#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QToolBar>

#include "worker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void updateTextBrowser(int nType, QString msg);


    void on_clear_pushButton_clicked();

    void on_apktoolDecode_pushButton_clicked();

    void on_apktoolBuild_pushButton_clicked();

    void on_actStack_pushButton_clicked();

    void on_killProcess_pushButton_clicked();


    void on_apkInfo_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Worker* m_pLTimeWorker;
    Worker* m_pSTimeWorker;

    QThread m_LTimeThread;
    QThread m_STimeThread;

    sPATH readConfigFile();
    sPATH m_sPath;
    sAPKINFO m_sApkInfo;
    sProcessARG m_sProcessArg;

    void initToolBar();

    void initWorker();

signals:
    void runLtProcess(sProcessARG& arg);
    void runStProcess(sProcessARG& arg);
};

#endif // MAINWINDOW_H
