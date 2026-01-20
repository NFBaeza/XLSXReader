#include "bank.h"

Bank::Bank(const QString& name, const QString& type) 
    : nameBank(name), typeAccount(type) {
}

Bank::Bank(const QString& name, const QString& type,  const QString& filePath) 
    : nameBank(name), typeAccount(type), filePath(filePath) {
}


void Bank::printBankFile(const QString& filePath) {
    QXlsx::Document xlsx(filePath);

    // Verificar si el archivo se abrió correctamente
    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return;
    }

    qDebug() << "Archivo abierto correctamente";

    // En QXlsx, filas y columnas empiezan en 1, no en 0
    for(int row = 1; row <= 25; row++){
        for(int col = 1; col <= 7; col++){
            QVariant data = xlsx.read(row, col);
            if (!data.isNull()) {
                qDebug() << "Fila:" << row << "Col:" << col << "Data:" << data.toString();
            }
        }
    }
    return;
}

void Bank::printBankFile() {
    QXlsx::Document xlsx(filePath);

    // Verificar si el archivo se abrió correctamente
    if (!xlsx.load()) {
        qDebug() << "Error: No se pudo abrir el archivo:" << filePath;
        return;
    }

    qDebug() << "Archivo abierto correctamente";

    // En QXlsx, filas y columnas empiezan en 1, no en 0
    for(int row = 1; row <= 25; row++){
        for(int col = 1; col <= 7; col++){
            QVariant data = xlsx.read(row, col);
            if (!data.isNull()) {
                qDebug() << "Fila:" << row << "Col:" << col << "Data:" << data.toString();
            }
        }
    }
    return;
}

QDateTime Bank::castQDateTime(const QString& dateString){
    QRegularExpression dateRegex("(\\d{1,2}/\\d{1,2}/\\d{4})");
    QRegularExpressionMatch dateMatch = dateRegex.match(dateString);

    QRegularExpression timeRegex("(\\d{1,2}:\\d{2})");
    QRegularExpressionMatch timeMatch = timeRegex.match(dateString);

    if (!dateMatch.hasMatch()) {
        qDebug() << "No se encontró fecha en:" << dateString;
        return QDateTime();
    }

    QString dateStr = dateMatch.captured(1);
    QDate date = QDate::fromString(dateStr, "d/M/yyyy");

    if (!date.isValid()) {
        qDebug() << "Error parseando fecha:" << dateStr;
        return QDateTime();
    }

    QTime time;
    if (timeMatch.hasMatch()) {
        QString timeStr = timeMatch.captured(1);
        time = QTime::fromString(timeStr, "h:mm");
        if (!time.isValid()) {
            time = QTime(0, 0);
        }
    } else {
        time = QTime(0, 0);
    }

    return QDateTime(date, time);
}   


bool Bank::uploadDataToDataBase(QSqlDatabase db, const QString& nameTable) {
    readBankMovements();
    QSqlQuery query(db);
    const QString instructionQuery = QString("INSERT INTO {} (date, amount, category, account, description) "
            "VALUES (:date, :amount, :category, :account, :description)").arg(nameTable);

    for(auto& t : transactions){
        query.prepare(instructionQuery);
    
        query.bindValue(":date", t.date);
        query.bindValue(":amount", t.amount);
        query.bindValue(":category", t.category);
        query.bindValue(":account", t.account);
        query.bindValue(":description", t.description);

        if (!query.exec()) {
            qDebug() << "[insertTransaction] ERROR:" << query.lastError().text();
            return false;
        }
        qDebug() << "[insertTransaction] Operation successful!";
    }
    return true;
}       
