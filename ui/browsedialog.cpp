/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2006 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */


#include "xclient.h"
#include "browsedialog.h"
#include "browsemodel.h"

#include <QDialog>
#include <QSettings>
#include <QSize>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QTreeView>
#include <QDialogButtonBox>

BrowseDialog::BrowseDialog (QWidget *parent, XClient *client) : QDialog (parent)
{
    setSizeGripEnabled(true);
    QGridLayout *grid = new QGridLayout (this);

	m_client = client;

	m_list = new QTreeView (this);

	m_model = new BrowseModel (this, client);
	m_list->setModel (m_model);
    m_list->setSelectionMode (QAbstractItemView::ExtendedSelection);
    m_list->setSelectionBehavior (QAbstractItemView::SelectRows);
	m_list->setRootIsDecorated (false);
//	m_list->setWrapping (true);
	m_list->setIndentation (0);
    //m_list->setResizeMode (QListView::Adjust);
    m_list->setEditTriggers (QAbstractItemView::EditKeyPressed);
    m_list->setContextMenuPolicy (Qt::CustomContextMenu);

    m_selections = new QItemSelectionModel (m_model);
	m_list->setSelectionModel (m_selections);

	connect (m_model, SIGNAL (dirChanged (QString)),
			 this, SLOT (dirChanged(const QString &)));

    grid->addWidget(m_list, 1, 0, 1, 6);
    connect (m_list, SIGNAL (activated (QModelIndex)), 
	         this, SLOT (setPath(const QModelIndex &)));
	/*
    QObject::connect(m_list, SIGNAL(customContextMenuRequested(QPoint)),
                     q, SLOT(_q_showContextMenu(QPoint)));
 */

    QHBoxLayout *box = new QHBoxLayout;
    box->setMargin(3);
    box->setSpacing(3);
    QSize tools(22, 22);

    QToolButton *toParentButton = new QToolButton (this);
    toParentButton->setIcon (style ()->standardPixmap (QStyle::SP_FileDialogToParent));
    toParentButton->setToolTip (tr("Parent Directory"));
    toParentButton->setAutoRaise (true);
    toParentButton->setFixedSize (tools);
    QObject::connect (toParentButton, SIGNAL(clicked ()), this, SLOT (navigateToPrevious ()));
    box->addWidget(toParentButton);

    QToolButton *listModeButton = new QToolButton (this);
    listModeButton->setIcon (style ()->standardPixmap (QStyle::SP_FileDialogListView));
    listModeButton->setToolTip (tr("List View"));
    listModeButton->setAutoRaise (true);
    listModeButton->setDown (true);
    listModeButton->setFixedSize (tools);
    box->addWidget(listModeButton);

    QToolButton *detailModeButton = new QToolButton (this);
    detailModeButton->setIcon(style ()->standardPixmap (QStyle::SP_FileDialogDetailedView));
    detailModeButton->setToolTip (tr("Detail View"));
    detailModeButton->setAutoRaise (true);
    detailModeButton->setFixedSize (tools);
    box->addWidget (detailModeButton);
    box->setSizeConstraint (QLayout::SetFixedSize);

    grid->addLayout(box, 0, 4, 1, 2);

    QLabel *lookInLabel = new QLabel (tr ("Look in:"), this);
    grid->addWidget (lookInLabel, 0, 0);

    // push buttons
	QDialogButtonBox *bbox = new QDialogButtonBox (Qt::Horizontal, this);
    QPushButton *acceptButton = new QPushButton (tr ("Open"), this);
    QObject::connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
	acceptButton->setDefault (true);
	bbox->addButton (acceptButton, QDialogButtonBox::AcceptRole);

    QPushButton *rejectButton = new QPushButton (tr ("Cancel"), this);
    QObject::connect(rejectButton, SIGNAL(clicked()), this, SLOT(reject()));
	bbox->addButton (rejectButton, QDialogButtonBox::RejectRole);
    grid->addWidget (bbox, 2, 0, 1, 6);

    m_lookInCombo = new QComboBox (this);
    m_lookInCombo->setInsertPolicy (QComboBox::NoInsert);
    m_lookInCombo->setDuplicatesEnabled (false);
    m_lookInCombo->setEditable (true);
    m_lookInCombo->setAutoCompletion (false);
    QObject::connect(m_lookInCombo, SIGNAL(activated(QString)),
					 this, SLOT(setPath(QString)));

    m_lookInEdit = new QLineEdit (m_lookInCombo);
    m_lookInCombo->setLineEdit (m_lookInEdit);
    m_lookInCombo->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    grid->addWidget (m_lookInCombo, 0, 1, 1, 3);

	QSettings s;
	if (!s.contains ("browsedialog/path"))
		s.setValue ("browsedialog/path", "");
	m_model->setPath (s.value("browsedialog/path").toString ());

    resize(530, 340);
}

QStringList
BrowseDialog::getFiles ()
{
	QStringList ret;
	if (exec () == QDialog::Accepted) {
		QModelIndexList list = m_selections->selectedIndexes ();
		for (int i = 0; i < list.size (); i++) {
			BrowseModelItem *item = m_model->itemByIndex (list.at (i));
			if (!item)
				continue;

			if (!item->isDir ())
				ret.append (item->data("path"));
		}
	}
	return ret;
}

void
BrowseDialog::accept ()
{
	QSettings s;
	s.setValue ("browsedialog/path", m_model->currentPath ());
	QDialog::accept ();
}

void
BrowseDialog::dirChanged (const QString &path)
{
	m_lookInCombo->clear ();
	/*
	QStringList l = path.split ('/', QString::SkipEmptyParts);
	for (int i = 0; i < l.size (); i ++)
		qDebug ("%s", qPrintable (l.at (i)));
		*/

	m_lookInCombo->insertItem (0, path);
}

void
BrowseDialog::navigateToPrevious ()
{
	QString dir = m_model->currentPath ();
	if (dir.endsWith ("/"))
		m_model->setPath (dir.left (dir.lastIndexOf ("/", -2) + 1));
	else
		m_model->setPath (dir.left (dir.lastIndexOf ("/") + 1));
}

void
BrowseDialog::setPath (const QModelIndex &index)
{
	BrowseModelItem *item = m_model->itemByIndex (index);
	if (item->isDir ()) {
		m_model->setPath (index);
	} else {
		m_client->playlist.addUrl (item->data("path").toStdString ());
	}
}

void
BrowseDialog::setPath (const QString &path)
{
	m_model->setPath (path);
}

