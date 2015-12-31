#ifndef UTIL_BATTERY_BATTERYMAC_H
#define UTIL_BATTERY_BATTERYMAC_H

#include "util/battery/battery.h"

class BatteryMac : public Battery {
    Q_OBJECT
  public:
    BatteryMac(QObject* pParent=nullptr);
    virtual ~BatteryMac();

  protected:
    void read();
};

#endif /* UTIL_BATTERY_BATTERYMAC_H */
