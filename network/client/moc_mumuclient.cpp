/****************************************************************************
** Meta object code from reading C++ file 'mumuclient.h'
**
** Created: Wed Apr 10 00:56:15 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mumuclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mumuclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MumuClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      30,   11,   11,   11, 0x08,
      41,   11,   11,   11, 0x08,
      55,   11,   11,   11, 0x08,
      73,   11,   11,   11, 0x08,
     122,   11,   11,   11, 0x08,
     183,  163,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MumuClient[] = {
    "MumuClient\0\0serverConnected()\0readFile()\0"
    "closeStream()\0hostServerFound()\0"
    "socketStateChanged(QAbstractSocket::SocketState)\0"
    "catchError(QAbstractSocket::SocketError)\0"
    "proxy,authenticator\0"
    "proxyAuthentication(QNetworkProxy,QAuthenticator*)\0"
};

void MumuClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MumuClient *_t = static_cast<MumuClient *>(_o);
        switch (_id) {
        case 0: _t->serverConnected(); break;
        case 1: _t->readFile(); break;
        case 2: _t->closeStream(); break;
        case 3: _t->hostServerFound(); break;
        case 4: _t->socketStateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 5: _t->catchError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 6: _t->proxyAuthentication((*reinterpret_cast< const QNetworkProxy(*)>(_a[1])),(*reinterpret_cast< QAuthenticator*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MumuClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MumuClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MumuClient,
      qt_meta_data_MumuClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MumuClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MumuClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MumuClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MumuClient))
        return static_cast<void*>(const_cast< MumuClient*>(this));
    return QObject::qt_metacast(_clname);
}

int MumuClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
