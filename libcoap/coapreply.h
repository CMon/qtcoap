/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2015 Simon Stuerz <simon.stuerz@guh.guru>                *
 *                                                                         *
 *  This file is part of guh.                                              *
 *                                                                         *
 *  Guh is free software: you can redistribute it and/or modify            *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 2 of the License.                *
 *                                                                         *
 *  Guh is distributed in the hope that it will be useful,                 *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with guh. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef COAPREPLY_H
#define COAPREPLY_H

#include <QObject>
#include <QTimer>

#include "coappdu.h"
#include "coapoption.h"
#include "coaprequest.h"

class CoapReply : public QObject
{
    friend class Coap;

    Q_OBJECT
    Q_ENUMS(Error)

public:
    enum Error {
        NoError,
        HostNotFoundError,
        TimeoutError,
        InvalidUrlSchemeError,
        InvalidPduError
    };

    CoapRequest request() const;
    QByteArray payload() const;

    bool isFinished() const;
    bool isRunning() const;

    Error error() const;
    QString errorString() const;

    CoapPdu::ContentType contentType() const;
    CoapPdu::MessageType messageType() const;
    CoapPdu::StatusCode statusCode() const;

private:
    CoapReply(const CoapRequest &request, QObject *parent = 0);

    void appendPayloadData(const QByteArray &data);

    void setFinished();
    void setError(const Error &error);

    void resend();

    void setContentType(const CoapPdu::ContentType contentType = CoapPdu::TextPlain);
    void setMessageType(const CoapPdu::MessageType &messageType);
    void setStatusCode(const CoapPdu::StatusCode &statusCode);

    QTimer *m_timer;
    CoapRequest m_request;
    QByteArray m_payload;

    Error m_error;

    bool m_isFinished;
    int m_retransmissions;

    CoapPdu::ContentType m_contentType;
    CoapPdu::MessageType m_messageType;
    CoapPdu::StatusCode m_statusCode;

    // data for the request
    void setHostAddress(const QHostAddress &address);
    QHostAddress hostAddress() const;

    void setRequestPayload(const QByteArray &requestPayload);
    QByteArray requestPayload() const;

    void setRequestMethod(const CoapPdu::StatusCode &method);
    CoapPdu::StatusCode requestMethod() const;

    void setRequestData(const QByteArray &requestData);
    QByteArray requestData() const;

    QHostAddress m_hostAddress;
    CoapPdu::StatusCode m_requestMethod;
    QByteArray m_requestPayload;
    QByteArray m_requestData;

signals:
    void timeout();
    void finished();
    void error(const Error &code);
};

#endif // COAPREPLY_H
