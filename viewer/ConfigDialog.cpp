#include "ConfigDialog.hpp"

#include "Validator.hpp"

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

    ui.keepPercentageLineEdit->setValidator(doubleValidator);
    ui.mutationPercentLineEdit->setValidator(doubleValidator);
    ui.matePercentLineEdit->setValidator(doubleValidator);
    ui.chanceToUseNearestNeighbourLineEdit->setValidator(doubleValidator);

    ui.stepSizeLineEdit->setValidator(validator<Value>());
    ui.solutionCritereaLineEdit->setValidator(validator<Value>());

    // assign vars

    ui.nearestNeighbourOptionCombo->setCurrentIndex(static_cast<int>(m_config.nearestNeighbourOption));
    ui.constantNodeCombo->setCurrentIndex(static_cast<int>(m_config.generateConstantNodes));

    ui.initialPopulationLineEdit->setText(QString::number(m_config.initialPopulation));
    ui.maxGenLineEdit->setText(QString::number(m_config.maxGenerations));
    ui.initialDepthLineEdit->setText(QString::number(m_config.initialMaxDepth));
    ui.maxSolutionDepthLineEdit->setText(QString::number(m_config.maxSolutionDepth));
    ui.minConstantValueLineEdit->setText(QString::number(m_config.constantDist.a()));
    ui.maxConstantValueLineEdit->setText(QString::number(m_config.constantDist.b()));
    ui.stepSizeLineEdit->setText(QString::number(m_config.stepSize));
    ui.solutionCritereaLineEdit->setText(QString::number(m_config.solutionCriterea));

    ui.keepPercentageLineEdit->setText(QString::number(m_config.keepPercentage));
    ui.mutationPercentLineEdit->setText(QString::number(m_config.mutationPercent));
    ui.matePercentLineEdit->setText(QString::number(m_config.matePercent));
    ui.chanceToChangeConstantLineEdit->setText(QString::number(m_config.chanceToChangeConstant));
    ui.chanceToChangeVarLineEdit->setText(QString::number(m_config.chanceToChangeVar));
    ui.chanceToUseNearestNeighbourLineEdit->setText(QString::number(m_config.chanceToUseNearestNeighbour));
}

int convertToInt(QLineEdit* lineEdit)
{
    assert(lineEdit);
    return lineEdit->text().toInt();
}

double convertToDouble(QLineEdit* lineEdit)
{
    assert(lineEdit);
    return lineEdit->text().toDouble();
}

namespace
{
    struct Assignment
    {
        void operator()(int& res, QLineEdit* lineEdit)
        {
            res = convertToInt(lineEdit);
        }

        void operator()(float& res, QLineEdit* lineEdit)
        {
            res = convertToDouble(lineEdit);
        }

        void operator()(double& res, QLineEdit* lineEdit)
        {
            res = convertToDouble(lineEdit);
        }
    };
}

template <class T>
void convert(T& t, QLineEdit* lineEdit)
{
    Assignment{}(t, lineEdit);
}

void ConfigDialog::on_buttonBox_accepted()
{
    convert(m_config.initialPopulation, ui.initialPopulationLineEdit);
    convert(m_config.maxGenerations, ui.maxGenLineEdit);

    convert(m_config.initialMaxDepth, ui.initialDepthLineEdit);
    convert(m_config.maxSolutionDepth, ui.maxSolutionDepthLineEdit);

    m_config.constantDist = decltype(m_config.constantDist){convertToInt(ui.minConstantValueLineEdit),
                                                            convertToInt(ui.maxConstantValueLineEdit)};

    convert(m_config.stepSize, ui.stepSizeLineEdit);

    convert(m_config.keepPercentage, ui.keepPercentageLineEdit);
    convert(m_config.matePercent, ui.matePercentLineEdit);
    convert(m_config.mutationPercent, ui.mutationPercentLineEdit);
    convert(m_config.chanceToChangeConstant, ui.chanceToChangeConstantLineEdit);
    convert(m_config.chanceToChangeVar, ui.chanceToChangeVarLineEdit);
    convert(m_config.chanceToUseNearestNeighbour, ui.chanceToUseNearestNeighbourLineEdit);
    convert(m_config.solutionCriterea, ui.solutionCritereaLineEdit);

    m_config.nearestNeighbourOption = static_cast<Config::NearestNeighbourOption>(ui.nearestNeighbourOptionCombo->currentIndex());
    m_config.generateConstantNodes = ui.constantNodeCombo->currentIndex();

    std::cout << "Config has been changed to:\n" << m_config << std::endl;
}

void ConfigDialog::on_buttonBox_rejected()
{
    this->close();
}
