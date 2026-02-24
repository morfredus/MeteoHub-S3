# FAQ

Minimum valid version: 1.0.167

## Which display is supported?
Only OLED (SH1106/SSD1306).

## Which PlatformIO environment should I use?
`esp32-s3-oled`.

## Is SD card mandatory?
No, it is optional for long-term archival.

## My SSD1306 has a yellow top band. Can UI overlap it?
No. Since version 1.0.167, the firmware applies a reserved top zone on SSD1306 and shifts page rendering to prevent overlap with this band.
