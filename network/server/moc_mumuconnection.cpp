/****************************************************************************
** Meta object code from reading C++ file 'mumuconnection.h'
**
** Created: Wed Apr 10 00:44:06 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mumuconnection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mumuconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MumuConnection[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      34,   15,   15,   15, 0x0a,
      55,   15,   15,   15, 0x0a,
      97,   15,   15,   15, 0x0a,
     115,   15,   15,   15, 0x0a,
     164,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MumuConnection[] = {
    "MumuConnection\0\0socketConnected()\0"
    "socketDisconnected()\0"
    "socketError(QAbstractSocket::SocketError)\0"
    "socketHostFound()\0"
    "socketStateChanged(QAbstractSocket::SocketState)\0"
    "processData()\0"
};

void MumuConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MumuConnection *_t = static_cast<MumuConnection *>(_o);
        switch (_id) {
        case 0: _t->socketConnected(); break;
        case 1: _t->socketDisconnected(); break;
        case 2: _t->socketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: _t->socketHostFound(); break;
        case 4: _t->socketStateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 5: _t->processData(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MumuConnection::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MumuConnection::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_MumuConnection,
      qt_meta_data_MumuConnection, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MumuConnection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MumuConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MumuConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MumuConnection))
        return static_cast<void*>(const_cast< MumuConnection*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int MumuConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
