#include "FileLoadingService.h"

#include <qgsproject.h>
#include <qgsguiutils.h>
#include <QMessageBox>
#include <QTextStream>

QList<QgsMapLayer*> FileLoadingService::loadVecLayers(QString dialogTitle, QString filterName, QgsProject* project, QWidget* parent)
{
    QStringList lyrFileList;  // selected file paths
    QString enc;  // file encoding
    QString vecFileFilter = "OGR Layer(*.shp *.geojson *.gmt);; KML(*.kml)";  // construct file type filter
    QgsGuiUtils::openFilesRememberingFilter(filterName, vecFileFilter, lyrFileList, enc, dialogTitle);
    QList<QgsMapLayer*> lyrsToBeAdded;
    if (lyrFileList.isEmpty()) return lyrsToBeAdded;
    for (QString path : lyrFileList) {
        path = path.trimmed();
        QFileInfo f(path);
        QString baseName = f.completeBaseName();
        QgsVectorLayer::LayerOptions options(project->transformContext());
        options.loadDefaultStyle = false;
        QgsVectorLayer* lyr = new QgsVectorLayer(path, baseName, "ogr", options);  // load vector layer

        if (lyr != nullptr && lyr->isValid()) {
            lyrsToBeAdded.append(lyr);
        }
        else {
            delete lyr;  // delete invalid pointer
            QString msg = QString("%1 is not a recognized or valid data source.").arg(path);
            QMessageBox::warning(parent, "Unsupported data type", msg);
        }

    }

    return lyrsToBeAdded;
}

QStringList FileLoadingService::open3DFileSelectDialog(QString dialogTitle, QString filterName, QWidget* parent)
{
    QStringList lyrFileList;  // selected file paths
    QString enc;  // file encoding
    QString vecFileFilter = "OGR Layer(*.shp *.geojson *.gmt);; KML(*.kml)";  // construct file type filter
    QgsGuiUtils::openFilesRememberingFilter(filterName, vecFileFilter, lyrFileList, enc, dialogTitle);
    return lyrFileList;
}

QStringList FileLoadingService::openCSVFileSelectDialog(QString dialogTitle, QString filterName, QWidget* parent)
{
    QStringList txtFileList;
    QString enc;
    QString txtFileFilter = "TXT/CSV File(*.txt *.csv)";
    QgsGuiUtils::openFilesRememberingFilter(filterName, txtFileFilter, txtFileList, enc, dialogTitle);
    
    return txtFileList;
}
