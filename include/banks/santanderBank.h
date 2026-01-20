#ifndef SANTADERBANK_H
#define SANTANDERBANK_H

#include "bank.h"

class SantanderBank : public Bank {
    Q_OBJECT

public:
    SantanderBank(const QString& type);
    SantanderBank(const QString& type, const QString& filePath);
    ~SantanderBank() = default;

    QList<Transaction> readBankMovements(const QString& filePath) override;

protected:
   
    bool readBankMovements() override;
};

#endif // BICEBANK_H
