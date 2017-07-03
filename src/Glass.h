#ifndef GLASS_H
#define GLASS_H

#include <QString>

class Glass
{
public:
    const QString& title() const { return _title; }
    double lambdaMin() const { return _lambdaMin; }
    double lambdaMax() const { return _lambdaMax; }

//    virtual double calcIndex(const double& lambda) const = 0;
//    virtual double calcIndex1(const double& lambda) const = 0;
//    virtual double calcIndex2(const double& lambda) const = 0;

private:
    QString _title;
    double _lambdaMin, _lambdaMax;

    void setLambdaMin(double value) { _lambdaMin = value; }
    void setLambdaMax(double value) { _lambdaMax = value; }

    friend class Catalog;
    friend class GlassEditor;
};


//class GlassReznik : public Glass
//{
//public:
//    double calcIndex(const double& lambda) const override { return 0; }
//    double calcIndex1(const double& lambda) const override { return 0; }
//    double calcIndex2(const double& lambda) const override { return 0; }
//};


//class GlassSelm : public Glass
//{
//public:
//    double calcIndex(const double& lambda) const override { return 0; }
//    double calcIndex1(const double& lambda) const override { return 0; }
//    double calcIndex2(const double& lambda) const override { return 0; }
//};


//class GlassShott : public Glass
//{
//public:
//    double calcIndex(const double& lambda) const override { return 0; }
//    double calcIndex1(const double& lambda) const override { return 0; }
//    double calcIndex2(const double& lambda) const override { return 0; }
//};


//class GlassCustom : public Glass
//{
//public:
//    double calcIndex(const double& lambda) const override { return 0; }
//    double calcIndex1(const double& lambda) const override { return 0; }
//    double calcIndex2(const double& lambda) const override { return 0; }
//};


#endif // GLASS_H
