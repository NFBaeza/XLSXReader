#include "banks/santanderBank.h"

SantanderBank::SantanderBank(const QString& type)
    : Bank("Santander", type) {
}

SantanderBank::SantanderBank(const QString& type, const QString& filePath)
    : Bank("Santander", type, filePath) {
}


bool SantanderBank::readBankMovements() {
    transactions.clear();
    QXlsx::Document xlsx(filePath);

    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return false ;
    }

    QRegularExpression yearRegex("(\\d{1,4})");
    QRegularExpressionMatch yearMatch = yearRegex.match(xlsx.read(1, 5).toString());

    if (!yearMatch.hasMatch()) {
        qDebug() << "No se encontró año en:" << xlsx.read(1, 5).toString();
        return false;
    }

    QString year = yearMatch.captured(1);
    qDebug()<<"año encontrada: "<< year;

    int row = 22;
    while(true){
        QString checkEndRows = xlsx.read(row, 1).toString();
        
        if(checkEndRows.trimmed().isEmpty()){
            break;
        } else {
            Transaction t;
            QString date = QString("%1/%2").arg(xlsx.read(row,1).toString()).arg(year);
            t.date = castQDateTime(date);
            
            t.description = xlsx.read(row, 3).toString();
            t.description.remove(QRegularExpression("\\d{10}"));
            t.description = t.description.trimmed();

            if(xlsx.read(row, 5).toString().isNull()){
                t.category = "Cargo";
                t.amount = QString::number(xlsx.read(row, 6).toInt());
            }else{
                t.category = "Abono";
                t.amount = QString::number(xlsx.read(row, 5).toInt());
            }

            t.account = QString("%1 %2 card").arg(nameBank).arg(typeAccount);

            qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
            transactions.append(t);
            
            row++;
        }
    } 
    return true;
}

QList<Bank::Transaction> SantanderBank::readBankMovements(const QString& filePath) {
    QXlsx::Document xlsx(filePath);
    QList<Transaction> transactionsList;

    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return transactionsList;
    }
    
    QRegularExpression yearRegex("(\\d{1,4})");
    QRegularExpressionMatch yearMatch = yearRegex.match(xlsx.read(1, 5).toString());

    if (!yearMatch.hasMatch()) {
        qDebug() << "No se encontró año en:" << xlsx.read(1, 5).toString();
        return transactionsList;
    }

    QString year = yearMatch.captured(1);
    qDebug()<<"año encontrada: "<< year;

    int row = 22;
    while(true){
        QString checkEndRows = xlsx.read(row, 1).toString();
        
        if(checkEndRows.trimmed().isEmpty()){
            break;
        } else {
            Transaction t;
            QString date = QString("%1/%2").arg(xlsx.read(row,1).toString()).arg(year);
            t.date = castQDateTime(date);
            
            t.description = xlsx.read(row, 3).toString();
            t.description.remove(QRegularExpression("\\d{10}"));
            t.description = t.description.trimmed();

            if(xlsx.read(row, 5).toString().isNull()){
                t.category = "Cargo";
                t.amount = QString::number(xlsx.read(row, 6).toInt());
            }else{
                t.category = "Abono";
                t.amount = QString::number(xlsx.read(row, 5).toInt());
            }

            t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

            qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
            transactionsList.append(t);
            
            row++;
        }
    } 
    return transactionsList;
}
