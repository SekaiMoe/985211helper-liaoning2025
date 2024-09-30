#include <sstream>

#include "window.h"
#include <base.h>

Window::Window(QObject *parent) : QObject(parent) {}

bool Window::hasGUI() {
    #if defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
        return !qgetenv("DISPLAY").isEmpty();
    #else
        return true;
    #endif
}

int Window::runGUI(int argc, char *argv[]) {
/*    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    
    Window window;
    engine.rootContext()->setContextProperty("backend", &window);
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return app.exec();
}*/

//int Window::hasGUI(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Window window;
    engine.rootContext()->setContextProperty("backend", &window);
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return app.exec();
}

void Window::updateProfessions(const QString &university) {
    std::vector<std::string> professions;
    search::listProfessions(university.toStdString(), professions);
    
    QStringList professionList;
    for (const auto &profession : professions)
    {
        professionList.append(QString::fromStdString(profession));
    }
    
    emit professionsUpdated(professionList);
}

void Window::performSearch(const QString &university, const QString &profession) {
    if (university.isEmpty() || profession.isEmpty())
    {
        emit searchCompleted("请输入大学名称并选择专业");
        return;
    }

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    search::search(university.toStdString(), profession.toStdString());

    std::cout.rdbuf(old);

    emit searchCompleted(QString::fromStdString(buffer.str()));
}
