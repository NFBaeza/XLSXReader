#ifndef BANKFACTORY_H
#define BANKFACTORY_H

#include <memory>
#include <QString>
#include "bank.h"

class BankFactory {
public:
    enum class BankType {
        BICE,
        SANTANDER,
    };

    static std::unique_ptr<Bank> create(BankType type, const QString& accountType);

    static std::unique_ptr<Bank> create(const QString& bankName, const QString& accountType);

    static std::unique_ptr<Bank> create(const QString& bankName, const QString& accountType, const QString& filePath);
};

#endif // BANKFACTORY_H
