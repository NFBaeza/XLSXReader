#include <QCoreApplication>
#include "bankFactory.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    //auto bank = BankFactory::create(BankFactory::BankType::BICE, "debit");

    // Opción 2: 
    auto bank = BankFactory::create("Santander", "debit");

    if (bank) {
        bank->readBankMovements("../files/santander_carlola_julio.xlsx");
        qDebug() << bank->nameBank;
    }

    return 0;
}