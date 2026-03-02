#include "banks/biceBank.h"

BiceBank::BiceBank(const QString& type)
    : Bank("BICE", type) {
}

BiceBank::BiceBank(const QString& type, const QString& filePath)
    : Bank("BICE", type, filePath) {
}

bool BiceBank::readBankMovementsCredit(const QString& filePath){
    qDebug()<<"product not supported";
    return false;
}

bool BiceBank::readBankMovementsDebit(const QString& filePath) {
    QXlsx::Document xlsx(filePath);
    QLocale locale(QLocale::Spanish); // O QLocale::Spanish

    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return false;
    }
    QString initAmount = xlsx.read(11, 2).toString().trimmed();
    
    int row = 19;
    QString fullDescription;
    while(true){
        QString checkEndRows = xlsx.read(row, 2).toString();
        if(checkEndRows.trimmed().isEmpty() || checkEndRows.contains("00:00")){
            return true;
        } else {
            Transaction t;
            
            if(initAmount == "Saldo inicial"){
                fullDescription = xlsx.read(row, 5).toString().trimmed();
                t.amount = xlsx.read(row, 6).toString().split("$")[1].replace(".","");
            } else{
                fullDescription = xlsx.read(row, 4).toString().trimmed();
                t.amount = xlsx.read(row, 5).toString().split("$")[1].replace(".","");
            }

            if (!fullDescription.isEmpty()) {
                auto isDateDividedBySpaceOnDescription = QRegularExpression("el\\s+\\d{1,2}/\\d{1,2}/\\d{4}\\s+a\\s+las\\s+\\d{1,2}:\\d{2}\\s*(hrs\\.?)?",
                    QRegularExpression::CaseInsensitiveOption).match(fullDescription);

                auto isDateDividedByLineOnDescription = QRegularExpression("el\\s+\\d{4}-\\d{1,2}-\\d{1,2}\\s+a\\s+las\\s+\\d{1,2}:\\d{2}\\s*(hrs\\.?)?",
                    QRegularExpression::CaseInsensitiveOption).match(fullDescription);
                
                if (isDateDividedBySpaceOnDescription.hasMatch()) {;
                    QStringList fullMatch = isDateDividedBySpaceOnDescription.captured(0).split("a las");
                    QString hour = fullMatch[1].trimmed();
                    QString date = fullMatch[0].split("el")[1].trimmed();
                    QString dateFormat = QString("%1 %2").arg(date).arg(hour);
                    t.date = castQDateTime(dateFormat).toString("yyyy-MM-dd HH:mm");

                }else if(isDateDividedByLineOnDescription.hasMatch()){
                    QStringList fullMatch = isDateDividedByLineOnDescription.captured(0).split("a las");
                    
                    QString hour = fullMatch[1].replace("hrs."," ").trimmed();
                    QString date = fullMatch[0].split("el")[1].trimmed();
                    QStringList dateByPieces = date.split("-");
                    QString dateFormat = QString("%1/%2/%3 %4").arg(dateByPieces[2]).arg(dateByPieces[1]).arg(dateByPieces[0]).arg(hour);
                    t.date = castQDateTime(dateFormat).toString("yyyy-MM-dd HH:mm");

                }else{              
                    QString dateFormat = xlsx.read(row, 2).toString().replace(" ","/");
                    QStringList dateMonthFormat = dateFormat.split("/");
                    dateFormat = QString("%1/%2/%3").arg(dateMonthFormat[0]).arg(dateFormat.split("/")[1].toUpper()).arg(dateMonthFormat[2]);
                    QDate monthNumber = locale.toDate(dateFormat, "d/MMM/yyyy");
                    t.date = QString("%3-%2-%1").arg(dateFormat.split("/")[0]).arg(monthNumber.toString("MM")).arg(dateFormat.split("/")[2]);
                }

                t.category = m_classifier.classify(fullDescription);
                
                t.description = fullDescription;
                t.description.remove(QRegularExpression("\\s*(el\\s*)?\\d{1,2}/\\d{1,2}/\\d{4}"));
                t.description.remove(QRegularExpression("\\s*(el\\s*)?\\d{4}-\\d{1,2}-\\d{1,2}"));
                t.description.remove(QRegularExpression("\\s*(a las\\s*)?\\d{1,2}:\\d{2}\\s*(hrs\\.?)?"));
                t.description.remove(QRegularExpression(":\\d{2}\\s*(hrs\\.?)?"));
                t.description.remove(QRegularExpression(",?\\s*monto\\s*\\$?\\s*[\\d.,]+"));
                t.description.remove(QRegularExpression(",?\\s*Rut\\s*[\\d.]+[-][\\dkK]"));
                t.description.remove(QRegularExpression("[,\\s]+$"));
                t.description = t.description.trimmed();

                t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

                qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
                Bank::transactions.append(t);
            }
            row++;
        }
    }
    return false;
}