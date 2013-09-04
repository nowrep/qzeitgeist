/********************************************************************************
** Form generated from reading UI file 'zgbrowser.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZGBROWSER_H
#define UI_ZGBROWSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_ZgBrowser
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QListWidget *listWidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *id;
    QLabel *label_3;
    QLabel *date;
    QLabel *label_5;
    QLabel *actor;
    QLabel *label_6;
    QLabel *interpretation;
    QLabel *label_7;
    QLabel *manifestation;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *text;
    QLabel *url;
    QWebView *webView;
    QCheckBox *recordSites;

    void setupUi(QWidget *ZgBrowser)
    {
        if (ZgBrowser->objectName().isEmpty())
            ZgBrowser->setObjectName(QString::fromUtf8("ZgBrowser"));
        ZgBrowser->resize(1000, 650);
        verticalLayout = new QVBoxLayout(ZgBrowser);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        splitter_2 = new QSplitter(ZgBrowser);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        listWidget = new QListWidget(splitter);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        splitter->addWidget(listWidget);
        formLayoutWidget = new QWidget(splitter);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        id = new QLabel(formLayoutWidget);
        id->setObjectName(QString::fromUtf8("id"));

        formLayout->setWidget(0, QFormLayout::FieldRole, id);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        date = new QLabel(formLayoutWidget);
        date->setObjectName(QString::fromUtf8("date"));

        formLayout->setWidget(1, QFormLayout::FieldRole, date);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

        actor = new QLabel(formLayoutWidget);
        actor->setObjectName(QString::fromUtf8("actor"));

        formLayout->setWidget(2, QFormLayout::FieldRole, actor);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_6);

        interpretation = new QLabel(formLayoutWidget);
        interpretation->setObjectName(QString::fromUtf8("interpretation"));

        formLayout->setWidget(3, QFormLayout::FieldRole, interpretation);

        label_7 = new QLabel(formLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_7);

        manifestation = new QLabel(formLayoutWidget);
        manifestation->setObjectName(QString::fromUtf8("manifestation"));

        formLayout->setWidget(4, QFormLayout::FieldRole, manifestation);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_2);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_4);

        text = new QLabel(formLayoutWidget);
        text->setObjectName(QString::fromUtf8("text"));

        formLayout->setWidget(5, QFormLayout::FieldRole, text);

        url = new QLabel(formLayoutWidget);
        url->setObjectName(QString::fromUtf8("url"));

        formLayout->setWidget(6, QFormLayout::FieldRole, url);

        splitter->addWidget(formLayoutWidget);
        splitter_2->addWidget(splitter);
        webView = new QWebView(splitter_2);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl(QString::fromUtf8("about:blank")));
        splitter_2->addWidget(webView);

        verticalLayout->addWidget(splitter_2);

        recordSites = new QCheckBox(ZgBrowser);
        recordSites->setObjectName(QString::fromUtf8("recordSites"));

        verticalLayout->addWidget(recordSites);


        retranslateUi(ZgBrowser);

        QMetaObject::connectSlotsByName(ZgBrowser);
    } // setupUi

    void retranslateUi(QWidget *ZgBrowser)
    {
        ZgBrowser->setWindowTitle(QApplication::translate("ZgBrowser", "ZgBrowser", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ZgBrowser", "ID:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ZgBrowser", "Date:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ZgBrowser", "Actor:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ZgBrowser", "Interpretation:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ZgBrowser", "Manifestation:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ZgBrowser", "Text:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ZgBrowser", "Url:", 0, QApplication::UnicodeUTF8));
        recordSites->setText(QApplication::translate("ZgBrowser", "Record visited sites", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ZgBrowser: public Ui_ZgBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZGBROWSER_H
