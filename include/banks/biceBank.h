#ifndef BICEBANK_H
#define BICEBANK_H

#include "bank.h"

class BiceBank : public Bank {
    Q_OBJECT

public:
    BiceBank(const QString& type);
    BiceBank(const QString& type, const QString& filePath);
    ~BiceBank() = default;
    bool readBankMovementsDebit(const QString& filePath) override;
    bool readBankMovementsCredit(const QString& filePath) override;
};

#endif // BICEBANK_H
