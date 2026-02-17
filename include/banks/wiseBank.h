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
    QList<Transaction> readBankMovements(const QString& filePath) override;
    bool readBankMovements() override;
};

#endif // WISEBANK_H
