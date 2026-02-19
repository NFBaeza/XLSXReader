#ifndef WISEBANK_H
#define WISEBANK_H

#include "bank.h"
#include <QFile>

class WiseBank : public Bank {
    Q_OBJECT

public:
    WiseBank(const QString& type);
    WiseBank(const QString& type, const QString& filePath);
    ~WiseBank() = default;
    bool readBankMovementsDebit(const QString& filePath) override;
    bool readBankMovementsCredit(const QString& filePath) override;
};

#endif // WISEBANK_H
