/*
 * cgiOneWire.h
 *
 *  Created on: 03.09.2015
 *      Author: X-SzczyptaM
 */

#ifndef CGIONEWIRE_H_
#define CGIONEWIRE_H_

void  cgiOneWire(HttpdConnData *connData);
void  cgiOneWire_cfg(HttpdConnData *connData);
void  cgiOneWireManual(HttpdConnData *connData);
void  cgiOneWire_read_temp(HttpdConnData *connData);

#endif /* CGIONEWIRE_H_ */
