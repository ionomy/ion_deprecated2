#if defined(HAVE_CONFIG_H)
#include "config/ion-config.h"
#endif

#include "stashedsendconfig.h"
#include "ui_stashedsendconfig.h"

#include "ionunits.h"
#include "guiconstants.h"
#include "optionsmodel.h"
#include "walletmodel.h"
#include "init.h"

#include <QMessageBox>
#include <QPushButton>
#include <QKeyEvent>
#include <QSettings>

StashedsendConfig::StashedsendConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StashedsendConfig),
    model(0)
{
    ui->setupUi(this);

    connect(ui->buttonBasic, SIGNAL(clicked()), this, SLOT(clickBasic()));
    connect(ui->buttonHigh, SIGNAL(clicked()), this, SLOT(clickHigh()));
    connect(ui->buttonMax, SIGNAL(clicked()), this, SLOT(clickMax()));
}

StashedsendConfig::~StashedsendConfig()
{
    delete ui;
}

void StashedsendConfig::setModel(WalletModel *model)
{
    this->model = model;
}

void StashedsendConfig::clickBasic()
{
    configure(true, 1000, 2);

    QString strAmount(IonUnits::formatWithUnit(
        model->getOptionsModel()->getDisplayUnit(), 1000 * COIN));
    QMessageBox::information(this, tr("Stashedsend Configuration"),
        tr(
            "Stashedsend was successfully set to basic (%1 and 2 rounds). You can change this at any time by opening Ion's configuration screen."
        ).arg(strAmount)
    );

    close();
}

void StashedsendConfig::clickHigh()
{
    configure(true, 1000, 8);

    QString strAmount(IonUnits::formatWithUnit(
        model->getOptionsModel()->getDisplayUnit(), 1000 * COIN));
    QMessageBox::information(this, tr("Stashedsend Configuration"),
        tr(
            "Stashedsend was successfully set to high (%1 and 8 rounds). You can change this at any time by opening Ion's configuration screen."
        ).arg(strAmount)
    );

    close();
}

void StashedsendConfig::clickMax()
{
    configure(true, 1000, 16);

    QString strAmount(IonUnits::formatWithUnit(
        model->getOptionsModel()->getDisplayUnit(), 1000 * COIN));
    QMessageBox::information(this, tr("Stashedsend Configuration"),
        tr(
            "Stashedsend was successfully set to maximum (%1 and 16 rounds). You can change this at any time by opening Ion's configuration screen."
        ).arg(strAmount)
    );

    close();
}

void StashedsendConfig::configure(bool enabled, int coins, int rounds) {

    QSettings settings;

    settings.setValue("nStashedsendRounds", rounds);
    settings.setValue("nAnonymizeIonAmount", coins);

    nStashedsendRounds = rounds;
    nAnonymizeIonAmount = coins;
}
