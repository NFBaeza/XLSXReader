#ifndef SIMPLECLASSIFIER_H
#define SIMPLECLASSIFIER_H

#include <QString>
#include <QList>
#include <QRegularExpression>
#include <QPair>

class SimpleClassifier {
public:
    struct Rule {
        QRegularExpression pattern;
        QString category;
    };

    SimpleClassifier();

    QString classify(const QString& description) const;

private:
    QList<Rule> m_rules;
    void initRules();
};

#endif // SIMPLECLASSIFIER_H
