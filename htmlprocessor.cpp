#include "htmlprocessor.h"

#include <QRegularExpression>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <QTextDocument>
#include <QPrinter>
#include <QDebug>
#include <QMessageBox>

HtmlProcessor::HtmlProcessor(QObject *parent)
    : QObject(parent),
    m_networkManager(new QNetworkAccessManager(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &HtmlProcessor::onDownloadFinished);
}

void HtmlProcessor::process(const QString &htmlContent, bool saveImages, bool generatePdf)
{
    m_saveImages = saveImages;
    m_generatePdf = generatePdf;
    m_downloadedImages.clear(); // 清空上次下载的图片
    m_imageUrls.clear();        // 清空上次的URL列表

    extractImageUrls(htmlContent);

    if (m_imageUrls.isEmpty()) {
        QMessageBox::information(nullptr, "提示", "未在HTML中找到图片链接。");
        return;
    }

    qDebug() << "找到的图片URL数量：" << m_imageUrls.size();
    downloadNextImage();
}

void HtmlProcessor::extractImageUrls(const QString &htmlContent)
{
    QRegularExpression regex("<img[^>]*src=\"([^\"]*)\"");
    QRegularExpressionMatchIterator i = regex.globalMatch(htmlContent);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        m_imageUrls << match.captured(1);
    }
}

void HtmlProcessor::downloadNextImage()
{
    if (m_imageUrls.isEmpty()) {
        qDebug() << "所有图片下载任务完成。";

        if (m_downloadedImages.isEmpty()) {
            QMessageBox::warning(nullptr, "警告", "所有图片下载失败，无法生成PDF。");
        } else if (m_generatePdf) {
            createPdf();
        }
        return;
    }

    QString url = m_imageUrls.takeFirst();
    QNetworkRequest request(url);
    m_networkManager->get(request);
}

void HtmlProcessor::onDownloadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        m_downloadedImages.append(QPair<QString, QByteArray>(reply->url().toString(), imageData));
        qDebug() << "图片下载成功：" << reply->url().toString();

        if (m_saveImages) {
            QDir().mkpath("images");
            QString fileName = "images/" + QFileInfo(reply->url().path()).fileName();
            if (!fileName.endsWith(".png", Qt::CaseInsensitive) && !fileName.endsWith(".jpg", Qt::CaseInsensitive)) {
                fileName += ".jpg";
            }
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(imageData);
                file.close();
                qDebug() << "图片已保存至：" << fileName;
            } else {
                qDebug() << "图片保存失败：" << fileName;
            }
        }
    } else {
        qDebug() << "图片下载失败：" << reply->errorString() << " URL:" << reply->url().toString();
    }

    reply->deleteLater();
    downloadNextImage();
}

void HtmlProcessor::createPdf()
{
    QTextDocument doc;
    QString htmlContent;

    for (const auto &pair : m_downloadedImages) {
        const QByteArray &imageData = pair.second;
        QString base64Image = QString::fromLatin1(imageData.toBase64());
        htmlContent += QString("<img src='data:image/jpeg;base64,%1' width='100%%'><br>").arg(base64Image);
    }

    doc.setHtml(htmlContent);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("output.pdf");

    doc.print(&printer);

    QMessageBox::information(nullptr, "成功", "PDF文件已生成：output.pdf");
    qDebug() << "PDF文件已生成：output.pdf";
}
