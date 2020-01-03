#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMetaType>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_sPath = readConfigFile();
    initWorker();

//    connect(ui->actionImport_Apk,SIGNAL(triggered()),this,SLOT(importApkFile()));
    initToolBar();

}

MainWindow::~MainWindow()
{
    m_LTimeThread.quit();
    m_LTimeThread.wait();

    m_STimeThread.quit();
    m_STimeThread.wait();

    delete ui;
}

void MainWindow::updateTextBrowser(int nType, QString msg)
{
    ui->textBrowser->append(msg);
}

sPATH MainWindow::readConfigFile()
{
    sPATH sPath;
    sPath.inRootPath = ".";
    sPath.outRootPath = "/Users/hhj/Desktop/AndroidReverse/smali";

    QString jsonFilePath = QDir::currentPath() + "/../../../config.json";
    QFile loadFile(jsonFilePath);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open config.json";
        return sPath;
    }

    QByteArray configData = loadFile.readAll();
    loadFile.close();

    QJsonParseError jsonErr;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(configData, &jsonErr));

    if(jsonErr.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return sPath;
    }

    QJsonObject rootObj = jsonDoc.object();

    QStringList keys = rootObj.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        qDebug() << "key" << i << " is:" << keys.at(i);
    }

    if(rootObj.contains("tool_path"))
    {
        QJsonObject subObj = rootObj.value("tool_path").toObject();
        sPath.apktoolPath = subObj["apktool_path"].toString();
        qDebug() << "apktool_path:" << sPath.apktoolPath;
    }

    if(rootObj.contains("file_path"))
    {
        QJsonObject subObj = rootObj.value("file_path").toObject();

        sPath.outRootPath = subObj["output_path"].toString();
        qDebug() << "output_path:" << sPath.outRootPath;

        sPath.dexPath = sPath.outRootPath + "/" + subObj["dex_path"].toString();
        qDebug() << "dex_path:" << sPath.dexPath;
        sPath.smaliPath = sPath.outRootPath + "/" + subObj["smali_path"].toString();
        qDebug() << "smali_path:" << sPath.smaliPath;
    }

    return sPath;
}

void MainWindow::initToolBar()
{
    QAction * pNew = new QAction("新建");

    connect(pNew,&QAction::triggered,[=]()
    {
        m_sPath.apkPath = QFileDialog::getOpenFileName(this, "选择apk文件",".","*.apk");
        qDebug() << "apk path:" << m_sPath.apkPath;
    });

    QToolBar *toolBar = addToolBar("toolbar");
    toolBar->addAction(pNew);
}

void MainWindow::initWorker()
{
    m_pSTimeWorker = new Worker();
    m_pSTimeWorker->moveToThread(&m_STimeThread);

    qRegisterMetaType<sProcessARG>("sProcessARG&");

    connect(this,SIGNAL(runStProcess(sProcessARG&)),m_pSTimeWorker,SLOT(runProcess(sProcessARG&)));
    connect(&m_STimeThread, &QThread::finished, m_pSTimeWorker, &QObject::deleteLater);
    connect(m_pSTimeWorker,SIGNAL(updateMsg(int, QString)),this,SLOT(updateTextBrowser(int, QString)));


    m_pLTimeWorker = new Worker();
    m_pLTimeWorker->moveToThread(&m_LTimeThread);

    connect(this,SIGNAL(runLtProcess(sProcessARG&)),m_pLTimeWorker,SLOT(runProcess(sProcessARG&)));
    connect(&m_LTimeThread, &QThread::finished, m_pLTimeWorker, &QObject::deleteLater);
    connect(m_pLTimeWorker,SIGNAL(updateMsg(int, QString)),this,SLOT(updateTextBrowser(int, QString)));

    m_LTimeThread.start();
    m_STimeThread.start();
}

void MainWindow::on_clear_pushButton_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_apktoolDecode_pushButton_clicked()
{
    m_sProcessArg.nType = CMD_TYPE_DECOMPILE;
    m_sProcessArg.strInputPath = m_sPath.apkPath;
    m_sProcessArg.strOutputPath = m_sPath.smaliPath;
    m_sProcessArg.strEnvPath = "/usr/local/bin/";

    if(!m_sProcessArg.strInputPath.isEmpty())
        emit runLtProcess(m_sProcessArg);
}

void MainWindow::on_apktoolBuild_pushButton_clicked()
{
    m_sProcessArg.nType = CMD_TYPE_COMPILE;
    m_sProcessArg.strInputPath = m_sPath.smaliPath;
    m_sProcessArg.strOutputPath = "";
    m_sProcessArg.strEnvPath = "/usr/local/bin/";

    if(!m_sProcessArg.strInputPath.isEmpty())
        emit runLtProcess(m_sProcessArg);
}

void MainWindow::on_actStack_pushButton_clicked()
{
    m_sProcessArg.nType = CMD_TYPE_ACT_STACK;
    m_sProcessArg.strEnvPath = "/Users/hhj/Library/Android/sdk/platform-tools/";

    emit runStProcess(m_sProcessArg);
}

void MainWindow::on_killProcess_pushButton_clicked()
{
    //    emit runLTimeCmd(CMD_TYPE_KILL, m_CmdArgs);
    //    emit runSTimeCmd(CMD_TYPE_KILL, m_CmdArgs);
}

//void MainWindow::importApkFile()
//{
//    m_sPath.apkPath = QFileDialog::getOpenFileName(this, "选择apk文件",".","*.apk");
//    qDebug() << "apk path:" << m_sPath.apkPath;
//}

void MainWindow::on_apkInfo_pushButton_clicked()
{
    //    m_sPath.apkPath = "/Users/hhj/Desktop/AndroidReverse/MobileAssistant_1.apk";
    m_sProcessArg.nType = CMD_TYPE_APK_INFO;
    m_sProcessArg.strEnvPath = "/Users/hhj/Library/Android/sdk/build-tools/28.0.3/";
    m_sProcessArg.strInputPath = m_sPath.apkPath;

    if(!m_sProcessArg.strInputPath.isEmpty())
        emit runStProcess(m_sProcessArg);
}
