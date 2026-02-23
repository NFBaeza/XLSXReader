#include "banks/wiseBank.h"

WiseBank::WiseBank(const QString& type)
    : Bank("Wise", type) {
}

WiseBank::WiseBank(const QString& type, const QString& filePath)
    : Bank("Wise", type, filePath) {
}


bool WiseBank::readBankMovementsCredit(const QString& filePath){
    qDebug()<<"product not supported";
    return false;

}

bool WiseBank::readBankMovementsDebit(const QString& filePath){
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
        QString rawDate = row[3].remove("\"").trimmed();
        QDateTime dt = QDateTime::fromString(rawDate, "yyyy-MM-dd HH:mm:ss");
        t.date = dt.toString("yyyy-MM-dd HH:mm:ss");
        
        t.description = row[12].remove("\"");
        t.category = m_classifier.classify(t.description);
        t.description = t.description.trimmed();
        t.amount = (row[14] == "CLP") ? row[13] : "0";
        t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

        if(t.amount != "0"){
            qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
            Bank::transactions.append(t);
        }
            
    }
    return true;

}
