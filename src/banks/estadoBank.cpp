#include "banks/estadoBank.h"

EstadoBank::EstadoBank(const QString& type)
    : Bank("ESTADO", type) {
}

EstadoBank::EstadoBank(const QString& type, const QString& filePath)
    : Bank("ESTADO", type, filePath) {
}


QList<Bank::Transaction> EstadoBank::readBankMovements(const QString& filePath) {
    QXlsx::Document xlsx(filePath);
    QList<Transaction> transactionsList;

    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return transactionsList;
    }

    QString year = xlsx.read(6, 4).toString().split("/")[2].split(" ")[0].trimmed();
    QLocale locale(QLocale::English); // O QLocale::Spanish

    qDebug()<<"año encontrada: "<< year;

    int row = 20;
    while(true){
        QString checkEndRows = xlsx.read(row, 1).toString();
        if(checkEndRows.trimmed().isEmpty()){
            break;
        } else {
            Transaction t;

            QString monthString = xlsx.read(row,1).toString().trimmed();
            QString date = QString("%1/%2").arg(monthString).arg(year);
            QDate monthNumber = locale.toDate(date, "dd/MMM/yyyy");

            t.date = QString("%1/%2/%3").arg(xlsx.read(row,1).toString().split("/")[0]).arg(monthNumber.toString("MM")).arg(year);
            t.description= xlsx.read(row, 3).toString();

            t.description = t.description.trimmed();

            QRegularExpression transferPrefix("(TEF)");
            QRegularExpressionMatch transferPrefixMatch = transferPrefix.match(t.description);

            if(xlsx.read(row, 4).toString().isNull()){
                t.amount = xlsx.read(row, 5).toString().replace(".","");
                t.category = (transferPrefixMatch.hasMatch()) ? "bank transfer" : m_classifier.classify(t.description);
            }else{
                t.amount = xlsx.read(row, 4).toString().replace(".","");
                t.category = (transferPrefixMatch.hasMatch()) ? "deposit" : m_classifier.classify(t.description);
            }

            t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

            qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
            transactionsList.append(t);   
            row++;
        }
    }
    return transactionsList;
}

bool EstadoBank::readBankMovements() {
    transactions.clear();
    QXlsx::Document xlsx(filePath);

    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return false ;
    }

    QString year = QDate::fromString(xlsx.read(6, 4).toString(), "dd/MM/yyyy").toString("yyyy");
    QString month = QDate::fromString(xlsx.read(6, 4).toString(), "dd/MM/yyyy").toString("MM");

    qDebug()<<"mes encontrada: "<< month;
    qDebug()<<"año encontrada: "<< year;

    int row = 20;
    while(true){
        QString checkEndRows = xlsx.read(row, 1).toString();
        if(checkEndRows.trimmed().isEmpty()){
            break;
        } else {
            Transaction t;

            t.date = QString("%1/%2/%3").arg(xlsx.read(row,1).toString().split("/")[0]).arg(month).arg(year);
            
            t.description= xlsx.read(row, 3).toString();

            t.category = m_classifier.classify(t.description);
            
            t.description.remove(QRegularExpression("\\d{10}"));
            t.description = t.description.trimmed();

            if(xlsx.read(row, 4).toString().isNull()){
                t.amount = xlsx.read(row, 5).toString().replace(".","");
            }else{
                t.amount = xlsx.read(row, 4).toString().replace(".","");
            }

            t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

            qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
            transactions.append(t);   
            row++;
        }
    }
    return true;
}