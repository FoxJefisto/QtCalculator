#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QStack>
#include <QtWidgets>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    QPushButton* createButton(const QString& str);
    void binaryCalculate();
    void unaryCalculate();

public slots:
    void slotButtonClicked();

private:
    QVector<QPushButton*> m_Buttons;
    QLCDNumber* m_plcd;
    QGridLayout* m_gridLayout;
    QStack<QString> m_stk;
    QString m_strDisplay;
    QStringList m_binaryOperations;
    QStringList m_unaryOperations;

    QPushButton* getButtonByName(QString name);
    void setUIDesign();


protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};
#endif // CALCULATOR_H
