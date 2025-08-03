#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onProcessButtonClicked();

private:
    void setupUi();

    QTextEdit *m_textEdit;
    QPushButton *m_processButton;
    QCheckBox *m_saveImagesCheckBox;
    QCheckBox *m_generatePdfCheckBox;
};

#endif // MAINWINDOW_H
