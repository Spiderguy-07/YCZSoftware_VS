#include <qgsvectorlayer.h>



class FileLoadingService {
	public:
		static QList<QgsMapLayer*> loadVecLayers(QString dialogTitle, QString filterName, QgsProject* project, QWidget* parent);
		static QStringList open3DFileSelectDialog(QString dialogTitle, QString filterName, QWidget* parent);
		static QStringList openCSVFileSelectDialog(QString dialogTitle, QString filterName, QWidget* parent);

};