#include "banks/biceBank.h"

BiceBank::BiceBank(const QString& type)
    : Bank("BICE", type) {
}

BiceBank::BiceBank(const QString& type, const QString& filePath)
    : Bank("BICE", type, filePath) {
}


QList<Bank::Transaction> BiceBank::readBankMovements(const QString& filePath) {
    QXlsx::Document xlsx(filePath);
    QList<Transaction> transactionsList;

    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return transactionsList;
    }

    int row = 19;
    while(true){
        QString checkEndRows = xlsx.read(row, 2).toString();
        if(checkEndRows.trimmed().isEmpty()){
            break;
        } else {
            QString fullDescription = xlsx.read(row, 5).toString();
            if (!fullDescription.isEmpty()) {
                Transaction t;
                t.date = castQDateTime(fullDescription).toString("yyyy-MM-dd hh:mm:ss");
                t.category = xlsx.read(row, 3).toString();
                t.description = fullDescription;
                t.description.remove(QRegularExpression("\\s*(el\\s*)?\\d{1,2}/\\d{1,2}/\\d{4}"));
                t.description.remove(QRegularExpression("\\s*(a las\\s*)?\\d{1,2}:\\d{2}\\s*(hrs\\.?)?"));
                t.description.remove(QRegularExpression(":\\d{2}\\s*(hrs\\.?)?"));
                t.description.remove(QRegularExpression(",?\\s*monto\\s*\\$?\\s*[\\d.,]+"));
                t.description.remove(QRegularExpression(",?\\s*Rut\\s*[\\d.]+[-][\\dkK]"));
                t.description.remove(QRegularExpression("[,\\s]+$"));
                t.description = t.description.trimmed();
                t.amount = xlsx.read(row, 6).toString().split("$")[1].replace(".","");
                t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

                //qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
                transactionsList.append(t);
            }
            row++;
        }
    }
    return transactionsList;
}

bool BiceBank::readBankMovements() {
    transactions.clear();
    QXlsx::Document xlsx(filePath);

    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return false;
    }

    int row = 19;
    while(true){
        QString checkEndRows = xlsx.read(row, 2).toString();
        if(checkEndRows.trimmed().isEmpty()){
            break;
        } else {
            QString fullDescription = xlsx.read(row, 5).toString();
            if (!fullDescription.isEmpty()) {
                Transaction t;
                t.date = castQDateTime(fullDescription).toString("yyyy-MM-dd hh:mm:ss");
                t.category = xlsx.read(row, 3).toString().toLower();
                t.description = fullDescription;
                t.description.remove(QRegularExpression("\\s*(el\\s*)?\\d{1,2}/\\d{1,2}/\\d{4}"));
                t.description.remove(QRegularExpression("\\s*(a las\\s*)?\\d{1,2}:\\d{2}\\s*(hrs\\.?)?"));
                t.description.remove(QRegularExpression(":\\d{2}\\s*(hrs\\.?)?"));
                t.description.remove(QRegularExpression(",?\\s*monto\\s*\\$?\\s*[\\d.,]+"));
                t.description.remove(QRegularExpression(",?\\s*Rut\\s*[\\d.]+[-][\\dkK]"));
                t.description.remove(QRegularExpression("[,\\s]+$"));
                t.description = t.description.trimmed();
                t.amount = xlsx.read(row, 6).toString().split("$")[1].replace(".","");
                t.account = QString("%1 %2 card").arg(nameBank).arg(typeAccount);

                //qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
                transactions.append(t);
            }
            row++;
        }
    }
    return true;
}
