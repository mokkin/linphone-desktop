#include <QtDebug>

#include "../core/CoreManager.hpp"
#include "ContactsListProxyModel.hpp"

#include "ContactsListModel.hpp"

using namespace std;

// ===================================================================

ContactsListModel::ContactsListModel (QObject *parent): QAbstractListModel(parent) {
  m_linphone_friends = CoreManager::getInstance()->getCore()->getFriendsLists().front();

  // Init contacts with linphone friends list.
  for (const auto &friend_ : m_linphone_friends->getFriends()) {
    ContactModel *contact = new ContactModel(friend_);
    m_friend_to_contact[friend_.get()] = contact;
    m_list << contact;
  }
}

QHash<int, QByteArray> ContactsListModel::roleNames () const {
  QHash<int, QByteArray> roles;
  roles[Qt::DisplayRole] = "$contact";
  return roles;
}

QVariant ContactsListModel::data (const QModelIndex &index, int role) const {
  int row = index.row();

  if (row < 0 || row >= m_list.count())
    return QVariant();

  if (role == Qt::DisplayRole)
    return QVariant::fromValue(m_list[row]);

  return QVariant();
}

// -------------------------------------------------------------------

ContactModel *ContactsListModel::mapLinphoneFriendToContact (
  const shared_ptr<linphone::Friend> &friend_
) const {
  return m_friend_to_contact[friend_.get()];
}

ContactModel *ContactsListModel::mapSipAddressToContact (const QString &sipAddress) const {
  ContactModel *contact = m_friend_to_contact[
    m_linphone_friends->findFriendByUri(
      sipAddress.toStdString()
    ).get()
  ];

  qInfo() << "Map sip address to contact:" << sipAddress << "->" << contact;

  return contact;
}
