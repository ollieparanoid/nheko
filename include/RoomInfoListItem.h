/*
 * nheko Copyright (C) 2017  Konstantinos Sideris <siderisk@auth.gr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QAction>
#include <QDateTime>
#include <QSharedPointer>
#include <QWidget>

#include <mtx/responses.hpp>

#include "RoomState.h"

class Menu;
class RippleOverlay;
class RoomSettings;

struct DescInfo
{
        QString username;
        QString userid;
        QString body;
        QString timestamp;
        QDateTime datetime;
};

class RoomInfoListItem : public QWidget
{
        Q_OBJECT
        Q_PROPERTY(QColor highlightedBackgroundColor READ highlightedBackgroundColor WRITE
                     setHighlightedBackgroundColor)
        Q_PROPERTY(
          QColor hoverBackgroundColor READ hoverBackgroundColor WRITE setHoverBackgroundColor)
        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

        Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor)
        Q_PROPERTY(QColor subtitleColor READ subtitleColor WRITE setSubtitleColor)

        Q_PROPERTY(
          QColor highlightedTitleColor READ highlightedTitleColor WRITE setHighlightedTitleColor)
        Q_PROPERTY(QColor highlightedSubtitleColor READ highlightedSubtitleColor WRITE
                     setHighlightedSubtitleColor)

        Q_PROPERTY(QColor btnColor READ btnColor WRITE setBtnColor)
        Q_PROPERTY(QColor btnTextColor READ btnTextColor WRITE setBtnTextColor)

public:
        RoomInfoListItem(QSharedPointer<RoomSettings> settings,
                         QSharedPointer<RoomState> state,
                         QString room_id,
                         QWidget *parent = 0);

        RoomInfoListItem(QString room_id, mtx::responses::InvitedRoom room, QWidget *parent = 0);

        void updateUnreadMessageCount(int count);
        void clearUnreadMessageCount() { updateUnreadMessageCount(0); };
        void setState(QSharedPointer<RoomState> state)
        {
                state_ = state;
                update();
        }

        QString roomId();
        bool isPressed() const { return isPressed_; };
        QSharedPointer<RoomState> state() const { return state_; }
        int unreadMessageCount() const { return unreadMsgCount_; };

        void setAvatar(const QImage &avatar_image);
        void setDescriptionMessage(const DescInfo &info);
        DescInfo lastMessageInfo() const { return lastMsgInfo_; }

        QColor highlightedBackgroundColor() const { return highlightedBackgroundColor_; }
        QColor hoverBackgroundColor() const { return hoverBackgroundColor_; }
        QColor backgroundColor() const { return backgroundColor_; }

        QColor highlightedTitleColor() const { return highlightedTitleColor_; }
        QColor highlightedSubtitleColor() const { return highlightedSubtitleColor_; }

        QColor titleColor() const { return titleColor_; }
        QColor subtitleColor() const { return subtitleColor_; }
        QColor btnColor() const { return btnColor_; }
        QColor btnTextColor() const { return btnTextColor_; }

        void setHighlightedBackgroundColor(QColor &color) { highlightedBackgroundColor_ = color; }
        void setHoverBackgroundColor(QColor &color) { hoverBackgroundColor_ = color; }
        void setBackgroundColor(QColor &color) { backgroundColor_ = color; }

        void setHighlightedTitleColor(QColor &color) { highlightedTitleColor_ = color; }
        void setHighlightedSubtitleColor(QColor &color) { highlightedSubtitleColor_ = color; }

        void setTitleColor(QColor &color) { titleColor_ = color; }
        void setSubtitleColor(QColor &color) { subtitleColor_ = color; }

        void setBtnColor(QColor &color) { btnColor_ = color; }
        void setBtnTextColor(QColor &color) { btnTextColor_ = color; }

signals:
        void clicked(const QString &room_id);
        void leaveRoom(const QString &room_id);
        void acceptInvite(const QString &room_id);
        void declineInvite(const QString &room_id);

public slots:
        void setPressedState(bool state);

protected:
        void mousePressEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        void contextMenuEvent(QContextMenuEvent *event) override;

private:
        void init(QWidget *parent);
        QString roomName()
        {
                if (roomType_ == RoomType::Joined)
                        return state_->getName();

                return roomName_;
        }

        QString notificationText();

        RippleOverlay *ripple_overlay_;

        enum class RoomType
        {
                Joined,
                Invited,
        };

        RoomType roomType_ = RoomType::Joined;

        // State information for the joined rooms.
        QSharedPointer<RoomState> state_;

        // State information for the invited rooms.
        mtx::responses::InvitedRoom invitedRoom_;

        QString roomId_;
        QString roomName_;

        DescInfo lastMsgInfo_;

        QPixmap roomAvatar_;

        Menu *menu_;
        QAction *toggleNotifications_;
        QAction *leaveRoom_;

        QSharedPointer<RoomSettings> roomSettings_;

        bool isPressed_ = false;

        int unreadMsgCount_ = 0;

        QColor highlightedBackgroundColor_;
        QColor hoverBackgroundColor_;
        QColor backgroundColor_;

        QColor highlightedTitleColor_;
        QColor highlightedSubtitleColor_;

        QColor titleColor_;
        QColor subtitleColor_;

        QColor btnColor_;
        QColor btnTextColor_;

        QRectF acceptBtnRegion_;
        QRectF declineBtnRegion_;
};

inline QString
RoomInfoListItem::roomId()
{
        return roomId_;
}
