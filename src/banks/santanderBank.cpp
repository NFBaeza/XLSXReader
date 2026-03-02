#include "banks/santanderBank.h"

SantanderBank::SantanderBank(const QString& type)
    : Bank("Santander", type) {
}

SantanderBank::SantanderBank(const QString& type, const QString& filePath)
    : Bank("Santander", type, filePath) {
}

bool SantanderBank::readBankMovementsCredit(const QString& filePath){
    qDebug()<<"product not supported";
    return false;
}

bool SantanderBank::readBankMovementsDebit(const QString& filePath) {
    QXlsx::Document xlsx(filePath);
    bool cartola = false;
    QString date = "";
    QString year = "";
    int row = 1;

    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return false;
    }
    

    if(xlsx.read(1, 1).toString() == "Banco Santander"){
        qDebug()<<"Cartola";
        cartola = true;
        QRegularExpression yearRegex("(\\d{1,4})");
        QRegularExpressionMatch yearMatch = yearRegex.match(xlsx.read(1, 5).toString());
        if (!yearMatch.hasMatch()) {
            qDebug() << "No se encontró año en:" << xlsx.read(1, 5).toString();
        return false;
        }

        year = yearMatch.captured(1);
        row = 22;
        qDebug()<<"año encontrada: "<< year;
        
    } else {
        qDebug()<<"Provisional";
        year = xlsx.read(4,1).toString().split("-")[2];

        qDebug()<<"año" << year;
        row = 4;
    }
    
    while(true){
        QString checkEndRows = xlsx.read(row, 1).toString();
        
        if(checkEndRows.trimmed().isEmpty()){
            return true;
        } else {
            Transaction t;
            if(cartola){
                date = QString("%1/%2").arg(xlsx.read(row,1).toString()).arg(year);
                t.date = castQDateTime(date).toString("yyyy-MM-dd");
                t.description = xlsx.read(row, 3).toString();
                
                if(xlsx.read(row, 5).toString().isNull()){
                    t.amount = QString::number(xlsx.read(row, 6).toInt());
                }else{
                    t.amount = QString::number(xlsx.read(row, 5).toInt());
                }
            }else{
                date = xlsx.read(row,1).toString();
                t.date = date;
                t.description = xlsx.read(row, 2).toString();
                
                if(xlsx.read(row, 3).toString().isNull()){
                    t.amount = QString::number(xlsx.read(row, 4).toInt());
                }else{
                    t.amount = QString::number(xlsx.read(row, 3).toInt());
                }
            }
            
            t.category = m_classifier.classify(t.description);
            t.description.remove(QRegularExpression("\\d{10}|\\d{9}[Kk]"));
            t.description = t.description.trimmed();

            t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

            qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
            Bank::transactions.append(t);
            
            row++;
        }
    } 
    return false;
}
