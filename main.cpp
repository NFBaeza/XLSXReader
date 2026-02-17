#include <QCoreApplication>
#include "bankFactory.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    //auto bank = BankFactory::create(BankFactory::BankType::BICE, "debit");

    // Opción 2: 
    auto bank = BankFactory::create("Santander", "debit");
    //se recomenda el uso de ruta global
    const QString filePathGlobal ="../files/Cartola de cuenta Corriente - Febrero 2022.xlsx"; 

    if (bank) {
        bank->readBankMovements(filePathGlobal);
        qDebug() << bank->nameBank;
    }

    return 0;
}