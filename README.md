The embedded software is running on an ATMEGA328PB on the [Tof_PCB](https://github.com/plex1/Tof_PCB).

Interface:
- Gepin Control and Status Interface
- via UART

Control:
- Set comparator level 1&2
- Set Photo diode voltage

Status:
-	Voltage measurements of supply and photo diode voltage
-	Lock in amplifier readout
-	Start and Stop counts (TxSync/RxSync)
-	Temperature

The [ioda control software](https://github.com/plex1/ioda_control_sw) was used for testing.
