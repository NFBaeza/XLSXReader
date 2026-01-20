#ifndef BICEBANK_H
#define BICEBANK_H

#include "bank.h"

class BiceBank : public Bank {
    Q_OBJECT

public:
    BiceBank(const QString& type);
    BiceBank(const QString& type, const QString& filePath);
    ~BiceBank() = default;
    QList<Transaction> readBankMovements(const QString& filePath) override;

protected:
    bool readBankMovements() override;
};

#endif // BICEBANK_H
