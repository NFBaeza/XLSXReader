#include "bankFactory.h"
#include "banks/biceBank.h"
#include "banks/santanderBank.h" 

std::unique_ptr<Bank> BankFactory::create(BankType type, const QString& accountType) {
    switch (type) {
        case BankType::BICE:
            return std::make_unique<BiceBank>(accountType);

        case BankType::SANTANDER:
            return std::make_unique<SantanderBank>(accountType);

        default:
            qDebug() << "Tipo de banco no soportado";
            return nullptr;
    }
}

std::unique_ptr<Bank> BankFactory::create(const QString& bankName, const QString& accountType) {
    QString name = bankName.toUpper();

    if (name == "BICE") {
        return std::make_unique<BiceBank>(accountType);
    }
    else if (name == "SANTANDER") {
         return std::make_unique<SantanderBank>(accountType);
    }

    qDebug() << "Banco no soportado:" << bankName;
    return nullptr;
}

std::unique_ptr<Bank> BankFactory::create(const QString& bankName, const QString& accountType, const QString& filePath) {
    QString name = bankName.toUpper();

    if (name == "BICE") {
        return std::make_unique<BiceBank>(accountType, filePath);
    }
    else if (name == "SANTANDER") {
         return std::make_unique<SantanderBank>(accountType, filePath);
    }

    qDebug() << "Banco no soportado:" << bankName;
    return nullptr;
}
