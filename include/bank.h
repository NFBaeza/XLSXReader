#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

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
        // Struct accesible para clases hijas
        struct Transaction {
            QDateTime date;
            QString category;
            QString account;
            QString amount;
            QString description;
        };

        QList<Transaction> transactions;

        QDateTime castQDateTime(const QString& dateString);

        virtual bool readBankMovements() = 0;

        void printBankFile();

    public:
        Bank(const QString& name, const QString& type);
        Bank(const QString& name, const QString& type, const QString& filePath);
        ~Bank() = default;

        QString nameBank;
        QString typeAccount;
        QString filePath;
        void printBankFile(const QString& filePath);
        bool uploadDataToDataBase(QSqlDatabase db, const QString& nameTable);
        virtual QList<Transaction> readBankMovements(const QString& filePath) = 0;

};

#endif // DATABASEMANAGER_H