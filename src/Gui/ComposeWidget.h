/* Copyright (C) 2006 - 2013 Jan Kundrát <jkt@flaska.net>

   This file is part of the Trojita Qt IMAP e-mail client,
   http://trojita.flaska.net/

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef COMPOSEWIDGET_H
#define COMPOSEWIDGET_H

#include <QList>
#include <QPersistentModelIndex>
#include <QUrl>
#include <QWidget>

#include "Composer/Recipients.h"

namespace Ui
{
class ComposeWidget;
}

class QAbstractListModel;
class QComboBox;
class QLineEdit;
class QMenu;
class QPushButton;

namespace Composer {
class Submission;
}

namespace MSA {
class MSAFactory;
}

namespace Gui
{

class MainWindow;

/** @short A "Compose New Mail..." dialog

  Implements a widget which can act as a standalone window for composing e-mail messages.
  Uses Imap::Mailbox::MessageComposer as a backend for composing a message. @see Imap::Mailbox::MessageComposer

  */
class ComposeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComposeWidget(MainWindow *mainWindow, MSA::MSAFactory *msaFactory);
    ~ComposeWidget();
    void setData(const QList<QPair<Composer::RecipientKind, QString> > &recipients,
                 const QString &subject,
                 const QString &body,
                 const QList<QByteArray> &inReplyTo, const QList<QByteArray> &references,
                 const QModelIndex &replyingToMessage);
    void loadDraft(const QString &path);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *ce);
    bool eventFilter(QObject *o, QEvent *e);

public slots:
    bool setReplyMode(const Composer::ReplyMode mode);

private slots:
    void collapseRecipients();
    void completeRecipient(QAction *act);
    void completeRecipients(const QString &text);
    void send();
    void gotError(const QString &error);
    void sent();
    void updateRecipientList();

    void slotCheckAddress();

    void slotAskForFileAttachment();
    void slotRemoveAttachment();
    void slotAttachFiles(QList<QUrl> urls);

    void slotUpdateSignature();

    void autoSaveDraft();
    void setMessageUpdated();

    void setUiWidgetsEnabled(const bool enabled);

private:
    static QByteArray extractMailAddress(const QString &text, bool &ok);
    static Composer::RecipientKind recipientKindForNextRow(const Composer::RecipientKind kind);
    void addRecipient(int position, Composer::RecipientKind kind, const QString &address);
    bool parseRecipients(QList<QPair<Composer::RecipientKind, Imap::Message::MailAddress> > &results, QString &errorMessage);
    void removeRecipient(int position);

    bool buildMessageData();
    bool shouldBuildMessageLocally() const;

    static QString killDomainPartFromString(const QString &s);

    void saveDraft(const QString &path);

    Ui::ComposeWidget *ui;
    QPushButton *sendButton;
    QPushButton *cancelButton;
    typedef QPair<QComboBox*, QLineEdit*> Recipient;
    QList<Recipient> m_recipients;
    QTimer *m_recipientListUpdateTimer;

    bool m_sentMail;
    /** @short Has it been updated since the last time we auto-saved it? */
    bool m_messageUpdated;
    /** @short Was this message ever editted by human?

    We have to track both of these. Simply changing the sender (and hence the signature) without any text being written
    shall not trigger automatic saving, but on the other hand changing the sender after something was already written
    is an important change.
    */
    bool m_messageEverEdited;
    bool m_explicitDraft;
    QString m_autoSavePath;

    bool m_appendUidReceived;
    uint m_appendUidValidity;
    uint m_appendUid;
    bool m_genUrlAuthReceived;
    QString m_urlauth;

    MainWindow *m_mainWindow;

    Composer::Submission *m_submission;
    QAction *m_actionRemoveAttachment;

    QMenu *m_completionPopup;
    QLineEdit *m_completionReceiver;
    int m_completionCount;


    ComposeWidget(const ComposeWidget &); // don't implement
    ComposeWidget &operator=(const ComposeWidget &); // don't implement
};

}

#endif // COMPOSEWIDGET_H
