/* Copyright (C) 2007 - 2010 Jan Kundrát <jkt@flaska.net>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or version 3 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef IMAP_FETCHMSGPARTTASK_H
#define IMAP_FETCHMSGPARTTASK_H

#include <QPersistentModelIndex>
#include "ImapTask.h"
#include "Parser/Parser.h"

namespace Imap {
namespace Mailbox {

class TreeItemMailbox;
class TreeItemPart;

/** @short Fetch a message part */
class FetchMsgPartTask : public ImapTask
{
Q_OBJECT
public:
    FetchMsgPartTask( Model* _model, TreeItemMailbox* mailbox, TreeItemPart* part );
    virtual void perform();

    virtual bool handleFetch( Imap::Parser* ptr, const Imap::Responses::Fetch* const resp );
    virtual bool handleStateHelper( Imap::Parser* ptr, const Imap::Responses::State* const resp );
private:
    CommandHandle tag;
    ImapTask* conn;
    QPersistentModelIndex index;
};

}
}

#endif // IMAP_FETCHMSGPARTTASK_H