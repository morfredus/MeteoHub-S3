# SD Card Troubleshooting

## SD Card not detected or mount fails

If logs show `SD Mount FAILED` or `cardType is CARD_NONE`:

1. **Reformat the card to FAT32**
   - This is the most common cause. Interrupted write operations can corrupt the file system.
   - Use a tool like **SD Memory Card Formatter** or your OS disk manager.
   - You MUST select **FAT32** file system (exFAT and NTFS are not supported).
   - Allocation unit size: 32 KB (recommended).

2. **Check Power Supply**
   - SD card must be powered at **3.3V**.
   - A module with a built-in 3.3V regulator is recommended.

3. **Check Wiring**
   - Wires should be as short as possible (< 10 cm / 4 inches).
   - Verify connections: CLK (GPIO 9), MISO (GPIO 10), MOSI (GPIO 11), CS (GPIO 12).

4. **Test another card**
   - Some old or very high capacity cards (> 64 GB) may be incompatible.
   - Prefer cards between 4 GB and 32 GB for maximum compatibility.