# AC logger
Based on [OpenEnergyMonitor](https://learn.openenergymonitor.org/electricity-monitoring/voltage-sensing/measuring-voltage-with-an-acac-power-adapter) library [EmonLib](https://github.com/openenergymonitor/EmonLib). Added SD card for data storage and Windows 10 executable file for  real-time voltage and current measurements.

### How it’s made
* Current sensor – SCT-013, Voltage sensor - selfmade (check in folder part1).
* Arduino Leonardo (because of USB HID capabilities)
* PowerShell script with such commands:
  1. *find Serial port with specific ID ( exact for AC logger device)*
  2. *create instance for this port*
  3. *set port parameters*
  4. *open and read serial port*

### Technical notes
* In SD card (in the main directory) has to be file named dati.txt.
* To create executable file (.exe) I used PS2EXE . Some things worth mentioning:
  - *Use v0.6.0 version, if you want to use application on Windows 10 ( keep in mind – to create .exe files for other platforms will require other versions and tools).*
  - *To run PS2EXE use powershell as admin. Command line will be similar to this one:* **./ps2exe.ps1 -inputFile C:\Skripti\aclog.ps1 -outputFile C:\Skripti\aclog.exe -elevated**
  - *Make sure you are in directory where is file you want to execute, this time PS2EXE.
  - *If you get error – “cannot be loaded because running scripts is disabled on this system. For more information, see about_Execution_Policies” – use this command:* **Set-ExecutionPolicy Unrestricted**
