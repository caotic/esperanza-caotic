#ifndef __PLAYLIST_MODEL_H__
#define __PLAYLIST_MODEL_H__

#include <xmmsclient/xmmsclient++.h>

#include <QAbstractTableModel>
#include <QVariant>
#include <QHash>
#include <QIcon>

#include "xclient.h"

class PlaylistModel : public QAbstractItemModel
{
	Q_OBJECT
	public:
		PlaylistModel (QObject *parent, XClient *client);

		enum {
			DisplayRole = Qt::DisplayRole,
			ToolTipRole = Qt::ToolTipRole,
			CurrentEntryRole = 200,
			MedialibIdRole,
			AvailableRole
		};

		int rowCount (const QModelIndex &parent) const;
		int columnCount (const QModelIndex &parent) const;
		QVariant data (const QModelIndex &index, int role = DisplayRole) const;
		QVariant playlist_data (const QModelIndex &index, int role = DisplayRole) const;
		QVariant decoration_data (const QModelIndex &index, int role = DisplayRole) const;
		QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex &) const;
		QModelIndex parent (const QModelIndex &) const;
		QModelIndex index (int row, int column, const QModelIndex &idx = QModelIndex ()) const;
		QMimeData *mimeData (const QModelIndexList &list) const;
		bool dropMimeData (const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
		QStringList mimeTypes () const;
		
		void setColumns (const QList <QString> &);
		void setColumnFallback (const QList <QString> &);

		QList<QString> columns () const;

		XClient *m_client;
		QList < unsigned int > m_plist;

		QList<uint32_t> get_all_id ();

		void set_cached_size (int i, const QSize &size) {
			m_cached_size[i] = size;
		};

		QSize cached_size (int i) const {
			return m_cached_size[i];
		};

		QModelIndexList get_idxlist_by_id (uint32_t);

	public slots:
		void got_connection (XClient *);
		void entry_changed (uint32_t);

	private:
		bool handle_list (const Xmms::List< unsigned int > &list);
		bool handle_change (const Xmms::Dict &chg);
		bool handle_update_pos (const unsigned int &pos);

		void getInfo (unsigned int id) const;
		QList < uint32_t > getPosById (uint32_t id);

		QList < QString > m_columns;
		QList < QString > m_colfallback;

		uint32_t m_current_pos;

		QList<QSize> m_cached_size;

};

#endif
