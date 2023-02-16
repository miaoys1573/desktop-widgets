#ifndef BASEPANEL_H
#define BASEPANEL_H

#include <QDialog>
#include <qboxlayout.h>

class BasePanel : public QDialog
{
    Q_OBJECT

private:
    void initUI();
    void loadStyleSheet();

protected:
    QVBoxLayout *layout;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

public:
    BasePanel(QWidget *parent = 0);
};

#endif // BASEPANEL_H
