#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "qqmlapplicationengine.h"
#include "qqmlcontext.h"
#include <QGuiApplication>

class Window : public QObject
{
    Q_OBJECT

public:
    explicit Window(QObject *parent = nullptr);
    static bool hasGUI();
    static int runGUI(int argc, char *argv[]);

public slots:
    void updateProfessions(const QString &university);
    void performSearch(const QString &university, const QString &profession);

signals:
    void professionsUpdated(const QStringList &professions);
    void searchCompleted(const QString &result);
};

#endif // WINDOW_H
