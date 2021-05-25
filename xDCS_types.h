#ifndef XDCS_TYPES_H
#define XDCS_TYPES_H

#include <QVariant>
#include <QFileInfo>
#include <QMetaType>
#include <QDateTime>
#include <QPixmap>

#include <QDataStream>


enum VARIANT_TYPES
{
    VT_READ_FILE = QMetaType::User,
    VT_SAVE_FILE = QMetaType::User+1,
    VT_READ_DIR  = QMetaType::User+2,
    VT_SAVE_DIR  = QMetaType::User+3,
    VT_ACTOR     = QMetaType::User+4,
    VT_WDGT_USER_DATA = QMetaType::User+5,
    VT_3D_SAMPLE_POS = QMetaType::User+5
};

struct LIMITS
{
    float _lowerLimit=0.0f;
    float _upperLimit=1.0f;
    float _reduction=1.0f;
};

enum SIG_TYPE
{
    ST_MSG,
    ST_ERROR_MSG,
    ST_WARN_MSG,
    ST_ABOUT_TO_CLOSE,
    ST_ADD_OBJECT,
    ST_OBJECT_ADDED,
    ST_REMOVE_OBJECT,
    ST_OBJECT_REMOVED,
    ST_SET_PROCESS,
    ST_SET_PROCESS_RANGE,
    ST_SET_PROCESS_TXT,
    ST_INC_PROCESS,
    ST_CAM_IMAGE,
    ST_RECORDING_FINISHED
};


enum xVSCALAR_TYPE
{
    ST_CHAR     = 0x00,
    ST_UCHAR    = 0x01,
    ST_SHORT    = 0x02,
    ST_USHORT   = 0x03,
    ST_INT      = 0x04,
    ST_UINT     = 0x05,
    ST_FLOAT    = 0x06,
    ST_DOUBLE   = 0x07
};

struct xPROP_TYPE
{
public:
    quint32 _id=0;
    QVariant _value;
    QString  _subGrp="";
    QString  _subGrpIDStr="";
    bool _enabled = true;

    void save(QDataStream &d) const;
    xPROP_TYPE(){};
    xPROP_TYPE(const xPROP_TYPE& other){
        _id=other._id;
        _value=other._value;
        _subGrp=other._subGrp;
        _enabled = other._enabled;
    }
    xPROP_TYPE(QDataStream& d)
    {
        d >> _id;
        d >> _value;
        d >> _subGrp;
        d >> _subGrpIDStr;
        d >> _enabled;
    }
};

struct xLimitedInt
{
public:
    xLimitedInt(){}
    xLimitedInt(const quint32& v, const quint32& l, const quint32& h){_value=v;_lowerLimit=l;_upperLimit=h;}
    xLimitedInt(const xLimitedInt& other){_value=other._value;_lowerLimit=other._lowerLimit;_upperLimit=other._upperLimit;}
    qint32 _value=0,_lowerLimit=0,_upperLimit=0;
    void setValue(const qint32& v){_value=v;}
    void setLowerLimit(const qint32& v){_lowerLimit=v;}
    void setUpperLimit(const qint32& v){_upperLimit=v;}
};

struct xLimitedDouble
{
public:
    xLimitedDouble(){}
    xLimitedDouble(const double& v,const double& l,const double& u,const double& p){
        _value=v;_lowerLimit=l;_upperLimit=u;_precision=p;}
    xLimitedDouble(const xLimitedDouble& other){
        _value=other._value;_lowerLimit=other._lowerLimit;_upperLimit=other._upperLimit;_precision=other._precision;}
    double _value=0.0,_lowerLimit=0.0,_upperLimit=0.0,_precision=0.0;
};

struct x3D_SAMPLE_POS
{
public:
    quint32 x,y,z;
    x3D_SAMPLE_POS(){x=y=z=0;}
    x3D_SAMPLE_POS(const long& v){x=y=z=v;}
    x3D_SAMPLE_POS(const long&xx,const long& yy,const long& zz){x=xx;y=yy;z=zz;}
};

class xFileName
{
public:
    enum TYPE
    {
        FN_INPUT_FILE,
        FN_OUTPUT_FILE,
        FN_INPUT_DIR,
        FN_OUTPUT_DIR
    };

    TYPE _type  = FN_INPUT_FILE;
    QString _fileName = "";
    bool _relative = false;

    xFileName()
    {
    }
    xFileName(const xFileName& other)
    {
        _type = other._type;
        _fileName = other._fileName;
        _relative = other._relative;
    }
    xFileName(const TYPE& t,const QString& name,bool rel=false)
    {
        _type=t;
        _fileName=name;
        _relative=rel;
    }

    bool exists(){QFileInfo info(_fileName);return info.exists();}
    bool isFile(){QFileInfo info(_fileName);return info.isFile();}
};

Q_DECLARE_METATYPE(x3D_SAMPLE_POS);
Q_DECLARE_METATYPE(xLimitedInt);
Q_DECLARE_METATYPE(xLimitedDouble);
Q_DECLARE_METATYPE(xFileName);
typedef QMap<QString,xPROP_TYPE> xParamMap;
Q_DECLARE_METATYPE(xParamMap);

QDataStream &operator<<(QDataStream &out, const xFileName &myObj);
QDataStream &operator>>(QDataStream &in, xFileName &myObj);
QDataStream &operator<<(QDataStream &out, const QPointF &myObj);
QDataStream &operator>>(QDataStream &in, QPointF &myObj);
QDataStream &operator<<(QDataStream &out, const x3D_SAMPLE_POS &myObj);
QDataStream &operator>>(QDataStream &in, x3D_SAMPLE_POS &myObj);
QDataStream &operator<<(QDataStream &out, const xLimitedInt &myObj);
QDataStream &operator>>(QDataStream &in, xLimitedInt &myObj);
QDataStream &operator<<(QDataStream &out, const xLimitedDouble &myObj);
QDataStream &operator>>(QDataStream &in, xLimitedDouble &myObj);
QDataStream &operator<<(QDataStream &out, const xParamMap &myObj);
QDataStream &operator>>(QDataStream &in, xParamMap &myObj);

extern QMap<QString,LIMITS> _limitMp;
extern QMap<QString,xPROP_TYPE> _settings;
extern QMap<QString,QStringList> _optionLsts;

void initAll();

#endif // XDCS_TYPES_H
