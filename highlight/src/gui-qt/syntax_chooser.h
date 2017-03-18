#ifndef SYNTAX_CHOOSER_H
#define SYNTAX_CHOOSER_H

#include <QDialog>
#include <QString>

namespace Ui {
class syntax_chooser;
}

class syntax_chooser : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(syntax_chooser)
public:
    explicit syntax_chooser(QWidget *parent = 0);
    ~syntax_chooser();

    void setUnclearExtension(const QString& ext);

    void addSyntaxName(const QString& name);

    QString getSelection();

    bool getRememberFlag();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::syntax_chooser *ui;
};

#endif // SYNTAX_CHOOSER_H
