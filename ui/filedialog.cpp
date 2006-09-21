#include "filedialog.h"

#include <QSettings>
#include <QFileDialog>
#include <QString>


FileDialog::FileDialog (QWidget *parent, const QString &name) : QFileDialog (parent)
{
	QSettings s;
	m_name = name;

	if (!s.contains("filedialog/" + name))
		s.setValue ("filedialog/" + name, QDir::homePath ());

	setDirectory (s.value("filedialog/" + name).toString ());
}

QStringList
FileDialog::getFiles ()
{
	QSettings s;
	QStringList ret = getOpenFileNames (NULL, "Choose files");
	if (ret.size() > 0)
		s.setValue ("filedialog/" + m_name, directory ().absolutePath ());

	qSort (ret);

	return ret;

}

QString
FileDialog::getDirectory ()
{
	QSettings s;
	QString ret = getExistingDirectory (NULL, "Choose directory");
	if (!ret.isNull())
		s.setValue ("filedialog/" + m_name, directory ().absolutePath ());

	return ret;

}
