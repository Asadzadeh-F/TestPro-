/**************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
**************************************************************************/

#ifndef Q7Z_EXTRACT_H
#define Q7Z_EXTRACT_H

#include "q7z_global.h"

#include <Common/MyCom.h>
#include <7zip/Archive/IArchive.h>

#include <QString>

class CArc;

QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE

namespace Q7z
{
    class Q7Z_EXPORT ExtractCallback : public IArchiveExtractCallback, public CMyUnknownImp
    {
        Q_DISABLE_COPY(ExtractCallback)

    public:
        ExtractCallback()
        {
            arc = 0;
            total = 0;
            completed = 0;
            currentIndex = 0;
        }
        virtual ~ExtractCallback()
        {

        }

        void setArchive(CArc *carc) { arc = carc; }
        void setTarget(const QString &dir) { targetDir = dir; }

        MY_UNKNOWN_IMP
        INTERFACE_IArchiveExtractCallback(;)

    protected:
        virtual bool prepareForFile(const QString & /*filename*/) { return true; }
        virtual void setCurrentFile(const QString &filename) { Q_UNUSED(filename) }
        virtual HRESULT setCompleted(quint64 /*completed*/, quint64 /*total*/) { return S_OK; }

    private:
        CArc *arc;

        QString targetDir;
        quint64 total;
        quint64 completed;
        quint32 currentIndex ;
    };

    void Q7Z_EXPORT extractArchive(QFile *archive, const QString &targetDirectory,
        ExtractCallback *callback = 0);

} // namespace Q7z

#endif // Q7Z_EXTRACT_H
