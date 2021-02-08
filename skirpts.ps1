$logo = @"
       _____    ______
      /     I  /  __  \
     /  /I  I I  /  \__\
    /  /_I  I I  I
   /  ___   I I  I   ___
  /  /   I  I I  \__/  /
 /__/    I__I  \______/

  ***    LOGGER    ***

author: Martins Klavins

"@
Write-Host $logo


$numurs = Get-WmiObject Win32_SerialPort | Where-Object {$_.PNPDeviceID -match "VID_2341&PID_8036" } | select -expand DeviceID

$port = New-Object System.IO.Ports.SerialPort
$port.PortName = $numurs
$port.BaudRate = "115200"
$port.Parity = "None"
$port.DataBits = 8
$port.StopBits = 1
$port.ReadTimeout = 3000
$port.DtrEnable = "true"

$port.open()


try
{
  while($myinput = $port.ReadLine())
  {
  $myinput
  }
}

catch [TimeoutException]
{
# Error handling code here
}

finally
{
# Any cleanup code goes here
}

$port.Close() #closes serial connection


PAUSE