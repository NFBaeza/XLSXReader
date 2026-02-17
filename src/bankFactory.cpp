#include "bankFactory.h"
#include "banks/biceBank.h"
#include "banks/santanderBank.h" 
#include "banks/wiseBank.h" 

std::unique_ptr<Bank> BankFactory::create(BankType type, const QString& accountType) {
    switch (type) {
        case BankType::BICE:      return std::make_unique<BiceBank>(accountType);
        case BankType::SANTANDER: return std::make_unique<SantanderBank>(accountType);
        case BankType::WISE:      return std::make_unique<WiseBank>(accountType);
        case BankType::UNKNOWN:
            qDebug() << "Tipo de banco no soportado";
            return nullptr;
    }
    return nullptr;
}

std::unique_ptr<Bank> BankFactory::create(const QString& bankName, const QString& accountType, const QString& filePath) {
    switch (fromString(bankName)) {
        case BankType::BICE:      return std::make_unique<BiceBank>(accountType, filePath);
        case BankType::SANTANDER: return std::make_unique<SantanderBank>(accountType, filePath);
        case BankType::WISE:      return std::make_unique<WiseBank>(accountType, filePath);
        case BankType::UNKNOWN:
            qDebug() << "Banco no soportado:" << bankName;
            return nullptr;
    }
    return nullptr;
}

std::unique_ptr<Bank> BankFactory::create(const QString& bankName, const QString& accountType) {
    return create(fromString(bankName), accountType);
}

BankFactory::BankType BankFactory::fromString(const QString& bankName) {
    static const QHash<QString, BankType> map = {
        {"BICE", BankType::BICE},
        {"SANTANDER", BankType::SANTANDER},
        {"WISE", BankType::WISE}
    };
    return map.value(bankName.toUpper(), BankType::UNKNOWN);
}
