#ifndef BANKMANAGER_H
#define BANKMANAGER_H

#include <QCoreApplication>
#include "xlsxdocument.h"
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QVariant>
#include <QLocale>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>

 class Bank: public QObject {
    Q_OBJECT

    protected:
        struct Transaction {
            QString date;
            QString category;
            QString account;
            QString amount;
            QString description;
        };

        QDateTime castQDateTime(const QString& dateString);
        void printBankFile();

    public:
        Bank(const QString& name, const QString& type);
        Bank(const QString& name, const QString& type, const QString& filePath);
        ~Bank() = default;

        QString nameBank;
        QString typeAccount;
        QString filePath;
        QList<Transaction> transactions;
        void printBankFile(const QString& filePath);
        virtual QList<Transaction> readBankMovements(const QString& filePath) = 0;
        virtual bool readBankMovements() = 0;

};

#endif // BANKMANAGER_H