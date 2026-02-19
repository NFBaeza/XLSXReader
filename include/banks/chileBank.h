#ifndef CHILEBANK_H
#define CHILEBANK_H

#include "bank.h"
#include <QFile>


 extern "C" {
   #include "xls.h"
 }

class ChileBank : public Bank {
    Q_OBJECT

public:
    ChileBank(const QString& type);
    ChileBank(const QString& type, const QString& filePath);
    ~ChileBank() = default;
    bool readBankMovementsDebit(const QString& filePath) override;
    bool readBankMovementsCredit(const QString& filePath) override;
};

#endif // CHILEBANK_H
