#include "windowframe.h"
#include "ui_windowframe.h"
#include <QMouseEvent>
#ifdef WIN32
#include <windows.h>
#include <windowsx.h>
#endif
#include <QDebug>

WindowFrame::WindowFrame(QWidget *parent, QWidget *child) :
QFrame(parent),
       ui(new Ui::WindowFrame)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //this->ui->verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
    this->setBorderSize(3);
    if(child!=nullptr) {
        this->ui->body->layout()->addWidget(child);
        QString title=child->windowTitle();
        this->setTitle(title);
        QIcon icon=child->windowIcon();
        this->setIcon(icon);
        this->mainBody=child;
        this->mainBody->installEventFilter(this);
        this->resize(child->size());
    }
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

void WindowFrame::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        QWidget* widget = this->childAt(event->x(), event->y());
        if(widget==this->ui->title || widget==this->ui->buttons) {
            if(this->isMaximized()) {
                this->showNormal();
            } else {
                this->showMaximized();
            }
        }
    }
}


void WindowFrame::setBorderSize(int sizeOfBorder) {
    this->borderSize=sizeOfBorder;
    this->ui->body->layout()->setMargin(this->borderSize);
    QString styleSheet=this->styleSheet();
    QString newBorderString=QString("border: %1px solid #2f96fe;").arg(this->borderSize);
    QString newStyleSheet=styleSheet.replace("border: 3px solid #2f96fe;",newBorderString);
    this->setStyleSheet(newStyleSheet);
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

void WindowFrame::addHeaderButton(QWidget *widget) {
    if(widget==nullptr) {
        return;
    }
    this->ui->customButtons->layout()->addWidget(widget);
}

void WindowFrame::enableMinimum(bool enable) {
    if(!enable) {
        this->ui->minimum->hide();
    } else {
        this->ui->minimum->show();
    }
}

void WindowFrame::enableMaximum(bool enable) {
    if(!enable) {
        this->ui->maximum->hide();
    } else {
        this->ui->maximum->show();
    }
}

void WindowFrame::enableClose(bool enable) {
    if(!enable) {
        this->ui->close->hide();
    } else {
        this->ui->close->show();
    }
}

bool WindowFrame::eventFilter(QObject *obj, QEvent *event) {
    if(obj==this->mainBody) {
        //qDebug()<<"Event received:"<<event->type();
        if(event->type()==QEvent::HideToParent) {
            this->hide();
            return true;
        }
        if(event->type()==QEvent::ShowToParent) {
            this->show();
            return true;
        }
        return QObject::eventFilter(obj,event);
    } else {
        return QFrame::eventFilter(obj,event);
    }
    return false;
}

