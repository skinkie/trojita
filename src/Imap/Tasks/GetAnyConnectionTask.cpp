/* Copyright (C) 2007 - 2011 Jan Kundrát <jkt@flaska.net>

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

#include "GetAnyConnectionTask.h"
#include <QTimer>
#include "KeepMailboxOpenTask.h"
#include "MailboxTree.h"
#include "OfflineConnectionTask.h"
#include "OpenConnectionTask.h"

namespace Imap
{
namespace Mailbox
{

GetAnyConnectionTask::GetAnyConnectionTask(Model *model) :
    ImapTask(model), newConn(0)
{
    QMap<Parser *,ParserState>::iterator it = model->m_parsers.begin();
    while (it != model->m_parsers.end()) {
        if (it->connState == CONN_STATE_LOGOUT) {
            // We cannot possibly use this connection
            ++it;
        } else {
            // we've found it
            break;
        }
    }

    if (it == model->m_parsers.end()) {
        // We're creating a completely new connection
        if (model->networkPolicy() == Model::NETWORK_OFFLINE) {
            // ...but we're offline -> too bad, got to fail
            newConn = new OfflineConnectionTask(model);
            newConn->addDependentTask(this);
        } else {
            newConn = model->m_taskFactory->createOpenConnectionTask(model);
            newConn->addDependentTask(this);
        }
    } else {
        parser = it.key();
        Q_ASSERT(parser);

        if (it->maintainingTask) {
            // The parser already has some maintaining task associated with it
            // We can't ignore the maintaining task, if only because of the IDLE
            newConn = it->maintainingTask;
            newConn->addDependentTask(this);
        } else {
            // The parser doesn't have anything associated with it, so we can go ahead and
            // register ourselves
            markAsActiveTask();
            QTimer::singleShot(0, model, SLOT(runReadyTasks()));
        }
    }
}

void GetAnyConnectionTask::perform()
{
    // This is special from most ImapTasks' perform(), because the activeTasks could have already been updated
    if (newConn) {
        // We're "dependent" on some connection, so we should update our parser (even though
        // it could be already set), and also register ourselves with the Model
        parser = newConn->parser;
        markAsActiveTask();
    }

    IMAP_TASK_CHECK_ABORT_DIE;

    // ... we don't really have to do any work here, just declare ourselves completed
    _completed();
}

bool GetAnyConnectionTask::isReadyToRun() const
{
    return ! isFinished() && ! newConn;
}


}
}
