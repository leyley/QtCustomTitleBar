#include "windowframe.h"
#include "ui_windowframe.h"
#include <QMouseEvent>
#ifdef WIN32
#include <windows.h>
#include <windowsx.h>
#endif

WindowFrame::WindowFrame(QWidget *parent, QWidget *child) :
QFrame(parent),
       ui(new Ui::WindowFrame)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->borderSize=3;
    if(child!=nullptr) {
        this->ui->body->layout()->addWidget(child);
        QString title=child->windowTitle();
        this->setTitle(title);
        QIcon icon=child->windowIcon();
        this->setIcon(icon);
        this->resize(child->size());
    }
    //this->ui->minimum->setDisabled(true);
}

WindowFrame::~WindowFrame()
{
    int subBodies=this->ui->body->layout()->count();
    if(subBodies>0) {
        for(int i=0; i<subBodies; i++) {
            QWidget *subBody=this->ui->body->layout()->itemAt(i)->widget();
            delete subBody;
        }
    }
    delete ui;
}

void WindowFrame::on_close_clicked()
{
    this->close();
}

void WindowFrame::on_maximum_clicked()
{
    if(this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}

void WindowFrame::on_minimum_clicked()
{
    this->showMinimized();
}

void WindowFrame::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        QWidget* widget = this->childAt(event->x(), event->y());
        if(widget==this->ui->title || widget==this->ui->buttons) {
            this->position.setX(event->x());
            this->position.setY(event->y());
        }
    }
}

void WindowFrame::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        if (this->position.x() != 0 || this->position.y() != 0) {
            this->move(event->globalX() - this->position.x(), event->globalY() - this->position.y());
        }
    }
}

void WindowFrame::mouseReleaseEvent(QMouseEvent *event) {
    this->position.setX(0);
    this->position.setY(0);
}

void WindowFrame::setBorderSize(int sizeOfBorder) {
    this->borderSize=sizeOfBorder;
}

bool WindowFrame::nativeEvent(const QByteArray &eventType, void *message, long *result) {
#ifdef WIN32
    Q_UNUSED(eventType)
    MSG *param = static_cast<MSG *>(message);
    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();
        if (childAt(nX, nY) != this->ui->header && childAt(nX, nY) != this->ui->body)
            return QWidget::nativeEvent(eventType, message, result);
        *result = HTCAPTION;
        if ((nX > 0) && (nX < this->borderSize))
            *result = HTLEFT;
        if ((nX > this->width() - this->borderSize) && (nX < this->width()))
            *result = HTRIGHT;
        if ((nY > 0) && (nY < this->borderSize))
            *result = HTTOP;
        if ((nY > this->height() - this->borderSize) && (nY < this->height()))
            *result = HTBOTTOM;
        if ((nX > 0) && (nX < this->borderSize) && (nY > 0)
                && (nY < this->borderSize))
            *result = HTTOPLEFT;
        if ((nX > this->width() - this->borderSize) && (nX < this->width())
                && (nY > 0) && (nY < this->borderSize))
            *result = HTTOPRIGHT;
        if ((nX > 0) && (nX < this->borderSize)
                && (nY > this->height() - this->borderSize) && (nY < this->height()))
            *result = HTBOTTOMLEFT;
        if ((nX > this->width() - this->borderSize) && (nX < this->width())
                && (nY > this->height() - this->borderSize) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;
        return true;
    }
    }
#endif
    return QWidget::nativeEvent(eventType, message, result);
}

void WindowFrame::setTitle(QString & displayName) {
    this->setWindowTitle(displayName);
    this->ui->displayName->setText(displayName);
}

void WindowFrame::setIcon(QIcon &icon) {
    if(icon.isNull()) {
        return;
    }
    this->ui->icon->setPixmap(icon.pixmap(this->ui->icon->pixmap()->size()));
    this->setWindowIcon(icon);
}
