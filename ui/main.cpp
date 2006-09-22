#include <xmmsclient/xmmsclient++.h>

#include <QTreeView>
#include <QErrorMessage>
#include <QHeaderView>
#include <QApplication>
#include <QImageReader>
#include <QSettings>
#include <QColor>

#include "xclient.h"
#include "playerwidget.h"
#include "serverdialog.h"

int
main (int argc, char **argv)
{
	QApplication app(argc, argv);
	QApplication::setDesktopSettingsAware (false);

	QCoreApplication::setOrganizationName("XMMS2");
	QCoreApplication::setOrganizationDomain("xmms.org");
	QCoreApplication::setApplicationName("simpleqt");

	QSettings s;
	QFont f = app.font ();
	f.setPixelSize (s.value ("ui/fontsize", 10).toInt ());
	QApplication::setFont (f);

	/* base palette */
	QPalette p (app.palette ());
	p.setColor (QPalette::Highlight,
				s.value ("ui/highlight", QColor (80, 80, 80)).value<QColor> ());
	p.setColor (QPalette::HighlightedText,
				s.value ("ui/highlightedtext", QColor (Qt::black)).value<QColor> ());
	p.setColor (QPalette::Inactive, QPalette::Text, QColor (Qt::black));
	QApplication::setPalette (p);

	XClient client (NULL, "SimpleQt");

	QString path;

browser:
	if (!getenv ("XMMS_PATH")) {
		ServerDialog sd (NULL);
		if (!s.value ("serverdialog/show", true).toBool ()) {
			path = sd.get_default ();
		} else {
			path = sd.get_path ();
		}
		if (path == "local") {
			path = "";
		} else if (path.isNull ()) {
			return EXIT_FAILURE;
		}
	} else {
		path = QString::fromAscii (getenv ("XMMS_PATH"));
	}

	PlayerWidget *pw = new PlayerWidget (NULL, &client);

	if (!client.connect (path.toStdString ()))
		goto browser;

	pw->show ();

	return app.exec ();
}

