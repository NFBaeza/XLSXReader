#include "banks/wiseBank.h"

WiseBank::WiseBank(const QString& type)
    : Bank("Wise", type) {
}

WiseBank::WiseBank(const QString& type, const QString& filePath)
    : Bank("Wise", type, filePath) {
}

bool WiseBank::readBankMovements(){
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return false;
    }

    QTextStream stream(&file);
    QRegularExpression dateRegex("(\\d{1,4})");

    while (!stream.atEnd()) {
        Transaction t;

        QStringList row = stream.readLine().split(",");
        t.date = row[3].remove("\"");
        
        t.description = row[12].remove("\"");
        t.category = m_classifier.classify(t.description);
        t.description = t.description.trimmed();
        t.amount = (row[14] == "CLP") ? row[13] : "0";
        t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

        qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
            
    }
    return true;
}

QList<Bank::Transaction> WiseBank::readBankMovements(const QString& filePath) {
    QList<Transaction> transactionsList;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return transactionsList;
    }

    QTextStream stream(&file);
    QRegularExpression dateRegex("(\\d{1,4})");

    while (!stream.atEnd()) {
        Transaction t;

        QStringList row = stream.readLine().split(",");
        t.date = row[3].remove("\"");
        
        t.description = row[12].remove("\"");
        t.category = m_classifier.classify(t.description);
        t.description = t.description.trimmed();
        t.amount = (row[14] == "CLP") ? row[13] : "0";
        t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

        qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
        transactionsList.append(t);
            
    }
    return transactionsList;
}
