#ifndef SANTADERBANK_H
#define SANTANDERBANK_H

#include "bank.h"

class SantanderBank : public Bank {
    Q_OBJECT

public:
    SantanderBank(const QString& type);
    SantanderBank(const QString& type, const QString& filePath);
    ~SantanderBank() = default;
    bool readBankMovementsDebit(const QString& filePath) override;
    bool readBankMovementsCredit(const QString& filePath) override;
};

#endif // BICEBANK_H
