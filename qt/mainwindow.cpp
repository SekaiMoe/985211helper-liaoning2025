#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QX11Info>
#include <iostream>

bool canDisplayXWindow() {
    if (QX11Info::isPlatformX11()) {
        std::cout << "当前在 X11 平台运行。" << std::endl;
        return true;
    } else {
        std::cout << "当前不在 X11 平台运行。" << std::endl;
        return false;
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (canDisplayXWindow()) {
        QApplication app(argc, argv);

        QWidget window;
        window.setWindowTitle("985/211查询");

        QVBoxLayout *layout = new QVBoxLayout(&window);

        QLabel *label = new QLabel("请输入大学和专业名称：");
        layout->addWidget(label);

        QLineEdit *universityInput = new QLineEdit();
        universityInput->setPlaceholderText("大学名称");
        layout->addWidget(universityInput);

        QLineEdit *professionInput = new QLineEdit();
        professionInput->setPlaceholderText("专业名称");
        layout->addWidget(professionInput);

        QPushButton *button = new QPushButton("查询");
        layout->addWidget(button);

        QLabel *resultLabel = new QLabel();
        layout->addWidget(resultLabel);

        // 按钮点击事件
        QObject::connect(button, &QPushButton::clicked, [&]() {
            QString university = universityInput->text();
            QString profession = professionInput->text();
            QString result = search::search(university, profession);
            resultLabel->setText(result);  // 显示结果
        });

        window.show();
    } else {
        // 提示不支持 GUI
        std::cout << "请在 X 窗口系统下运行此程序。" << std::endl;
        exit(1);
    }
    return app.exec();
}

