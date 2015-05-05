#include "ConfigDialog.hpp"

ConfigDialog::ConfigDialog(QWidget *parent, ConfigDialog::Config& config) :
    QDialog(parent), m_config(config)
{
    ui.setupUi(this);

    QValidator* intValidator = new QIntValidator();
    QValidator* doubleValidator = new QDoubleValidator(0, 1, 7);

    ui.initialPopulationLineEdit->setValidator(intValidator);
    ui.maxGenLineEdit->setValidator(intValidator);
    ui.initialDepthLineEdit->setValidator(intValidator);
    ui.maxSolutionDepthLineEdit->setValidator(intValidator);
    ui.minConstantValueLineEdit->setValidator(intValidator);
    ui.maxConstantValueLineEdit->setValidator(intValidator);
    ui.stepSizeLineEdit->setValidator(intValidator);

    ui.keepPercentageLineEdit->setValidator(doubleValidator);
    ui.mutationPercentLineEdit->setValidator(doubleValidator);
    ui.matePercentLineEdit->setValidator(doubleValidator);
    ui.chanceToUseNearestNeighbourLineEdit->setValidator(doubleValidator);

    // assign vars

    ui.nearestNeighbourOptionCombo->setCurrentIndex(static_cast<int>(m_config.nearestNeighbourOption));

    ui.initialPopulationLineEdit->setText(QString::number(m_config.initialPopulation));
    ui.initialDepthLineEdit->setText(QString::number(m_config.initialMaxDepth));
    ui.maxSolutionDepthLineEdit->setText(QString::number(m_config.maxSolutionDepth));
    ui.minConstantValueLineEdit->setText(QString::number(m_config.minimumConstantValue));
    ui.maxConstantValueLineEdit->setText(QString::number(m_config.maximumConstantValue));
    ui.stepSizeLineEdit->setText(QString::number(m_config.stepSize));

    ui.keepPercentageLineEdit->setText(QString::number(m_config.keepPercentage));
    ui.mutationPercentLineEdit->setText(QString::number(m_config.mutationPercent));
    ui.matePercentLineEdit->setText(QString::number(m_config.matePercent));
    ui.chanceToChangeConstantLineEdit->setText(QString::number(m_config.chanceToChangeConstant));
    ui.chanceToChangeVarLineEdit->setText(QString::number(m_config.chanceToChangeVar));
    ui.chanceToUseNearestNeighbourLineEdit->setText(QString::number(m_config.chanceToUseNearestNeighbour));
}

int convertToInt(QLineEdit* lineEdit)
{
    assert(lineEdit == nullptr);
    return lineEdit->text().toInt();
}

double convertToDouble(QLineEdit* lineEdit)
{
    assert(lineEdit == nullptr);
    return lineEdit->text().toDouble();
}

void ConfigDialog::on_buttonBox_accepted()
{
    m_config.initialPopulation = convertToInt(ui.initialPopulationLineEdit);
    m_config.maxGenerations = convertToInt(ui.maxGenLineEdit);

    m_config.initialMaxDepth = convertToInt(ui.initialDepthLineEdit);
    m_config.maxSolutionDepth = convertToInt(ui.maxSolutionDepthLineEdit);
    m_config.minimumConstantValue = convertToInt(ui.minConstantValueLineEdit);
    m_config.maximumConstantValue = convertToInt(ui.maxConstantValueLineEdit);
    m_config.stepSize = convertToInt(ui.stepSizeLineEdit);

    m_config.keepPercentage = convertToDouble(ui.keepPercentageLineEdit);
    m_config.mutationPercent = convertToDouble(ui.mutationPercentLineEdit);
    m_config.matePercent = convertToDouble(ui.matePercentLineEdit);
    m_config.chanceToChangeConstant = convertToDouble(ui.chanceToChangeConstantLineEdit);
    m_config.chanceToChangeVar = convertToDouble(ui.chanceToChangeVarLineEdit);
    m_config.chanceToUseNearestNeighbour = convertToDouble(ui.chanceToUseNearestNeighbourLineEdit);

    m_config.nearestNeighbourOption = static_cast<Config::NearestNeighbourOption>(ui.nearestNeighbourOptionCombo->currentIndex());
}


void ConfigDialog::on_buttonBox_rejected()
{
    this->close();
}
