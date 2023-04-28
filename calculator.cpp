#include "calculator.h"
#include <QtMath>
#include <QtAlgorithms>
#include <QRegion>

Calculator::Calculator(QWidget *parent)
    : QWidget{parent}
{
    m_binaryOperations = QStringList {"/", "*", "-", "+", "%"};
    m_unaryOperations = QStringList {"^2", "sqrt", "1/x"};
    m_plcd = new QLCDNumber(12);
    m_plcd->setSegmentStyle(QLCDNumber::Flat);
    m_plcd->setMinimumSize(150,50);
    QString aButtons[4][5] = {{"7", "8", "9", "/", "%"},
                            {"4", "5", "6", "*", "^2"},
                            {"1", "2", "3", "-", "sqrt"},
                            {"0", ".", "=", "+", "1/x"}};
    m_gridLayout = new QGridLayout;
    m_gridLayout->setSpacing(10);
    m_gridLayout->addWidget(m_plcd, 0, 0, 1, 5);
    m_gridLayout->addWidget(createButton("C"), 1, 3);
    m_gridLayout->addWidget(createButton("CE"), 1, 4);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++){
            m_gridLayout->addWidget(createButton(aButtons[i][j]), i + 2, j);
        }
    }
    setLayout(m_gridLayout);
    setUIDesign();
}

void Calculator::binaryCalculate(){
    double operand2 = m_stk.pop().toDouble();
    QString strOperation = m_stk.pop();
    double operand1 = m_stk.pop().toDouble();
    double result = 0;
    if(strOperation == "+"){
        result = operand1 + operand2;
    }
    if(strOperation == "-"){
        result = operand1 - operand2;
    }
    if(strOperation == "/"){
        result = operand1 / operand2;
    }
    if(strOperation == "*"){
        result = operand1 * operand2;
    }
    if(strOperation == "%"){
        result = operand1 / 100 * operand2;
    }
    m_plcd->display(result);
}

void Calculator::unaryCalculate(){
    QString strOperation = m_stk.pop();
    double operand = m_stk.pop().toDouble();
    double result = 0;
    if(strOperation == "^2"){
        result = qPow(operand, 2);
    }
    if(strOperation == "sqrt"){
        result = qSqrt(operand);
    }
    if(strOperation == "1/x"){
        result = 1. / operand;
    }
    m_plcd->display(result);
}

QPushButton* Calculator::createButton(const QString &str){
    QPushButton* pcmd = new QPushButton(str);
    pcmd->setCursor(Qt::PointingHandCursor);
    pcmd->setMinimumSize(40,40);
    connect(pcmd, SIGNAL(clicked()), SLOT(slotButtonClicked()));
    m_Buttons.push_back(pcmd);
    return pcmd;
}

void Calculator::slotButtonClicked(){
    QString btnName = ((QPushButton*)sender())->text();
    if(btnName == "C"){
        m_strDisplay = QString().setNum(m_plcd->value());
        m_strDisplay.chop(1);
        if(m_strDisplay.isEmpty()){
            m_strDisplay = "0";
        }
        m_plcd->display(m_strDisplay);
    }
    if(btnName == "CE") {
        m_stk.clear();
        m_strDisplay = "";
        m_plcd->display("0");
        return;
    }
    if (QRegExp("[0-9]").exactMatch(btnName)){
        m_strDisplay += btnName;
        m_plcd->display(m_strDisplay.toDouble());
    }
    if (btnName == "." && m_strDisplay.count(".") == 0) {
        m_strDisplay += btnName;
        m_plcd->display(m_strDisplay);
    }
    if(m_unaryOperations.indexOf(btnName) != -1){
        m_stk.push(m_strDisplay);
        m_stk.push(btnName);
        unaryCalculate();
        m_stk.clear();
        m_stk.push(QString().setNum((m_plcd->value())));
        m_strDisplay = QString::number(m_plcd->value());
    }
    if(QRegExp("[^0-9\\.]+").exactMatch(btnName) && m_stk.count() >= 2){
        m_stk.push(QString().setNum(m_plcd->value()));
        binaryCalculate();
        m_stk.clear();
        m_stk.push(QString().setNum((m_plcd->value())));
        m_strDisplay = QString::number(m_plcd->value());
        if(btnName != "="){
            m_stk.push(btnName);
        }
    }
    if(m_binaryOperations.indexOf(btnName) != -1){
        m_stk.push(QString().setNum(m_plcd->value()));
        m_stk.push(btnName);
        m_strDisplay="";
    }
}

void Calculator::keyPressEvent(QKeyEvent *event)
{
    if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9){
        QString name = QString::number(event->key() - Qt::Key_0);
        QPushButton* btn = getButtonByName(name);
        btn->click();
    }
    if(event->key() == Qt::Key_Plus){
        QPushButton* btn = getButtonByName("+");
        btn->click();
    }
    if(event->key() == Qt::Key_Minus){
        QPushButton* btn = getButtonByName("-");
        btn->click();
    }
    if(event->key() == Qt::Key_Asterisk){
        QPushButton* btn = getButtonByName("*");
        btn->click();
    }
    if(event->key() == Qt::Key_Slash){
        QPushButton* btn = getButtonByName("/");
        btn->click();
    }
    if(event->key() == Qt::Key_Equal || event->key() == Qt::Key_Return){
        QPushButton* btn = getButtonByName("=");
        btn->click();
    }
    if(event->key() == Qt::Key_Period){
        QPushButton* btn = getButtonByName(".");
        btn->click();
    }
    if(event->key() == Qt::Key_Backspace){
        QPushButton* btn = getButtonByName("C");
        btn->click();
    }
}

QPushButton* Calculator::getButtonByName(QString name){
    for(auto btn : m_Buttons){
        if(btn->text() == name){
            return btn;
        }
    }
    return nullptr;
}

void Calculator::setUIDesign(){
    this->setWindowIcon(QIcon(":resource/resources/icon"));
    QPalette pal = this->palette();
    pal.setColor(this->backgroundRole(), Qt::black);
    this->setPalette(pal);
    pal = m_plcd->palette();
    pal.setColor(m_plcd->backgroundRole(), QColor(82,163,1));
    m_plcd->setPalette(pal);
    m_plcd->setAutoFillBackground(true);
    for(auto btn : m_Buttons){
        if(QRegExp("[0-9\\.=]").exactMatch(btn->text())){
            btn->setStyleSheet("QPushButton {background-color: white; color: black; border-radius: 10px}");
        }
        else if(QRegExp("C|CE").exactMatch(btn->text())){
            btn->setStyleSheet("QPushButton {background-color: rgb(251,100,12); color: white; border-radius: 10px}");
        }
        else {
            btn->setStyleSheet("QPushButton {background-color: rgb(78,74,66); color: white; border-radius: 10px}");
        }
    }
}
