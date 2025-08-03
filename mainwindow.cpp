#include "mainwindow.h"
#include "htmlprocessor.h"

#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    // 创建中央窗口部件和布局
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 文本输入区域
    QLabel *textLabel = new QLabel("请输入HTML内容：");
    mainLayout->addWidget(textLabel);

    m_textEdit = new QTextEdit(this);
    m_textEdit->setPlaceholderText("在此输入HTML代码，例如：<img src='https://example.com/image1.jpg'>...");
    mainLayout->addWidget(m_textEdit);

    // 功能选择复选框
    m_saveImagesCheckBox = new QCheckBox("存储HTML文件指向的图片");
    mainLayout->addWidget(m_saveImagesCheckBox);

    m_generatePdfCheckBox = new QCheckBox("生成PDF");
    mainLayout->addWidget(m_generatePdfCheckBox);

    // 按钮
    m_processButton = new QPushButton("开始处理");
    mainLayout->addWidget(m_processButton);

    // 连接信号与槽
    connect(m_processButton, &QPushButton::clicked, this, &MainWindow::onProcessButtonClicked);

    setWindowTitle("HTML图片处理工具");
    resize(800, 600);
}

void MainWindow::onProcessButtonClicked()
{
    QString htmlContent = m_textEdit->toPlainText();

    if (htmlContent.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入HTML内容！");
        return;
    }

    bool saveImages = m_saveImagesCheckBox->isChecked();
    bool generatePdf = m_generatePdfCheckBox->isChecked();

    // 创建并运行HTML处理器
    HtmlProcessor processor;
    processor.process(htmlContent, saveImages, generatePdf);
}
