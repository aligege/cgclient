#ifndef SupernovaApple_h
#define SupernovaApple_h

#include "System.h"

class SupernovaApple: public cg::System{

public:

    SupernovaApple();
    
    virtual ~SupernovaApple();
    
    virtual sg_context_desc getSokolContext();

    virtual int getScreenWidth();
    virtual int getScreenHeight();
    
    virtual void showVirtualKeyboard(std::wstring text);
    virtual void hideVirtualKeyboard();
    
    virtual std::string getAssetPath();
    virtual std::string getUserDataPath();
    virtual std::string getLuaPath();
    
    virtual bool getBoolForKey(const char *key, bool defaultValue);
    virtual int getIntegerForKey(const char *key, int defaultValue);
    virtual long getLongForKey(const char *key, long defaultValue);
    virtual float getFloatForKey(const char *key, float defaultValue);
    virtual double getDoubleForKey(const char *key, double defaultValue);
    virtual cg::Data getDataForKey(const char* key, const cg::Data& defaultValue);
    virtual std::string getStringForKey(const char *key, std::string defaultValue);

    virtual void setBoolForKey(const char *key, bool value);
    virtual void setIntegerForKey(const char *key, int value);
    virtual void setLongForKey(const char *key, long value);
    virtual void setFloatForKey(const char *key, float value);
    virtual void setDoubleForKey(const char *key, double value);
    virtual void setDataForKey(const char* key, cg::Data& value);
    virtual void setStringForKey(const char* key, std::string value);

    virtual void removeKey(const char *key);

    virtual void initializeAdMob(bool tagForChildDirectedTreatment, bool tagForUnderAgeOfConsent);
    virtual void setMaxAdContentRating(cg::AdMobRating rating);
    virtual void loadInterstitialAd(std::string adUnitID);
    virtual bool isInterstitialAdLoaded();
    virtual void showInterstitialAd();
};


#endif /* SupernovaSokol_h */
