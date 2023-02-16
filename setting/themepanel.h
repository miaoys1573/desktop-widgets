#ifndef THEMEPANEL_H
#define THEMEPANEL_H

#include <base/basesetting.h>

class ThemePanel : public BaseSetting
{
    Q_OBJECT

private:
    void initUI();
    void initSignalSlots();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

public:
    ThemePanel(QWidget *parent = 0);
};

#endif // THEMEPANEL_H
