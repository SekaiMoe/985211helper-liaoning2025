#include <sstream>

#include "window.h"
#include <base.h>

#include <sstream>
#include <QFile>
#include <QTextStream>

Window::Window(QObject *parent) : QObject(parent) {}

bool Window::hasGUI()
{
    #if defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
        return !qgetenv("DISPLAY").isEmpty();
    #else
        return QGuiApplication::platformName() != "offscreen";
    #endif
}

int Window::runGUI(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    if (!hasGUI()) {
        return -1; // 表示无法运行GUI
    }
    
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

void Window::updateProfessions(const QString &university)
{
    std::vector<std::string> professions;
    search::listProfessions(university.toStdString(), professions);
    if (professions.empty()) {
        std::cout << "No professions found for university: " << university.toStdString() << std::endl;
    } else {
        std::cout << "Professions found: " << professions.size() << std::endl;
    }
    QStringList professionList;
    for (const auto &profession : professions)
    {
        professionList.append(QString::fromStdString(profession));
    }
    
    emit professionsUpdated(professionList);
}

void Window::performSearch(const QString &university, const QString &profession)
{
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

void Window::viewMetadata()
{
    const char* filePath = "./metadata.json";
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    view::view(filePath);

    std::cout.rdbuf(old);

    emit metadataViewed(QString::fromStdString(buffer.str()));
}

#include "main.moc"
