#include "xDCS_types.h"

QMap<QString,LIMITS> _limitMp;
QMap<QString,xPROP_TYPE> _settings;
QMap<QString,QStringList> _optionLsts;

void xPROP_TYPE::save(QDataStream &d) const
{
    d << _id;
    d << _value;
    d << _subGrp;
    QString ID="";
    d << ID;
    d << _enabled;
}
QDataStream &operator<<(QDataStream &out, const xParamMap &myObj){
    out << (quint16)myObj.count();
    for (xParamMap::const_iterator it=myObj.begin();it!=myObj.end();++it)
    {
        out << it.key();
        (*it).save(out);
    }
    return out;}
QDataStream &operator>>(QDataStream &in, xParamMap &myObj){
    quint16 count;
    in >> count;
    QString key;
    for (int i=0;i<count;++i)
    {
        in >> key;
        xPROP_TYPE _prop(in);
        myObj[key]=_prop;
    }
    return in;}
QDataStream &operator<<(QDataStream &out, const xFileName &myObj){
    out << myObj._fileName << myObj._type << myObj._relative;
    return out;}
QDataStream &operator>>(QDataStream &in, xFileName &myObj){
    QString s;
    in >> myObj._fileName >> myObj._type >> myObj._relative;
    return in;}
QDataStream &operator<<(QDataStream &out, const QPointF &myObj){
    out << myObj.x() << myObj.y();
    return out;}
QDataStream &operator>>(QDataStream &in, QPointF &myObj){
    qreal x,y;
    in >> x >> y;myObj=QPointF(x,y);
    return in;}
QDataStream &operator<<(QDataStream &out, const x3D_SAMPLE_POS &myObj){
    out << myObj.x << myObj.y << myObj.z;
    return out;}
QDataStream &operator>>(QDataStream &in, x3D_SAMPLE_POS &myObj){
    in >> myObj.x >> myObj.y >> myObj.z;
    return in;}
QDataStream &operator<<(QDataStream &out, const xLimitedInt &myObj){
    out << myObj._value << myObj._lowerLimit << myObj._upperLimit;
    return out;}
QDataStream &operator>>(QDataStream &in, xLimitedInt &myObj){
    in >> myObj._value >> myObj._lowerLimit >> myObj._upperLimit;
    return in;}
QDataStream &operator<<(QDataStream &out, const xLimitedDouble &myObj){
    out << myObj._value << myObj._lowerLimit << myObj._upperLimit << myObj._precision;
    return out;}
QDataStream &operator>>(QDataStream &in, xLimitedDouble &myObj){
    in >> myObj._value >> myObj._lowerLimit >> myObj._upperLimit >> myObj._precision;
    return in;}


void initAll()
{
    qRegisterMetaType<xFileName>("xFileName");
    qRegisterMetaType<x3D_SAMPLE_POS>("x3D_SAMPLE_POS");
    qRegisterMetaType<xLimitedInt>("xLimitedInt");
    qRegisterMetaType<xLimitedDouble>("xLimitedDouble");
    qRegisterMetaType<xParamMap>("xParamMap");

    qRegisterMetaTypeStreamOperators<xFileName>("xFileName");
    qRegisterMetaTypeStreamOperators<QPointF>("QPointF");
    qRegisterMetaTypeStreamOperators<x3D_SAMPLE_POS>("x3D_SAMPLE_POS");
    qRegisterMetaTypeStreamOperators<xLimitedInt>("xLimitedInt");
    qRegisterMetaTypeStreamOperators<xLimitedDouble>("xLimitedDouble");
    qRegisterMetaTypeStreamOperators<xParamMap>("xParamMap");
}
