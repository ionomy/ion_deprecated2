#ifndef STASHEDSENDCONFIG_H
#define STASHEDSENDCONFIG_H

#if defined(HAVE_CONFIG_H)
#include "config/ion-config.h"
#endif

#include <QDialog>

namespace Ui {
    class StashedsendConfig;
}
class WalletModel;

/** Multifunctional dialog to ask for passphrases. Used for encryption, unlocking, and changing the passphrase.
 */
class StashedsendConfig : public QDialog
{
    Q_OBJECT

public:

    StashedsendConfig(QWidget *parent = 0);
    ~StashedsendConfig();

    void setModel(WalletModel *model);


private:
    Ui::StashedsendConfig *ui;
    WalletModel *model;
    void configure(bool enabled, int coins, int rounds);

private slots:

    void clickBasic();
    void clickHigh();
    void clickMax();
};

#endif // STASHEDSENDCONFIG_H
