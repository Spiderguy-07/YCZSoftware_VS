#include "MainWindow.h"
#include <QtWidgets/QApplication>

#include <qgsapplication.h>
#include <fstream>
#include <string>

#define CONFIG_PATH "./config/config.json"
#define QGS_PREFIX_PATH "$(OSGEO_HOME)/apps/qgis-ltr-dev"

int main(int argc, char *argv[])
{
    
    // read configuration file
    std::ifstream ifs(CONFIG_PATH);
    if (!ifs.good()) {
        return -1;
    }
    std::ostringstream strBuff;
    char tmp;
    while (strBuff && ifs.get(tmp)) {
        strBuff.put(tmp);
    }
    std::string configStr = strBuff.str();
    ifs.close();

    // pharse json str to json object
    Json::CharReaderBuilder jcrBuilder;
    std::unique_ptr<Json::CharReader> const jcr(jcrBuilder.newCharReader());
    Json::Value config;
    Json::String err;
    if (!jcr->parse(configStr.c_str(), configStr.c_str() + configStr.length(),
        &config, &err)) {
        return -1;
    }

    QgsApplication a(argc, argv, true);
    QgsApplication::setPrefixPath(QGS_PREFIX_PATH, true);
    QgsApplication::initQgis(); 

    MainWindow w(config);
    //w.setFixedHeight(60);
    //QString styleSheet = "QHeaderView::section { height: 50px; }"; // 设置标题栏高度为50px
    //w.setStyleSheet(styleSheet);
    w.menuBar()->setFixedHeight(32);
    w.show();
    return a.exec();
}
