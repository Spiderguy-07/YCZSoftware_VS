#include <QDialog>
#include <qgsproject.h>
#include "ui_SearchBySQLDialog.h"

class SearchBySQLDialog : public QDialog
{
	Q_OBJECT
public:
	SearchBySQLDialog();
	~SearchBySQLDialog();

private:
	Ui::SearchBySQLDialog* ui;
};

