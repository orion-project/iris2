#ifndef GLASS_H
#define GLASS_H

#include <QString>

class DispersionFormula;

class Glass
{
public:
    int id() const { return _id; }
    const QString& title() const { return _title; }
    double lambdaMin() const { return _lambdaMin; }
    double lambdaMax() const { return _lambdaMax; }
    DispersionFormula* formula() const { return _formula; }

//    virtual double calcIndex(const double& lambda) const = 0;
//    virtual double calcIndex1(const double& lambda) const = 0;
//    virtual double calcIndex2(const double& lambda) const = 0;

protected:
    Glass(DispersionFormula* formula) : _formula(formula) {}

    double _lambdaMin, _lambdaMax;
    DispersionFormula* _formula;

private:
    int _id;
    QString _title;

    friend class Catalog;
    friend class GlassEditor;
    friend class GlassManager;
};


class GlassShott : public Glass
{
public:
//    double calcIndex(const double& lambda) const override { return 0; }
//    double calcIndex1(const double& lambda) const override { return 0; }
//    double calcIndex2(const double& lambda) const override { return 0; }

private:
    GlassShott(DispersionFormula* formula) : Glass(formula) {}
    friend class ShottFormula;
};


class GlassSellmeier : public Glass
{
public:
//    double calcIndex(const double& lambda) const override { return 0; }
//    double calcIndex1(const double& lambda) const override { return 0; }
//    double calcIndex2(const double& lambda) const override { return 0; }

private:
    GlassSellmeier(DispersionFormula* formula) : Glass(formula) {}
    friend class SellmeierFormula;
};


class GlassCustom : public Glass
{
public:
//    double calcIndex(const double& lambda) const override { return 0; }
//    double calcIndex1(const double& lambda) const override { return 0; }
//    double calcIndex2(const double& lambda) const override { return 0; }

private:
    GlassCustom(DispersionFormula* formula) : Glass(formula) {}
    friend class CustomFormula;
};


class GlassReznik : public Glass
{
public:
//    double calcIndex(const double& lambda) const override { return 0; }
//    double calcIndex1(const double& lambda) const override { return 0; }
//    double calcIndex2(const double& lambda) const override { return 0; }

private:
    GlassReznik(DispersionFormula* formula) : Glass(formula) {}
    friend class ReznikFormula;
};

#endif // GLASS_H
