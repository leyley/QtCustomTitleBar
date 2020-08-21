#ifndef WINDOWFRAME_H
#define WINDOWFRAME_H

#include <QFrame>

namespace Ui {
    class WindowFrame;
}

class WindowFrame : public QFrame
{
    Q_OBJECT

    public:
    explicit WindowFrame(QWidget *parent = nullptr, QWidget *child=nullptr);
    ~WindowFrame();

    private slots:
    void on_close_clicked();

    void on_maximum_clicked();

    void on_minimum_clicked();

    private:
    Ui::WindowFrame *ui;
    QPoint position;
    int borderSize;
    protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void setBorderSize(int sizeOfBorder);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    public:
    void setTitle(QString &displayName);
    void setIcon(QIcon &icon);
    void addHeaderButton(QWidget *widget);
    void enableMinimum(bool enable);
    void enableMaximum(bool enable);
    void enableClose(bool enable);
};

#endif // WINDOWFRAME_H
