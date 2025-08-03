#ifndef HTMLPROCESSOR_H
#define HTMLPROCESSOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QList>
#include <QPair>

class HtmlProcessor : public QObject
{
    Q_OBJECT

public:
    HtmlProcessor(QObject *parent = nullptr);
    void process(const QString &htmlContent, bool saveImages, bool generatePdf);

private slots:
    void onDownloadFinished(QNetworkReply *reply);

private:
    void extractImageUrls(const QString &htmlContent);
    void downloadNextImage();
    void createPdf();

    QNetworkAccessManager *m_networkManager;
    QStringList m_imageUrls;
    QList<QPair<QString, QByteArray>> m_downloadedImages; // 存储图片URL和数据
    bool m_saveImages;
    bool m_generatePdf;
};

#endif // HTMLPROCESSOR_H
