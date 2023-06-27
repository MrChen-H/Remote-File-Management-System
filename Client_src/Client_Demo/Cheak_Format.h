#ifndef CHEAK_FORMAT_H
#define CHEAK_FORMAT_H

#endif // CHEAK_FORMAT_H

#include "register.h"
#include "ui_register.h"
#include "QRegularExpression"
#include "QRegularExpressionMatch"
#include "QMessageBox"

bool Check_format(QString be_cheak_str,QString ruler,int max_number)
{
    QRegularExpression re(ruler);
    QRegularExpressionMatch res = re.match(be_cheak_str);
    if(res.hasMatch()==true||be_cheak_str.size()>max_number)
    {
       return false;
    }
    return true;
}

bool Check_telepohon(QString number)
{
    QRegularExpression re("[^0-9-()（）]");
    QRegularExpressionMatch res = re.match(number);
    if(res.hasMatch() || number.size()<7 ||number.size()>18)
    {
       return false;
    }
    return true;
}

bool Check_Email(QString Email)
{
    QRegularExpression re("\\w[-\\w.+]*@([A-Za-z0-9][-A-Za-z0-9]+\\.)+[A-Za-z]{2,14}");
    QRegularExpressionMatch res = re.match(Email);

    if(res.hasMatch())
    {
       return true;
    }
    return false;
}
