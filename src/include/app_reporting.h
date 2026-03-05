#ifndef SRC_INCLUDE_APP_REPORTING_H_
#define SRC_INCLUDE_APP_REPORTING_H_

void forceReportBattery();
void app_forcedReport(uint8_t endpoint, uint16_t claster_id, uint16_t attr_id);

#endif /* SRC_INCLUDE_APP_REPORTING_H_ */
