#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <options.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // QML Engine
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // communicating between QML and C++
    QQmlContext *ctx = engine.rootContext();
    options connection ;
    ctx->setContextProperty("connection",&connection);

    connection.siftedText("ok done");



    return app.exec();
}
