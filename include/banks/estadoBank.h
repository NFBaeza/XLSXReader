#ifndef ESTADOBANK_H
#define ESTADOBANK_H

#include "bank.h"
#include <QLocale>

class EstadoBank : public Bank {
    Q_OBJECT

public:
    EstadoBank(const QString& type);
    EstadoBank(const QString& type, const QString& filePath);
    ~EstadoBank() = default;
    bool readBankMovementsDebit(const QString& filePath) override;
    bool readBankMovementsCredit(const QString& filePath) override;
};

#endif // ESTADOBANK_H
