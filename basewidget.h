#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QDialog>

class BaseWidget : public QDialog
{
    Q_OBJECT

private:
    int _width;
    int _height;
    QBrush brush;
    QPoint offPos;

    void initUI();
    void initSignalSlots();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

public:
    QString name;
    BaseWidget(QWidget *parent = 0);
    BaseWidget(QColor background, QWidget *parent = 0);
    void setBrush(QBrush brush);
};

#endif // BASEWIDGET_H
