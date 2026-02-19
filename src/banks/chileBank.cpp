#include "banks/chileBank.h"
#include <xls.h>

ChileBank::ChileBank(const QString& type)
    : Bank("Chile", type) {
}

ChileBank::ChileBank(const QString& type, const QString& filePath)
    : Bank("Chile", type, filePath) {
}

bool ChileBank::readBankMovementsDebit(const QString& filePath){
    Bank::transactions.clear();
    QString year = "";

    xls::xls_error_t error = xls::LIBXLS_OK;
    xls::xlsWorkBook* fileContent = xls::xls_open_file(filePath.toUtf8().constData(), "UTF-8", &error);

    if (!fileContent) {
        qDebug() << "Error opening file:" << filePath << "-" << xls::xls_getError(error);
        return false;
    }

    xls::xlsWorkSheet* sheetContent = xls::xls_getWorkSheet(fileContent, 0);
    xls::xls_parseWorkSheet(sheetContent);

    for (xls::WORD row = 13; row <= sheetContent->rows.lastrow; ++row) {
        QString rowStr;
        for (xls::WORD col = 0; col <= sheetContent->rows.row[row].lcell; ++col) {
            xls::xlsCell* cell = xls::xls_cell(sheetContent, row, col);
            if (cell && !cell->isHidden)
                rowStr += QString::fromUtf8(cell->str) + ",";
            else
                rowStr += ",";
        }
        //qDebug() << "Row" << row << ":" << rowStr;

        QStringList dataOnList =  rowStr.split(",");
        if(row == 13){
            year = dataOnList[4].split("/")[2].trimmed();
        }

        if(row > 25){
            if(dataOnList[2].trimmed() == "SALDO FINAL"){
                xls::xls_close_WS(sheetContent);
                xls::xls_close_WB(fileContent);
                return true;      
            } else {
                Transaction t;
                QString date = QString("%1/%2").arg(dataOnList[1].trimmed()).arg(year);
                t.date = castQDateTime(date).toString("yyyy-MM-dd");
                
                t.description = dataOnList[2].trimmed();

                bool isTransferTo   = QRegularExpression("TRASPASO A:",   QRegularExpression::CaseInsensitiveOption).match(t.description).hasMatch();
                bool isTransferFrom = QRegularExpression("TRASPASO DE:", QRegularExpression::CaseInsensitiveOption).match(t.description).hasMatch();

                if(dataOnList[4].isEmpty()){
                    t.amount = dataOnList[5].split(".")[0];
                    t.category = isTransferFrom ? "deposit" : m_classifier.classify(t.description);
                } else {
                    t.amount = dataOnList[4].split(".")[0];
                    t.category = isTransferTo ? "bank transfer" : m_classifier.classify(t.description);
                }

                t.description = t.description.trimmed();
                t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

                qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
                Bank::transactions.append(t);
            }
        }
        
    }

    xls::xls_close_WS(sheetContent);
    xls::xls_close_WB(fileContent);
    return false;
}

bool ChileBank::readBankMovementsCredit(const QString& filePath){
    Bank::transactions.clear();

    xls::xls_error_t error = xls::LIBXLS_OK;
    xls::xlsWorkBook* fileContent = xls::xls_open_file(filePath.toUtf8().constData(), "UTF-8", &error);

    if (!fileContent) {
        qDebug() << "Error opening file:" << filePath << "-" << xls::xls_getError(error);
        return false;
    }

    xls::xlsWorkSheet* sheetContent = xls::xls_getWorkSheet(fileContent, 0);
    xls::xls_parseWorkSheet(sheetContent);

    for (xls::WORD row = 18; row <= sheetContent->rows.lastrow; ++row) {
        QString rowStr;
        for (xls::WORD col = 0; col <= sheetContent->rows.row[row].lcell; ++col) {
            xls::xlsCell* cell = xls::xls_cell(sheetContent, row, col);
            if (cell && !cell->isHidden)
                rowStr += QString::fromUtf8(cell->str) + "|";
            else
                rowStr += "|";
        }
        //qDebug() << "Row" << row << ":" << rowStr;

        QStringList dataOnList =  rowStr.split("|");

        if(dataOnList[2].isEmpty()){
            xls::xls_close_WS(sheetContent);
            xls::xls_close_WB(fileContent);
            return true;      
        } else {
            Transaction t;
            t.date = castQDateTime(dataOnList[2].trimmed()).toString("yyyy-MM-dd");

            t.description = dataOnList[3].trimmed();
            t.amount = dataOnList[7].split(".")[0];
            t.category = m_classifier.classify(t.description);
            t.account = QString("%1 %2").arg(nameBank).arg(typeAccount);

            if(dataOnList[6] != "01/01"){
                int numberPayments = dataOnList[6].split("/")[1].toInt();
                int currentPayment = dataOnList[6].split("/")[0].toInt();
                if(currentPayment == 0){
                    continue;
                }
                currentPayment++;
                for(currentPayment; currentPayment <= numberPayments; currentPayment++){
                    Transaction nextPayment = t;
                    QDateTime nextPaymentDate = castQDateTime(dataOnList[2].trimmed());
                    nextPaymentDate = nextPaymentDate.addMonths(currentPayment-1);
                    nextPayment.date = nextPaymentDate.toString("yyyy-MM-dd");
                    qDebug() << "nextPayment" << nextPayment.date << nextPayment.category << nextPayment.description << nextPayment.amount;

                    Bank::transactions.append(nextPayment);   
                }
            }
            qDebug() << "Transaction" << t.date << t.category << t.description << t.amount;
            Bank::transactions.append(t);
        }
    }

    xls::xls_close_WS(sheetContent);
    xls::xls_close_WB(fileContent);
    return false;
}