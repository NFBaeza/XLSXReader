#include <QCoreApplication>
#include "bankFactory.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    //auto bank = BankFactory::create(BankFactory::BankType::BICE, "debit");

    // Opción 2: 
    auto bank = BankFactory::create("Wise", "debit");
    //se recomenda el uso de ruta global
    const QString filePathGlobal ="../files/transaction-history.csv"; 

    if (bank) {
        bank->readBankMovements(filePathGlobal);
        qDebug() << bank->nameBank;
    }

    return 0;
}