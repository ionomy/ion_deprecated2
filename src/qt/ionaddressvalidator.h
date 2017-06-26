#ifndef IONADDRESSVALIDATOR_H
#define IONADDRESSVALIDATOR_H

#include <QValidator>

/** Base48 entry widget validator.
   Corrects near-miss characters and refuses characters that are no part of base48.
 */
class IonAddressValidator : public QValidator
{
    Q_OBJECT

public:
    explicit IonAddressValidator(QObject *parent = 0);

    State validate(QString &input, int &pos) const;

    static const int MaxAddressLength = 128;
};

#endif // IONADDRESSVALIDATOR_H
